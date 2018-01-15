#ifndef CODE_GENERATE_H
#define CODE_GENERATE_H
#include "GraphColoring.h"
#define ASM_OUTPUT
#define REGPOOL_SIZE 10
#define GLOBAL_REGPOOL_SIZE 8
//#define PRINTLN
//寄存器池项目
typedef struct {
	bool inUse;//该寄存器是否在使用
	string id;//对应的标识符名
	string mem;//对应的内存位置
	bool dirty;//脏数据才需要写回
} regPoolItem;
#ifdef ASM_OUTPUT
#define cout asmout
ofstream asmout;
#endif // ASM_OUTPUT
//代码生成单元
class CodeGenerator {
private:
	int curBtabIdx;//在分程序表中的函数指针，指向当前是第几个函数
	int curFuncIdx;//当前函数在符号表中的位置
	map<int, int> tempVar;//将临时变量的标号映射为它们在本层的地址
	map<string, int > func2num;//将函数映射为数字标号
	vector<regPoolItem> regPool;//寄存器池
	vector<regPoolItem> globalRegPool;//全局寄存器池
	vector<FuncBaseBlock*> funcBlocks;//函数块
	int regPoolPointer;//寄存器分配指针，用于实现类似轮转分配的效果
	FuncBaseBlock* curFuncBlock;//当前函数块
	int curBBIdx;//当前基本块的索引
	string regV0;//V0寄存器当前存的值，用于避免重复给v0寄存器赋相同的值
public:
	CodeGenerator(vector<FuncBaseBlock*> _funcBlocks) {
		funcBlocks = _funcBlocks;
		curFuncBlock = NULL;
		curBBIdx = 0;
		curBtabIdx = 0;
		curFuncIdx = 0;
		regPoolPointer = 0;
		regV0 = "";
		for (int i = 0; i < REGPOOL_SIZE; i++)
			regPool.push_back({ false,"","" ,false });
		for (int i = 0; i < GLOBAL_REGPOOL_SIZE; i++)
			globalRegPool.push_back({ false,"","",false });
	}
	//将整数变为字符串
	static string int2str(int x) {
		stringstream ss;
		ss << x;
		return ss.str();
	}
	//生成单条指令
	static void emit(string op, string dest, string src1, string src2, string comment) {//add $t1,$t0,$t1
		cout << "\t" << op << " " << dest << "," << src1 << "," << src2 << comment << endl;
	}
	static void emit(string op, string dest, string src1, int src2, string comment) {//add $t1,$t0,$t1
		cout << "\t" << op << " " << dest << "," << src1 << "," << src2 << comment << endl;
	}
	static void emit(string op, string value, string target, string comment) {//sw $t0,0($sp)
		cout << "\t" << op << " " << value << "," << target << comment << endl;
	}
	static void emit(string op, string target) {//jr $ra
		cout << "\t" << op << " " << target << endl;
	}
	//在寄存器池中查询id对应的寄存器，实际上就是找到变量与临时寄存器的对应关系
	string findInRegPool(string id) {
		//搜索相应的标识符在寄存器池中是否已经存在
		for (int i = 0; i < globalRegPool.size(); i++) {
			if (globalRegPool[i].id == id && globalRegPool[i].inUse)
				return "$s" + int2str(i);
		}
		for (int i = 0; i < regPool.size(); i++) {
			if (regPool[i].id == id && regPool[i].inUse)
				return "$t" + int2str(i);
		}
		return "";
	}
	//寄存器分配，当寄存器池全满了，则选择一个不在usedReg中的寄存器溢出
	string regAlloc(string id, set<string> usedReg, bool needload) {
		string mem = getItemAddr(id);//获取内存地址
		stringstream ss;
		int i;
		bool found = false;
		if (curFuncBlock->id2Greg.find(id) != curFuncBlock->id2Greg.end() && curFuncBlock->id2Greg[id] != -1) {
			//如果是被分配全局寄存器的变量
			i = curFuncBlock->id2Greg[id];
			globalRegPool[i].id = id;
			globalRegPool[i].inUse = true;
			globalRegPool[i].mem = mem;
			return "$s" + int2str(i);
		}
		else {//否则只能将该变量存入临时寄存器
			for (i = 0; i < regPool.size(); i++) {//检测寄存器池是否有可用寄存器
				if (regPool[i].inUse == false) {
					found = true;
					break;
				}
			}
			if (!found) {//无可用寄存器，强行分配，其中usedReg中的元素形如$t2,$t3，表示这些寄存器已经分配
				while (usedReg.find("$t" + int2str(regPoolPointer)) != usedReg.end()) {
					//如果regPoolPointer指向的寄存器在usedReg中，则不能将其回写到内存，而应该查看下一个寄存器
					regPoolPointer = (regPoolPointer + 1) % REGPOOL_SIZE;
				}
				//现在regPoolPointer对应的寄存器保存的值不会即将被用到
				i = regPoolPointer;
				if (regPool[i].dirty)
					emit("sw", "$t" + int2str(i), regPool[i].mem, "");//将其写回内存
			}
			ss << "$t" << i;
			if (needload) //如果id是""，那么这就是直接分配一个寄存器，而无需加载
				emit("lw", ss.str(), mem, " # load " + ss.str() + " = " + id);
			regPool[i].inUse = true;//此寄存器被使用
			regPool[i].id = id;
			regPool[i].mem = mem;
			regPool[i].dirty = false;//此寄存器暂时未被改变
			return ss.str();
		}
	}
	//将临时寄存器池中的所有变量全部写回内存，而刚刚取出的变量则无需再写回
	void regPoolSpillAll() {
		//将寄存器池清空，其中寄存器保存的所有变量写入相应位置
		set<string> tempOutSet = curFuncBlock->innerBlocks[curBBIdx]->outSet;
		for (int i = 0; i < regPool.size(); i++) {
			if (regPool[i].inUse && regPool[i].id != "" && regPool[i].dirty) {
				//注意只需要写回全局变量和后续会使用到的变量
				int sbtIdx = cgGlobalSearch(regPool[i].id);
				if ((sbtIdx != 0 && symbolTable[sbtIdx].lev == 0) || tempOutSet.find(regPool[i].id) != tempOutSet.end()) {
					emit("sw", "$t" + int2str(i), regPool[i].mem, " # spill " + regPool[i].id);
				}
				regPool[i].id = "";
				regPool[i].mem = "";
			}
			regPool[i].inUse = false;
			regPool[i].dirty = false;
		}
		regV0 = "";
	}
	//CodeGenerator类的全局搜索，搜索对应标识符在符号表中索引
	int cgGlobalSearch(string itemName) {
		symbolTable[0].name = itemName;//设置哨兵
		int i, curIdx = btab[curBtabIdx].lastItem;//获取当前所在函数层的最后一个符号表项
		if (curIdx == 0) {
			//如果当前的索引为0，那么说明当前函数中没有任何项，为此从前往后所搜符号表，找到相应的函数位置
			//因为此时，curBtab就指示出当前是第几个函数
			int temp = 0, j = 1;
			while (true) {
				if (symbolTable[j].objTyp == function) temp++;
				if (temp == curBtabIdx) break;
				j++;
			}
			curIdx = j;
		}
		while (true) {
			i = curIdx;
			while (symbolTable[i].name != itemName) {
				if (symbolTable[i].link == 0) curIdx = i - 1;//如果第i项的link是0，
				//说明第i项就是本层第一个项，需要将curIdx设置为i-1以返回上一层最后一项，那一项就是当前函数
				i = symbolTable[i].link;
			}
			if (curIdx == 0 || i != 0) break;//如果curIdx=0说明到达符号表第0项，表示没找到，而i!=0表示找到了对应的符号表项
		}
		return i;
	}
	//根据单个寄存器的名字获取它的编号，判断是否在regSet中，不在则插入regMap，并且更新regIdx
	void singleTemp(string& regName, set<int>& regSet, int& regIdx) {
		if (regName.size() > 0 && regName[0] == '#') {
			int tempRegId = getIntValue(false, regName.substr(regPrefix.size()));
			if (regSet.find(tempRegId) == regSet.end()) {//如果集合中不存在这个临时变量的编号
				regSet.insert(tempRegId);
				tempVar[tempRegId] = regIdx++;
			}
		}
	}
	//计算函数中的临时变量所占空间，根据当前四元式索引向后搜索临时变量
	int calTemp(int curQuadIdx) {
		set<int> regSet;
		int regIdx = 0;
		while (curQuadIdx < quadCodeTable.size()) {
			if (quadCodeTable[curQuadIdx].op == "endFunc") break;
			singleTemp(quadCodeTable[curQuadIdx].left, regSet, regIdx);//left,right,result都可能存放临时寄存器
			singleTemp(quadCodeTable[curQuadIdx].right, regSet, regIdx);
			singleTemp(quadCodeTable[curQuadIdx].result, regSet, regIdx);
			curQuadIdx++;
		}
		return regIdx;
	}
	//给定字符串，将其中的反斜杠进行转义
	static string processStr(string str) {
		stringstream ret;
		for (int i = 0; i < str.size(); i++) {
			if (str[i] == '\\') ret << "\\\\";
			else ret << str[i];
		}
		return ret.str();
	}
	//放置标签
	static void labelInstr(string label, string comment) {
		cout << label << ":" << " # " << comment << endl;
	}
	//根据四元式中给出的项名，返回其在本层的栈中的地址
	string getItemAddr(string& itemName) {
		stringstream ss;
		if (itemName.size() > 0 && itemName[0] == '#') {//是一个临时变量
			ss << (-4 * (MEM_SAVE + tempVar[getIntValue(false, itemName.substr(regPrefix.size()))] + btab[curBtabIdx].isize)) << "($fp)";
		}
		else if (itemName == "") {
			return "";
		}
		else {
			int itemIdx = cgGlobalSearch(itemName);
			if (symbolTable[itemIdx].lev == 0) //全局
				ss << (4 * symbolTable[itemIdx].addr) << "($gp)";
			else //局部
				ss << (-4 * symbolTable[itemIdx].addr) << "($fp)";
		}
		return ss.str();
	}
	//根据给定的四元式操作符quadOpe，将ope填上正确的临时寄存器的值，表示分到了该临时寄存器
	void fillOperand(string& ope, string& quadOpe, set<string> usedReg) {
		ope = findInRegPool(quadOpe);
		if (ope == "") {
			if (isConstant(quadOpe)) {
				ope = regAlloc("", usedReg, false);
				emit("li", ope, quadOpe, "");
			}
			else if (quadOpe == "@RET") {
				ope = regAlloc("", usedReg, false);
				emit("addu", ope, "$0", "$v1", "");
			}
			else {
				ope = regAlloc(quadOpe, usedReg, true);
			}
			regPool[getIntValue(false, ope.substr(2))].id = quadOpe;
		}
	}
	//根据四元式的result域，返回其相应的寄存器
	string fillResult(string& result, set<string> usedReg) {
		string dest;
		string resultReg = findInRegPool(result);
		if (resultReg == "") {//寄存器池中无该变量，则分配，但是不用lw
			int destIdx;
			if (curFuncBlock->id2Greg.find(result) != curFuncBlock->id2Greg.end() &&
				curFuncBlock->id2Greg[result] != -1) {
				//如果该变量是可以被分配全局寄存器的变量
				dest = regAlloc(result, usedReg, false);//获取结果对应的寄存器
			}
			else {//局部变量
				dest = regAlloc("", usedReg, false);//获取结果对应的寄存器
				destIdx = getIntValue(false, dest.substr(2));
				regPool[destIdx].id = result;//重新设置好id和mem
				regPool[destIdx].mem = getItemAddr(result);
			}
		}
		else {//寄存器池中有该变量，则直接使用该寄存器
			dest = resultReg;
		}
		if (dest[1] != 's')
			regPool[getIntValue(false, dest.substr(2))].dirty = true;//将脏位进行标记，表示被赋值
		return dest;
	}
	//根据四元式索引，生成跳转指令
	void branchInstr(int quadIdx) {
		string lop, rop;
		fillOperand(lop, quadCodeTable[quadIdx].left, {});
		fillOperand(rop, quadCodeTable[quadIdx].right, { lop });
		//在跳转之前清空寄存器池
		regPoolSpillAll();
		if (quadCodeTable[quadIdx].op == "==")
			emit("bne", lop, rop, quadCodeTable[quadIdx + 1].left,
				" # " + quadCodeTable[quadIdx].left + "!=" + quadCodeTable[quadIdx].right);
		else if (quadCodeTable[quadIdx].op == "!=")
			emit("beq", lop, rop, quadCodeTable[quadIdx + 1].left,
				" # " + quadCodeTable[quadIdx].left + "==" + quadCodeTable[quadIdx].right);
		else if (quadCodeTable[quadIdx].op == "<")
			emit("bge", lop, rop, quadCodeTable[quadIdx + 1].left,
				" # " + quadCodeTable[quadIdx].left + ">=" + quadCodeTable[quadIdx].right);
		else if (quadCodeTable[quadIdx].op == ">")
			emit("ble", lop, rop, quadCodeTable[quadIdx + 1].left,
				" # " + quadCodeTable[quadIdx].left + "<=" + quadCodeTable[quadIdx].right);
		else if (quadCodeTable[quadIdx].op == "<=")
			emit("bgt", lop, rop, quadCodeTable[quadIdx + 1].left,
				" # " + quadCodeTable[quadIdx].left + ">" + quadCodeTable[quadIdx].right);
		else if (quadCodeTable[quadIdx].op == ">=")
			emit("blt", lop, rop, quadCodeTable[quadIdx + 1].left,
				" # " + quadCodeTable[quadIdx].left + "<" + quadCodeTable[quadIdx].right);
	}
	//计算指令
	void calcuInstr(int quadIdx) {
		string op = quadCodeTable[quadIdx].op;
		string lop, rop, dest;
		fillOperand(lop, quadCodeTable[quadIdx].left, {});//获取左操作符对应寄存器
		fillOperand(rop, quadCodeTable[quadIdx].right, { lop });//获取右操作符对应寄存器
		dest = fillResult(quadCodeTable[quadIdx].result, { lop,rop });//获取结果标识符对应的寄存器
		if (op == "+")
			emit("addu", dest, lop, rop, " # " + quadCodeTable[quadIdx].result +
				" = " + quadCodeTable[quadIdx].left + "+" + quadCodeTable[quadIdx].right);
		else if (op == "-")
			emit("subu", dest, lop, rop, " # " + quadCodeTable[quadIdx].result +
				" = " + quadCodeTable[quadIdx].left + "-" + quadCodeTable[quadIdx].right);
		else if (op == "*")
			emit("mul", dest, lop, rop, " # " + quadCodeTable[quadIdx].result +
				" = " + quadCodeTable[quadIdx].left + "*" + quadCodeTable[quadIdx].right);
		else if (op == "/")
			emit("div", dest, lop, rop, " # " + quadCodeTable[quadIdx].result +
				" = " + quadCodeTable[quadIdx].left + "/" + quadCodeTable[quadIdx].right);
	}
	//产生函数标签
	static string funcLabel(int funcId) {
		stringstream ss;
		ss << "Func_" << funcId;
		return ss.str();
	}
	//生成MIPS指令代码
	void genOptMips() {
		int i, funcNum = 0;
		//如果有错误或者没有一条四元式，不生成mips代码
		if (errCnt != 0 || quadCodeTable.size() == 0) return;
		cout << "############################## Global String ##############################" << endl;
		cout << ".data" << endl;
		for (i = 0; i < stringTable.size(); i++) //字符串常量
			cout << "_str" << i << " : " << " .asciiz " << processStr(stringTable[i].val) << endl;
		i = 0;
		cout << "##############################  Text Segment ##############################" << endl;
		cout << ".text" << endl;
		//为常量赋值，常量指针gp充当基地址
		cout << "# initialize global constant" << endl;
		cout << "\t" << "addu $gp,$sp,$0 # let $gp=$sp to get more space" << endl;
		cout << "\t" << "subi $gp,$gp," << (btab[0].isize * 4) << " # make space for global variabel and constant" << endl;
		cout << "# initialize global constant" << endl;
		while (true) {//为全局常量赋初始值
			if (quadCodeTable[i].op == "const") {
				cout << "\t" << "li $t0," << quadCodeTable[i].result << " # " << quadCodeTable[i].right << "=" << quadCodeTable[i].result << endl;
				//由于全局常量的声明一定在程序的前面，所以它们的地址就是索引i乘以4
				cout << "\t" << "sw $t0," << (i * 4) << "($gp)" << endl;
			}
			else break;
			i++;
		}
		//为函数建立 函数名-索引 的映射关系，目的是将所有函数名映射为'Func_n'的形式
		for (int j = 0; j < quadCodeTable.size(); j++) {
			if (quadCodeTable[j].right == "()")  //函数
				func2num[quadCodeTable[j].left] = funcNum++;//更新函数索引
		}
		cout << "\t" << "addu $sp,$gp,$0 # update $sp" << endl;
		cout << "\t" << "j " << funcLabel(func2num["main"]) << " # jump to main function" << endl;//跳转到主函数开始执行
		while (i < quadCodeTable.size() && !isQuadFunc(quadCodeTable[i])) //跳转到函数出现
			i++;
		while (i < quadCodeTable.size()) {
			bool spilled = false;
			if (isQuadFunc(quadCodeTable[i])) {//函数声明，注意它们都是全局的
				tempVar.clear();//清空临时变量与索引的映射关系
				string func_name = quadCodeTable[i].left;//函数名
				curFuncBlock = funcBlocks[curBtabIdx];//当前函数基本块
				curBBIdx = 0;//当前基本块在函数块内的索引为0
				curBtabIdx++;//函数索引加一
				curFuncIdx = cgGlobalSearch(func_name);//找到此函数对应的符号表项索引
				for (int k = 0; k < globalRegPool.size(); k++)
					globalRegPool[k] = { false,"","",false };
				labelInstr(funcLabel(func2num[func_name]), "function " + func_name);//产生函数下标
				//$sp下降，继续为保留空间留出位置，所谓保留空间，是指$ra与$fp等在函数调用的过程需要保存的数据
				emit("subi", "$sp", "$sp", int2str(MEM_SAVE * 4), " # make space for $fp and $ra");
				//约定旧的fp在新的fp所指向的位置，而旧的ra在旧的fp下面
				emit("sw", "$fp", int2str((MEM_SAVE - 1) * 4) + "($sp)", " # save $fp");//保存fp指针
				emit("sw", "$ra", int2str((MEM_SAVE - 2) * 4) + "($sp)", " # save $ra");//保存函数返回地址
				emit("addi", "$fp", "$sp", ((MEM_SAVE - 1) * 4 + symbolTable[curFuncIdx].dimen * 4), " # $fp = old $sp");//现在fp充当定位的base
				if (btab[curBtabIdx].isize * 4 + calTemp(i) * 4 - btab[curBtabIdx].psize * 4 != 0)
					emit("subi", "$sp", "$sp", (btab[curBtabIdx].isize * 4 + calTemp(i) * 4 - btab[curBtabIdx].psize * 4),
						" # make space for locals and temps");//为局部变量以及临时变量留空间
			}
			else if (quadCodeTable[i].op == "endFunc") {//函数结束	exit_Func_1 # exit function foo
				regPoolSpillAll();
				spilled = true;
				labelInstr("exit_" + funcLabel(func2num[quadCodeTable[i].left]), "exit function " + quadCodeTable[i].left);
				if ((btab[curBtabIdx].isize - btab[curBtabIdx].psize + tempVar.size()) * 4 != 0)
					emit("addi", "$sp", "$sp", ((btab[curBtabIdx].isize - btab[curBtabIdx].psize + tempVar.size()) * 4), " # pop locals and temps");//弹栈
				emit("lw", "$fp", int2str((MEM_SAVE - 1) * 4) + "($sp)", "");
				emit("lw", "$ra", int2str((MEM_SAVE - 2) * 4) + "($sp)", "");
				emit("addi", "$sp", "$sp", ((symbolTable[curFuncIdx].dimen + MEM_SAVE) * 4), "");//恢复参数区
				if (quadCodeTable[i].left != "main") //不是主函数才返回
					emit("jr", "$ra");
				else {
					emit("addi", "$sp", "$sp", (btab[0].isize * 4), "");//主函数无需返回，直接恢复sp指针，这是刚才被gp下降占用的空间
					emit("li", "$gp", "0x10008000", " # restore $gp");//恢复gp为默认值
				}
				//由于优化过程中产生了多余的四元式，通过下面代码将其略过
				while (i + 1 < quadCodeTable.size() && !isQuadFunc(quadCodeTable[i + 1]))//跳到下一个函数出现或者整个四元式表被遍历完成
					i++;
			}
			else if (quadCodeTable[i].op == "const") {//局部常量声明
				int itemAddr = cgGlobalSearch(quadCodeTable[i].right);
				BaseBlock* tempBB = curFuncBlock->innerBlocks[curBBIdx];
				//如果常量在out或者use集合里面，就需要对其进行赋值
				if (tempBB->outSet.find(quadCodeTable[i].right) != tempBB->outSet.end() ||
					tempBB->useSet.find(quadCodeTable[i].right) != tempBB->useSet.end()) {
					emit("li", "$t0", int2str(symbolTable[itemAddr].value), " # const " + quadCodeTable[i].right + "=" + quadCodeTable[i].result);
					// 这里的symbolTable[itemAddr].addr就是加上了MEM_SAVE之后的，所以无需再加了
					emit("sw", "$t0", int2str(-(symbolTable[itemAddr].addr) * 4) + "($fp)", "");
				}
			}
			else if (quadCodeTable[i].op == "push") {//将参数压入栈中
				string _tn;
				fillOperand(_tn, quadCodeTable[i].left, {});
				emit("subi", "$sp", "$sp", "4", "");//压栈
				emit("sw", _tn, "0($sp)", "");//存入参数
			}
			else if (quadCodeTable[i].op == "call") {//函数调用，实际上就是一个跳转
				regPoolSpillAll();//全部写回
				spilled = true;
				for (int k = 0; k < globalRegPool.size(); k++) {
					if (globalRegPool[k].inUse && curFuncBlock->innerBlocks[curBBIdx]->outSet.find(globalRegPool[k].id) !=
						curFuncBlock->innerBlocks[curBBIdx]->outSet.end())
						emit("sw", "$s" + int2str(k), globalRegPool[k].mem, " # save " + globalRegPool[k].id + "(global reg)");//存入参数
				}
				emit("jal", funcLabel(func2num[quadCodeTable[i].left]));
				for (int k = 0; k < globalRegPool.size(); k++) {
					if (globalRegPool[k].inUse && curFuncBlock->innerBlocks[curBBIdx]->outSet.find(globalRegPool[k].id) !=
						curFuncBlock->innerBlocks[curBBIdx]->outSet.end())
						emit("lw", "$s" + int2str(k), globalRegPool[k].mem, " # save " + globalRegPool[k].id + "(global reg)");//存入参数
				}
			}
			else if (isQuadComp(quadCodeTable[i])) {//关系运算符
				branchInstr(i);
			}
			else if (isQuadLabel(quadCodeTable[i])) {//Label类型指令
				cout << quadCodeTable[i].op << endl;//直接输出标号即可
			}
			else if (quadCodeTable[i].op == "GOTO") {//GOTO类型指令
				regPoolSpillAll();
				spilled = true;
				emit("j", quadCodeTable[i].left);
			}
			else if (quadCodeTable[i].op == "ret") {
				if (quadCodeTable[i].left != "") { //只有当操作数不为0的时候才继续，否则可能是void函数的返回语句
					//约定用$v1返回值
					if (isConstant(quadCodeTable[i].left)) {
						emit("li", "$v1", quadCodeTable[i].left, " # @RET = " + quadCodeTable[i].left);
					}
					else if (quadCodeTable[i].left != "@RET") {//如果四元式的left域不是@RET，那么需要对$v1进行赋值，否则不需要
						if (findInRegPool(quadCodeTable[i].left) == "")
							emit("lw", "$v1", getItemAddr(quadCodeTable[i].left), " # @RET = " + quadCodeTable[i].left);
						else
							emit("addu", "$v1", "$0", findInRegPool(quadCodeTable[i].left), " # @RET = " + quadCodeTable[i].left);
					}
				}
				regPoolSpillAll();//清空寄存器池
				spilled = true;
				emit("j", "exit_" + funcLabel(func2num[symbolTable[curFuncIdx].name]));//离开函数
			}
			else if (isQuadCalc(quadCodeTable[i])) {//运算类指令
				calcuInstr(i);
			}
			else if (quadCodeTable[i].op == "=") {//简单赋值
				string lop, dest;
				if (isConstant(quadCodeTable[i].left)) {
					dest = fillResult(quadCodeTable[i].result, {});
					emit("li", dest, quadCodeTable[i].left, " # " + dest + "=" + quadCodeTable[i].left);
				}
				else if (quadCodeTable[i].left == "@RET") {
					dest = fillResult(quadCodeTable[i].result, {});
					emit("addu", dest, "$0", "$v1", " # " + dest + "=" + quadCodeTable[i].left);//完成赋值
				}
				else {
					fillOperand(lop, quadCodeTable[i].left, {});//获取左操作数寄存器
					dest = fillResult(quadCodeTable[i].result, { lop });//获取结果标识符对应寄存器
					emit("addu", dest, "$0", lop, " # " + dest + "=" + quadCodeTable[i].left);//完成赋值
				}
			}
			else if (quadCodeTable[i].op == "=[]") {//从数组中取值
				string lop, rop, dest;//三个操作符
				string arrIdx;//数组下标
				fillOperand(rop, quadCodeTable[i].right, {});
				arrIdx = regAlloc("", { rop }, false);
				emit("sll", arrIdx, rop, "2", "");//乘以4得到偏移量
				lop = regAlloc("", { arrIdx,rop }, false);//数组名不会是dirty，id域也始终是""，所以不会被回写，也不会被反复利用
				int itemIdx = cgGlobalSearch(quadCodeTable[i].left);
				//获取数组的地址
				if (symbolTable[itemIdx].lev == 0) { //全局
					emit("addiu", lop, "$gp", (4 * symbolTable[itemIdx].addr), "");
					emit("addu", lop, lop, arrIdx, "");
				}
				else { //局部
					emit("subi", lop, "$fp", (4 * symbolTable[itemIdx].addr), "");
					emit("subu", lop, lop, arrIdx, "");
				}
				//查找结果变量是否在寄存器中存在
				dest = fillResult(quadCodeTable[i].result, { lop,rop,arrIdx });
				//现在lop存放元素地址
				emit("lw", dest, "0(" + lop + ")",
					" # " + quadCodeTable[i].result + "=" + quadCodeTable[i].left + "[" + quadCodeTable[i].right + "]");
			}
			else if (quadCodeTable[i].op == "[]=") {//赋值到数组元素,[]=,#reg0,#reg1,a		a[#reg0] = #reg1
				string lop, rop, dest;//左右操作符，目标符
				string arrIdx;//数组下标
				fillOperand(lop, quadCodeTable[i].left, {});//获取下标所在的寄存器
				arrIdx = regAlloc("", { lop }, false);//申请新的寄存器存放下标*4的值
				emit("sll", arrIdx, lop, "2", "");//得到数组下标
				dest = regAlloc("", { lop,arrIdx }, false);//获取目标地址
				int itemIdx = cgGlobalSearch(quadCodeTable[i].result);
				if (symbolTable[itemIdx].lev == 0) { //全局
					emit("addiu", dest, "$gp", (4 * symbolTable[itemIdx].addr), "");
					emit("addu", dest, dest, arrIdx, "");
				}
				else { //局部
					emit("subi", dest, "$fp", (4 * symbolTable[itemIdx].addr), "");
					emit("subu", dest, dest, arrIdx, "");
				}
				fillOperand(rop, quadCodeTable[i].right, { lop,dest,arrIdx });//获取写入数组的值
				emit("sw", rop, "0(" + dest + ")",
					" # " + quadCodeTable[i].result + "[" + quadCodeTable[i].left + "] = " + quadCodeTable[i].right);//将结果写入数组对应位置
				regPoolSpillAll();//清空寄存器
				spilled = true;
			}
			else if (quadCodeTable[i].op == "scanfInt" || quadCodeTable[i].op == "scanfChr") {//读入整数或者字符
				string dest;
				regPoolSpillAll();//清空寄存器
				spilled = true;
				if (quadCodeTable[i].op == "scanfInt") {
					if (regV0 != "5") emit("li", "$v0", "5", " # read int : " + quadCodeTable[i].left);
					regV0 = "5";
				}
				else {
					if (regV0 != "12")emit("li", "$v0", "12", " # read char : " + quadCodeTable[i].left);
					regV0 = "12";
				}
				emit("syscall", "");
				regV0 = "";
				if (curFuncBlock->id2Greg.find(quadCodeTable[i].left) != curFuncBlock->id2Greg.end()) {//如果是分配全局寄存器的变量
					dest = "$s" + int2str(curFuncBlock->id2Greg[quadCodeTable[i].left]);
					emit("addu", dest, "$0", "$v0", "");
					globalRegPool[getIntValue(false, dest.substr(2))].id = quadCodeTable[i].left;
					globalRegPool[getIntValue(false, dest.substr(2))].mem = getItemAddr(quadCodeTable[i].left);
					globalRegPool[getIntValue(false, dest.substr(2))].inUse = true;
				}
				else {
					dest = findInRegPool(quadCodeTable[i].left);
					if (dest == "") {
						emit("sw", "$v0", getItemAddr(quadCodeTable[i].left), "");
					}
					else {
						emit("addu", dest, "$0", "$v0", "");
						regPool[getIntValue(false, dest.substr(2))].dirty = true;
						regPool[getIntValue(false, dest.substr(2))].inUse = true;
					}
				}
			}
			else if (quadCodeTable[i].op == "printInt" || quadCodeTable[i].op == "printChr") {
				if (quadCodeTable[i].op == "printInt") {
					if (regV0 != "1") emit("li", "$v0", "1", "# print int : " + quadCodeTable[i].left);
					regV0 = "1";
				}
				else {
					if (regV0 != "11")emit("li", "$v0", "11", " # print char : " + quadCodeTable[i].left);
					regV0 = "11";
				}
				if (isConstant(quadCodeTable[i].left)) {  //针对数字常量情况
					emit("li", "$a0", quadCodeTable[i].left, "");
				}
				else if (quadCodeTable[i].left == "@RET") {//针对@RET的情况
					emit("addu", "$a0", "$0", "$v1", "");
				}
				else {//针对寄存器传值情况
					if (findInRegPool(quadCodeTable[i].left) == "") //寄存器池中没有，不得不访问内存
						emit("lw", "$a0", getItemAddr(quadCodeTable[i].left), "");
					else //寄存器池中已经有，直接使用
						emit("addu", "$a0", "$0", findInRegPool(quadCodeTable[i].left), "");
				}
				emit("syscall", "");
			}
			else if (quadCodeTable[i].op == "printStr") {//输出字符串
				emit("li", "$v0", "4", " # print string");
				regV0 = "4";
				emit("la", "$a0", "_" + quadCodeTable[i].left.substr(1), "");
				emit("syscall", "");
			}
#ifdef PRINTLN
			else if (quadCodeTable[i].op == "println") {//输出换行
				emit("li", "$v0", "11", " # println");
				regV0 = "11";
				emit("li", "$a0", "'\\n'", "");
				emit("syscall", "");
			}
#endif // PRINTLN
			else if (quadCodeTable[i].op == "dagBegin") {
				curBBIdx++;
			}
			if (i + 1 < quadCodeTable.size() && quadCodeTable[i + 1].op == "dagBegin" && !spilled) {
				//如果下一条四元式是基本块的头部，而且此基本块没有进行过寄存器池的清空，那么需要清空，
				//否则会发生本基本块的寄存器池到下一个基本块才清空的情况
				regPoolSpillAll();
			}
			i++;
		}
	}
};

class NaiveGenerator {//不加优化的代码生成器
private:
	int curBtab;//在分程序表中的函数指针，指向当前是第几个函数
	int curFunc;//当前函数在符号表中的位置
	map<int, int> tempVar;//将临时变量的标号映射为它们在本层的地址
	map<string, int > funcIdx;
	int funcNum;
	int regPoolPointer;//寄存器分配指针
public:
	NaiveGenerator() {
		curBtab = 0;
		curFunc = 0;
		funcNum = 0;
		regPoolPointer = 0;
	}
	//CodeGenerator类的全局搜索，搜索对应标识符在符号表中索引
	int cgGlobalSearch(string itemName) {
		symbolTable[0].name = itemName;//设置哨兵
		int i, curIdx = btab[curBtab].lastItem;//获取当前所在函数层的最后一个符号表项
		if (curIdx == 0) {
			int temp = 0, j = 1;
			while (true) {
				if (symbolTable[j].objTyp == function)
					temp++;
				if (temp == curBtab) break;
				j++;
			}
			curIdx = j;
		}
		while (true) {
			i = curIdx;
			while (symbolTable[i].name != itemName) {
				if (symbolTable[i].link == 0) curIdx = i - 1;//如果第i项的link是0，
															 //说明第i项就是本层第一个项，需要将curIdx设置为i-1以返回上一层最后一项，那一项就是当前函数
				i = symbolTable[i].link;
			}
			if (curIdx == 0 || i != 0) break;//如果curIdx=0说明到达符号表第0项，表示没找到，而i!=0表示找到了对应的符号表项
		}
		return i;
	}
	//根据单个寄存器的名字获取它的编号，判断是否在regSet中，不在则插入regMap，并且更新regIdx
	void singleTemp(string& regName, set<int>& regSet, int& regIdx) {
		if (regName.size() > 0 && regName[0] == '#') {
			int tempRegId = getIntValue(false, regName.substr(regPrefix.size()));
			if (regSet.find(tempRegId) == regSet.end()) {//如果集合中不存在这个临时变量的编号
				regSet.insert(tempRegId);
				tempVar[tempRegId] = regIdx++;
			}
		}
	}
	//计算临时变量所占空间，根据当前四元式索引向后搜索临时变量
	int calTemp(int curQuadIdx) {
		set<int> regSet;
		int regIdx = 0;
		while (curQuadIdx < quadCodeTable.size()) {
			if (quadCodeTable[curQuadIdx].op == "endFunc") break;
			singleTemp(quadCodeTable[curQuadIdx].left, regSet, regIdx);//left,right,result都可能存放临时寄存器
			singleTemp(quadCodeTable[curQuadIdx].right, regSet, regIdx);
			singleTemp(quadCodeTable[curQuadIdx].result, regSet, regIdx);
			curQuadIdx++;
		}
		return regIdx;
	}
	//根据四元式中给出的项名，返回其在本层的栈中的地址
	string getItemAddr(string& itemName) {
		stringstream ss;
		if (itemName.size() > 0 && itemName[0] == '#') {//是一个临时变量
			ss << (-4 * (MEM_SAVE + tempVar[getIntValue(false, itemName.substr(regPrefix.size()))] + btab[curBtab].isize)) << "($fp)";
		}
		else if (itemName == "") {
			return "";
		}
		else {
			int itemIdx = cgGlobalSearch(itemName);
			if (symbolTable[itemIdx].lev == 0) //全局
				ss << (4 * symbolTable[itemIdx].addr) << "($gp)";
			else //局部
				ss << (-4 * symbolTable[itemIdx].addr) << "($fp)";
		}
		return ss.str();
	}
	//根据四元式索引，生成跳转指令
	void branchInstr(int quadIdx) {
		if (isConstant(quadCodeTable[quadIdx].left))//常数情况
			CodeGenerator::emit("li", "$t0", quadCodeTable[quadIdx].left, "");
		else if (quadCodeTable[quadIdx].left == "@RET")//RET情况
			CodeGenerator::emit("addu", "$t0", "$0", "$v1", "");
		else
			CodeGenerator::emit("lw", "$t0", getItemAddr(quadCodeTable[quadIdx].left), "");//其他情况
		if (isConstant(quadCodeTable[quadIdx].right))
			CodeGenerator::emit("li", "$t1", quadCodeTable[quadIdx].right, "");
		else if (quadCodeTable[quadIdx].right == "@RET")
			CodeGenerator::emit("addu", "$t1", "$0", "$v1", "");
		else
			CodeGenerator::emit("lw", "$t1", getItemAddr(quadCodeTable[quadIdx].right), "");
		if (quadCodeTable[quadIdx].op == "==")
			CodeGenerator::emit("bne", "$t0", "$t1", quadCodeTable[quadIdx + 1].left, "");
		else if (quadCodeTable[quadIdx].op == "!=")
			CodeGenerator::emit("beq", "$t0", "$t1", quadCodeTable[quadIdx + 1].left, "");
		else if (quadCodeTable[quadIdx].op == "<")
			CodeGenerator::emit("bge", "$t0", "$t1", quadCodeTable[quadIdx + 1].left, "");
		else if (quadCodeTable[quadIdx].op == ">")
			CodeGenerator::emit("ble", "$t0", "$t1", quadCodeTable[quadIdx + 1].left, "");
		else if (quadCodeTable[quadIdx].op == "<=")
			CodeGenerator::emit("bgt", "$t0", "$t1", quadCodeTable[quadIdx + 1].left, "");
		else if (quadCodeTable[quadIdx].op == ">=")
			CodeGenerator::emit("blt", "$t0", "$t1", quadCodeTable[quadIdx + 1].left, "");
	}
	//计算指令
	void calcuInstr(int quadIdx) {
		string op = quadCodeTable[quadIdx].op;
		if (isConstant(quadCodeTable[quadIdx].left))
			CodeGenerator::emit("li", "$t0", quadCodeTable[quadIdx].left, "");
		else if (quadCodeTable[quadIdx].left == "@RET")
			CodeGenerator::emit("addu", "$t0", "$0", "$v1", "");
		else
			CodeGenerator::emit("lw", "$t0", getItemAddr(quadCodeTable[quadIdx].left), "");
		if (isConstant(quadCodeTable[quadIdx].right))
			CodeGenerator::emit("li", "$t1", quadCodeTable[quadIdx].right, "");
		else if (quadCodeTable[quadIdx].right == "@RET")
			CodeGenerator::emit("addu", "$t1", "$0", "$v1", "");
		else
			CodeGenerator::emit("lw", "$t1", getItemAddr(quadCodeTable[quadIdx].right), "");
		if (op == "+")
			CodeGenerator::emit("addu", "$t2", "$t0", "$t1", "");
		else if (op == "-")
			CodeGenerator::emit("subu", "$t2", "$t0", "$t1", "");
		else if (op == "*")
			CodeGenerator::emit("mul", "$t2", "$t0", "$t1", "");
		else if (op == "/")
			CodeGenerator::emit("div", "$t2", "$t0", "$t1", "");
		CodeGenerator::emit("sw", "$t2", getItemAddr(quadCodeTable[quadIdx].result), "");
	}
	//生成MIPS指令代码
	void generateMips() {
		int i;
		//如果有错误或者没有一条四元式，不生成mips代码
		if (errCnt != 0 || quadCodeTable.size() == 0) return;
		cout << "############################## Global String ##############################" << endl;
		cout << ".data" << endl;
		for (i = 0; i < stringTable.size(); i++) //字符串常量
			cout << "_str" << i << " : " << " .asciiz " << CodeGenerator::processStr(stringTable[i].val) << endl;
		i = 0;
		cout << "##############################  Text Segment ##############################" << endl;
		cout << ".text" << endl;
		//为常量赋值，常量指针gp充当基地址
		cout << "# initialize global constant" << endl;
		cout << "addu $gp,$sp,$0 # let $gp=$sp to get more space" << endl;
		cout << "subu $gp,$gp," << (btab[0].isize * 4) << " # make space for global variabel and constant" << endl;
		while (true) {//为全局常量赋初始值
			if (quadCodeTable[i].op == "const") {
				cout << "li $t0," << quadCodeTable[i].result << " # " 
					<< quadCodeTable[i].right << "=" << quadCodeTable[i].result << endl;
				cout << "sw $t0," << (i * 4) << "($gp)" << endl;
			}
			else break;
			i++;
		}
		for (int i = 0; i < quadCodeTable.size(); i++) {
			if (quadCodeTable[i].right == "()")  //函数
				funcIdx[quadCodeTable[i].left] = funcNum++;//更新函数索引
		}
		cout << "addu $sp,$gp,$0 # update $sp" << endl;
		cout << "j " << CodeGenerator::funcLabel(funcIdx["main"]) << " # jump to main function" << endl;//跳转到主函数开始执行
		while (i < quadCodeTable.size() && !isQuadFunc(quadCodeTable[i])) //跳转到函数出现
			i++;
		while (i < quadCodeTable.size()) {
			if (isQuadFunc(quadCodeTable[i])) {//函数声明，注意它们都是全局的
				tempVar.clear();//清空临时变量与索引的映射关系
				string func_name = quadCodeTable[i].left;//函数名
				curBtab++;//函数索引加一
				curFunc = cgGlobalSearch(func_name);//找到此函数对应的符号表项索引
				cout << endl;
				CodeGenerator::labelInstr(CodeGenerator::funcLabel(funcIdx[func_name]), "function " + func_name);//产生函数下标
				CodeGenerator::emit("subu", "$sp", "$sp", "8", " # make space for $fp and $ra");
				CodeGenerator::emit("sw", "$fp", "4($sp)", " # save $fp");//保存fp指针
				CodeGenerator::emit("sw", "$ra", "0($sp)", " # save $ra");//保存函数返回地址
														   //下一行第一个常数4是为了寻址方便，偏移了1位
				CodeGenerator::emit("addu", "$fp", "$sp", (4 + symbolTable[curFunc].dimen * 4), " # fp = new base");//现在fp充当定位的base
				CodeGenerator::emit("subu", "$sp", "$sp", (btab[curBtab].isize * 4 + calTemp(i) * 4 - btab[curBtab].psize * 4),
					" # make space for locals and temps");;//为局部变量以及临时变量留空间
			}
			else if (quadCodeTable[i].op == "endFunc") {//函数结束
														//exit_Func_1 # exit function foo
				CodeGenerator::labelInstr("exit_" + CodeGenerator::funcLabel(funcIdx[quadCodeTable[i].left]), "exit function " + quadCodeTable[i].left);
				CodeGenerator::emit("addu", "$sp", "$sp", ((btab[curBtab].isize - btab[curBtab].psize + tempVar.size()) * 4), " # pop locals and temps");//弹栈
				CodeGenerator::emit("lw", "$fp", "4($sp)", "");
				CodeGenerator::emit("lw", "$ra", "0($sp)", "");
				CodeGenerator::emit("addu", "$sp", "$sp", ((btab[curBtab].psize + MEM_SAVE) * 4), "");;//恢复参数区
				if (quadCodeTable[i].left != "main") //不是主函数才返回
					CodeGenerator::emit("jr", "$ra");
				else {
					CodeGenerator::emit("addu", "$sp", "$sp", (btab[0].isize * 4), "");//主函数无需返回，直接恢复sp指针，这是刚才被gp下降占用的空间
					CodeGenerator::emit("li", "$gp", "0x10008000", " # restore $gp");//恢复gp为默认值
				}
				//由于优化过程中产生了多余的四元式，通过下面代码将其略过
				while (i + 1 < quadCodeTable.size() && !isQuadFunc(quadCodeTable[i + 1]))//跳到下一个函数出现或者整个四元式表被遍历完成
					i++;
			}
			else if (quadCodeTable[i].op == "const") {//局部常量声明
				int itemAddr = cgGlobalSearch(quadCodeTable[i].right);
				CodeGenerator::emit("li", "$t0", CodeGenerator::int2str(symbolTable[itemAddr].value), " # " + quadCodeTable[i].right + "=" + quadCodeTable[i].result);
				CodeGenerator::emit("sw", "$t0", CodeGenerator::int2str(-(symbolTable[itemAddr].addr) * 4) + "($fp)", "");
			}
			else if (quadCodeTable[i].op == "push") {//将参数压入栈中
				if (isConstant(quadCodeTable[i].left))
					CodeGenerator::emit("li", "$t0", quadCodeTable[i].left, " # push " + quadCodeTable[i].left);
				else if (quadCodeTable[i].left == "@RET")
					CodeGenerator::emit("addu", "$t0", "$0", "$v1", "");//RET情况
				else
					CodeGenerator::emit("lw", "$t0", getItemAddr(quadCodeTable[i].left), " # push " + quadCodeTable[i].left);
				CodeGenerator::emit("subu", "$sp", "$sp", "4", "");
				CodeGenerator::emit("sw", "$t0", "0($sp)", "");
			}
			else if (quadCodeTable[i].op == "call") {//函数调用，实际上就是一个跳转
				CodeGenerator::emit("jal", CodeGenerator::funcLabel(funcIdx[quadCodeTable[i].left]));
				CodeGenerator::emit("addu", "$v1", "$0", "$v0", " # save the return value");
			}
			else if (isQuadComp(quadCodeTable[i])) {//关系运算符
				branchInstr(i);
			}
			else if (isQuadLabel(quadCodeTable[i])) {//Label类型指令
				cout << quadCodeTable[i].op << endl;//直接输出标号即可
			}
			else if (quadCodeTable[i].op == "GOTO") {//GOTO类型指令
				CodeGenerator::emit("j", quadCodeTable[i].left);
			}
			else if (quadCodeTable[i].op == "ret") {
				if (quadCodeTable[i].left != "") { //只有当操作数不为0的时候才继续，否则可能是void函数的返回语句
					if (isConstant(quadCodeTable[i].left))
						CodeGenerator::emit("li", "$v0", quadCodeTable[i].left, " # return" + quadCodeTable[i].left);
					else if (quadCodeTable[i].left == "@RET")
						CodeGenerator::emit("addu", "$v0", "$0", "$v1", "");//RET情况
					else
						CodeGenerator::emit("lw", "$v0", getItemAddr(quadCodeTable[i].left), "");
				}
				CodeGenerator::emit("j", "exit_" + CodeGenerator::funcLabel(funcIdx[symbolTable[curFunc].name]));//离开函数
			}
			else if (isQuadCalc(quadCodeTable[i])) {//运算类指令
				calcuInstr(i);
			}
			else if (quadCodeTable[i].op == "=") {//简单赋值
				if (quadCodeTable[i].left == "@RET") {//等于返回值
					CodeGenerator::emit("sw", "$v1", getItemAddr(quadCodeTable[i].result), " # " + quadCodeTable[i].result + "=@RET");
				}
				else if (isConstant(quadCodeTable[i].left)) { //等于常量
					CodeGenerator::emit("li", "$t0", quadCodeTable[i].left, " # " + quadCodeTable[i].result + "=" + quadCodeTable[i].left);
					CodeGenerator::emit("sw", "$t0", getItemAddr(quadCodeTable[i].result), "");
				}
				else {//否则是等于一个标识符或者临时变量
					CodeGenerator::emit("lw", "$t0", getItemAddr(quadCodeTable[i].left),
						" # " + quadCodeTable[i].result + "=" + quadCodeTable[i].left);
					CodeGenerator::emit("sw", "$t0", getItemAddr(quadCodeTable[i].result), "");
				}
			}
			else if (quadCodeTable[i].op == "=[]") {//从数组中取值
				string arrIdx = quadCodeTable[i].right;
				if (isConstant(arrIdx)) //cout << "li $t0," << arrIdx << endl;//获取数组下标
					CodeGenerator::emit("li", "$t0", arrIdx, "");
				else if (arrIdx == "@RET")
					CodeGenerator::emit("addu", "$t0", "$0", "$v1", "");
				else //cout << "lw $t0," << getItemAddr(arrIdx) << endl;
					CodeGenerator::emit("lw", "$t0", getItemAddr(arrIdx), "");
				CodeGenerator::emit("sll", "$t0", "$t0", "2", "");
				int itemIdx = cgGlobalSearch(quadCodeTable[i].left);
				if (symbolTable[itemIdx].lev == 0) { //全局
					CodeGenerator::emit("addu", "$t1", "$gp", (4 * symbolTable[itemIdx].addr), "");
					CodeGenerator::emit("addu", "$t1", "$t1", "$t0", "");
				}
				else { //局部
					CodeGenerator::emit("subu", "$t1", "$fp", (4 * symbolTable[itemIdx].addr), "");
					CodeGenerator::emit("subu", "$t1", "$t1", "$t0", "");
				}
				//现在$t1存放元素地址
				CodeGenerator::emit("lw", "$t0", "0($t1)", "");
				CodeGenerator::emit("sw", "$t0", getItemAddr(quadCodeTable[i].result), "");
			}
			else if (quadCodeTable[i].op == "[]=") {//赋值到数组元素,[]=,#reg0,#reg1,a		a[#reg0] = #reg1
				string arrIdx = quadCodeTable[i].left;
				if (isConstant(arrIdx))
					CodeGenerator::emit("li", "$t0", arrIdx, "");//获取数组下标
				else if (arrIdx == "@RET")
					CodeGenerator::emit("addu", "$t0", "$0", "$v1", "");
				else
					CodeGenerator::emit("lw", "$t0", getItemAddr(arrIdx), "");
				CodeGenerator::emit("sll", "$t0", "$t0", "2", "");
				int itemIdx = cgGlobalSearch(quadCodeTable[i].result);
				if (symbolTable[itemIdx].lev == 0) { //全局
					CodeGenerator::emit("addu", "$t1", "$gp", (4 * symbolTable[itemIdx].addr), "");
					CodeGenerator::emit("addu", "$t1", "$t1", "$t0", "");
				}
				else { //局部
					CodeGenerator::emit("subu", "$t1", "$fp", (4 * symbolTable[itemIdx].addr), "");
					CodeGenerator::emit("subu", "$t1", "$t1", "$t0", "");
				}
				if (isConstant(quadCodeTable[i].right))
					CodeGenerator::emit("li", "$t0", quadCodeTable[i].right, "");//获取相应的值
				else if (quadCodeTable[i].right == "@RET")
					CodeGenerator::emit("addu", "$t0", "$0", "$v1", "");
				else
					CodeGenerator::emit("lw", "$t0", getItemAddr(quadCodeTable[i].right), "");
				CodeGenerator::emit("sw", "$t0", "0($t1)", "");//将结果写入数组对应位置
			}
			else if (quadCodeTable[i].op == "scanfInt" || quadCodeTable[i].op == "scanfChr") {//读入整数类型标识符
				if (quadCodeTable[i].op == "scanfInt") CodeGenerator::emit("li", "$v0", "5", "# read int");
				else CodeGenerator::emit("li", "$v0", "12", "# read char");
				CodeGenerator::emit("syscall", "");
				CodeGenerator::emit("sw", "$v0", getItemAddr(quadCodeTable[i].left), " # scanfChr " + quadCodeTable[i].left);
			}
			else if (quadCodeTable[i].op == "printInt" || quadCodeTable[i].op == "printChr") {
				if (quadCodeTable[i].op == "printInt") CodeGenerator::emit("li", "$v0", "1", "# print int " + quadCodeTable[i].left);
				else CodeGenerator::emit("li", "$v0", "11", " # print char : " + quadCodeTable[i].left);
				if (isConstant(quadCodeTable[i].left))   //针对数字常量情况
					CodeGenerator::emit("li", "$a0", quadCodeTable[i].left, "");
				else if (quadCodeTable[i].left == "@RET") //要输出的是v0中的值
					CodeGenerator::emit("addu", "$a0", "$0", "$v1", "");//v0可能被之前的代码修改，所以选用v1，v1一直保持刚才调用函数的结果
				else //针对寄存器传值情况
					CodeGenerator::emit("lw", "$a0", getItemAddr(quadCodeTable[i].left), "");
				CodeGenerator::emit("syscall", "");
			}
			else if (quadCodeTable[i].op == "printStr") {//输出字符串
				CodeGenerator::emit("li", "$v0", "4", " # print string");
				CodeGenerator::emit("la", "$a0", "_" + quadCodeTable[i].left.substr(1), "");
				CodeGenerator::emit("syscall", "");
			}
#ifdef PRINTLN
			else if (quadCodeTable[i].op == "println") {//输出换行
				CodeGenerator::emit("li", "$v0", "11", " # print char");
				CodeGenerator::emit("li", "$a0", "'\\n'", "");
				CodeGenerator::emit("syscall", "");
			}
#endif
			i++;
		}
	}
};
#endif // CODE_GENERATE_H