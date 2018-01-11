#ifndef CODE_GENERATE_H
#define CODE_GENERATE_H
#include "GraphColoring.h"
#define ASM_OUTPUT
#define REGPOOL_SIZE 10
#define GLOBAL_REGPOOL_SIZE 8
//#define PRINTLN
//�Ĵ�������Ŀ
typedef struct {
	bool inUse;//�üĴ����Ƿ���ʹ��
	string id;//��Ӧ�ı�ʶ����
	string mem;//��Ӧ���ڴ�λ��
	bool dirty;//�����ݲ���Ҫд��
} regPoolItem;
#ifdef ASM_OUTPUT
#define cout asmout
ofstream asmout;
#endif // ASM_OUTPUT
//�������ɵ�Ԫ
class CodeGenerator {
private:
	int curBtabIdx;//�ڷֳ�����еĺ���ָ�룬ָ��ǰ�ǵڼ�������
	int curFuncIdx;//��ǰ�����ڷ��ű��е�λ��
	map<int, int> tempVar;//����ʱ�����ı��ӳ��Ϊ�����ڱ���ĵ�ַ
	map<string, int > func2num;//������ӳ��Ϊ���ֱ��
	vector<regPoolItem> regPool;//�Ĵ�����
	vector<regPoolItem> globalRegPool;//ȫ�ּĴ�����
	vector<FuncBaseBlock*> funcBlocks;//������
	int regPoolPointer;//�Ĵ�������ָ�룬����ʵ��������ת�����Ч��
	FuncBaseBlock* curFuncBlock;//��ǰ������
	int curBBIdx;//��ǰ�����������
	string regV0;//V0�Ĵ�����ǰ���ֵ�����ڱ����ظ���v0�Ĵ�������ͬ��ֵ
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
	//��������Ϊ�ַ���
	static string int2str(int x) {
		stringstream ss;
		ss << x;
		return ss.str();
	}
	//���ɵ���ָ��
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
	//�ڼĴ������в�ѯid��Ӧ�ļĴ�����ʵ���Ͼ����ҵ���������ʱ�Ĵ����Ķ�Ӧ��ϵ
	string findInRegPool(string id) {
		//������Ӧ�ı�ʶ���ڼĴ��������Ƿ��Ѿ�����
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
	//�Ĵ������䣬���Ĵ�����ȫ���ˣ���ѡ��һ������usedReg�еļĴ������
	string regAlloc(string id, set<string> usedReg, bool needload) {
		string mem = getItemAddr(id);//��ȡ�ڴ��ַ
		stringstream ss;
		int i;
		bool found = false;
		if (curFuncBlock->id2Greg.find(id) != curFuncBlock->id2Greg.end() && curFuncBlock->id2Greg[id] != -1) {
			//����Ǳ�����ȫ�ּĴ����ı���
			i = curFuncBlock->id2Greg[id];
			globalRegPool[i].id = id;
			globalRegPool[i].inUse = true;
			globalRegPool[i].mem = mem;
			return "$s" + int2str(i);
		}
		else {//����ֻ�ܽ��ñ���������ʱ�Ĵ���
			for (i = 0; i < regPool.size(); i++) {//���Ĵ������Ƿ��п��üĴ���
				if (regPool[i].inUse == false) {
					found = true;
					break;
				}
			}
			if (!found) {//�޿��üĴ�����ǿ�з��䣬����usedReg�е�Ԫ������$t2,$t3����ʾ��Щ�Ĵ����Ѿ�����
				while (usedReg.find("$t" + int2str(regPoolPointer)) != usedReg.end()) {
					//���regPoolPointerָ��ļĴ�����usedReg�У����ܽ����д���ڴ棬��Ӧ�ò鿴��һ���Ĵ���
					regPoolPointer = (regPoolPointer + 1) % REGPOOL_SIZE;
				}
				//����regPoolPointer��Ӧ�ļĴ��������ֵ���ἴ�����õ�
				i = regPoolPointer;
				if (regPool[i].dirty)
					emit("sw", "$t" + int2str(i), regPool[i].mem, "");//����д���ڴ�
			}
			ss << "$t" << i;
			if (needload) //���id��""����ô�����ֱ�ӷ���һ���Ĵ��������������
				emit("lw", ss.str(), mem, " # load " + ss.str() + " = " + id);
			regPool[i].inUse = true;//�˼Ĵ�����ʹ��
			regPool[i].id = id;
			regPool[i].mem = mem;
			regPool[i].dirty = false;//�˼Ĵ�����ʱδ���ı�
			return ss.str();
		}
	}
	//����ʱ�Ĵ������е����б���ȫ��д���ڴ棬���ո�ȡ���ı�����������д��
	void regPoolSpillAll() {
		//���Ĵ�������գ����мĴ�����������б���д����Ӧλ��
		set<string> tempOutSet = curFuncBlock->innerBlocks[curBBIdx]->outSet;
		for (int i = 0; i < regPool.size(); i++) {
			if (regPool[i].inUse && regPool[i].id != "" && regPool[i].dirty) {
				//ע��ֻ��Ҫд��ȫ�ֱ����ͺ�����ʹ�õ��ı���
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
	//CodeGenerator���ȫ��������������Ӧ��ʶ���ڷ��ű�������
	int cgGlobalSearch(string itemName) {
		symbolTable[0].name = itemName;//�����ڱ�
		int i, curIdx = btab[curBtabIdx].lastItem;//��ȡ��ǰ���ں���������һ�����ű���
		if (curIdx == 0) {
			//�����ǰ������Ϊ0����ô˵����ǰ������û���κ��Ϊ�˴�ǰ�������ѷ��ű��ҵ���Ӧ�ĺ���λ��
			//��Ϊ��ʱ��curBtab��ָʾ����ǰ�ǵڼ�������
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
				if (symbolTable[i].link == 0) curIdx = i - 1;//�����i���link��0��
				//˵����i����Ǳ����һ�����Ҫ��curIdx����Ϊi-1�Է�����һ�����һ���һ����ǵ�ǰ����
				i = symbolTable[i].link;
			}
			if (curIdx == 0 || i != 0) break;//���curIdx=0˵��������ű��0���ʾû�ҵ�����i!=0��ʾ�ҵ��˶�Ӧ�ķ��ű���
		}
		return i;
	}
	//���ݵ����Ĵ��������ֻ�ȡ���ı�ţ��ж��Ƿ���regSet�У����������regMap�����Ҹ���regIdx
	void singleTemp(string& regName, set<int>& regSet, int& regIdx) {
		if (regName.size() > 0 && regName[0] == '#') {
			int tempRegId = getIntValue(false, regName.substr(regPrefix.size()));
			if (regSet.find(tempRegId) == regSet.end()) {//��������в����������ʱ�����ı��
				regSet.insert(tempRegId);
				tempVar[tempRegId] = regIdx++;
			}
		}
	}
	//���㺯���е���ʱ������ռ�ռ䣬���ݵ�ǰ��Ԫʽ�������������ʱ����
	int calTemp(int curQuadIdx) {
		set<int> regSet;
		int regIdx = 0;
		while (curQuadIdx < quadCodeTable.size()) {
			if (quadCodeTable[curQuadIdx].op == "endFunc") break;
			singleTemp(quadCodeTable[curQuadIdx].left, regSet, regIdx);//left,right,result�����ܴ����ʱ�Ĵ���
			singleTemp(quadCodeTable[curQuadIdx].right, regSet, regIdx);
			singleTemp(quadCodeTable[curQuadIdx].result, regSet, regIdx);
			curQuadIdx++;
		}
		return regIdx;
	}
	//�����ַ����������еķ�б�ܽ���ת��
	static string processStr(string str) {
		stringstream ret;
		for (int i = 0; i < str.size(); i++) {
			if (str[i] == '\\') ret << "\\\\";
			else ret << str[i];
		}
		return ret.str();
	}
	//���ñ�ǩ
	static void labelInstr(string label, string comment) {
		cout << label << ":" << " # " << comment << endl;
	}
	//������Ԫʽ�и������������������ڱ����ջ�еĵ�ַ
	string getItemAddr(string& itemName) {
		stringstream ss;
		if (itemName.size() > 0 && itemName[0] == '#') {//��һ����ʱ����
			ss << (-4 * (MEM_SAVE + tempVar[getIntValue(false, itemName.substr(regPrefix.size()))] + btab[curBtabIdx].isize)) << "($fp)";
		}
		else if (itemName == "") {
			return "";
		}
		else {
			int itemIdx = cgGlobalSearch(itemName);
			if (symbolTable[itemIdx].lev == 0) //ȫ��
				ss << (4 * symbolTable[itemIdx].addr) << "($gp)";
			else //�ֲ�
				ss << (-4 * symbolTable[itemIdx].addr) << "($fp)";
		}
		return ss.str();
	}
	//���ݸ�������Ԫʽ������quadOpe����ope������ȷ����ʱ�Ĵ�����ֵ����ʾ�ֵ��˸���ʱ�Ĵ���
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
	//������Ԫʽ��result�򣬷�������Ӧ�ļĴ���
	string fillResult(string& result, set<string> usedReg) {
		string dest;
		string resultReg = findInRegPool(result);
		if (resultReg == "") {//�Ĵ��������޸ñ���������䣬���ǲ���lw
			int destIdx;
			if (curFuncBlock->id2Greg.find(result) != curFuncBlock->id2Greg.end() &&
				curFuncBlock->id2Greg[result] != -1) {
				//����ñ����ǿ��Ա�����ȫ�ּĴ����ı���
				dest = regAlloc(result, usedReg, false);//��ȡ�����Ӧ�ļĴ���
			}
			else {//�ֲ�����
				dest = regAlloc("", usedReg, false);//��ȡ�����Ӧ�ļĴ���
				destIdx = getIntValue(false, dest.substr(2));
				regPool[destIdx].id = result;//�������ú�id��mem
				regPool[destIdx].mem = getItemAddr(result);
			}
		}
		else {//�Ĵ��������иñ�������ֱ��ʹ�øüĴ���
			dest = resultReg;
		}
		if (dest[1] != 's')
			regPool[getIntValue(false, dest.substr(2))].dirty = true;//����λ���б�ǣ���ʾ����ֵ
		return dest;
	}
	//������Ԫʽ������������תָ��
	void branchInstr(int quadIdx) {
		string lop, rop;
		fillOperand(lop, quadCodeTable[quadIdx].left, {});
		fillOperand(rop, quadCodeTable[quadIdx].right, { lop });
		//����ת֮ǰ��ռĴ�����
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
	//����ָ��
	void calcuInstr(int quadIdx) {
		string op = quadCodeTable[quadIdx].op;
		string lop, rop, dest;
		fillOperand(lop, quadCodeTable[quadIdx].left, {});//��ȡ���������Ӧ�Ĵ���
		fillOperand(rop, quadCodeTable[quadIdx].right, { lop });//��ȡ�Ҳ�������Ӧ�Ĵ���
		dest = fillResult(quadCodeTable[quadIdx].result, { lop,rop });//��ȡ�����ʶ����Ӧ�ļĴ���
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
	//����������ǩ
	static string funcLabel(int funcId) {
		stringstream ss;
		ss << "Func_" << funcId;
		return ss.str();
	}
	//����MIPSָ�����
	void genOptMips() {
		int i, funcNum = 0;
		//����д������û��һ����Ԫʽ��������mips����
		if (errCnt != 0 || quadCodeTable.size() == 0) return;
		cout << "############################## Global String ##############################" << endl;
		cout << ".data" << endl;
		for (i = 0; i < stringTable.size(); i++) //�ַ�������
			cout << "_str" << i << " : " << " .asciiz " << processStr(stringTable[i].val) << endl;
		i = 0;
		cout << "##############################  Text Segment ##############################" << endl;
		cout << ".text" << endl;
		//Ϊ������ֵ������ָ��gp�䵱����ַ
		cout << "# initialize global constant" << endl;
		cout << "\t" << "addu $gp,$sp,$0 # let $gp=$sp to get more space" << endl;
		cout << "\t" << "subi $gp,$gp," << (btab[0].isize * 4) << " # make space for global variabel and constant" << endl;
		cout << "# initialize global constant" << endl;
		while (true) {//Ϊȫ�ֳ�������ʼֵ
			if (quadCodeTable[i].op == "const") {
				cout << "\t" << "li $t0," << quadCodeTable[i].result << " # " << quadCodeTable[i].right << "=" << quadCodeTable[i].result << endl;
				//����ȫ�ֳ���������һ���ڳ����ǰ�棬�������ǵĵ�ַ��������i����4
				cout << "\t" << "sw $t0," << (i * 4) << "($gp)" << endl;
			}
			else break;
			i++;
		}
		//Ϊ�������� ������-���� ��ӳ���ϵ��Ŀ���ǽ����к�����ӳ��Ϊ'Func_n'����ʽ
		for (int j = 0; j < quadCodeTable.size(); j++) {
			if (quadCodeTable[j].right == "()")  //����
				func2num[quadCodeTable[j].left] = funcNum++;//���º�������
		}
		cout << "\t" << "addu $sp,$gp,$0 # update $sp" << endl;
		cout << "\t" << "j " << funcLabel(func2num["main"]) << " # jump to main function" << endl;//��ת����������ʼִ��
		while (i < quadCodeTable.size() && !isQuadFunc(quadCodeTable[i])) //��ת����������
			i++;
		while (i < quadCodeTable.size()) {
			bool spilled = false;
			if (isQuadFunc(quadCodeTable[i])) {//����������ע�����Ƕ���ȫ�ֵ�
				tempVar.clear();//�����ʱ������������ӳ���ϵ
				string func_name = quadCodeTable[i].left;//������
				curFuncBlock = funcBlocks[curBtabIdx];//��ǰ����������
				curBBIdx = 0;//��ǰ�������ں������ڵ�����Ϊ0
				curBtabIdx++;//����������һ
				curFuncIdx = cgGlobalSearch(func_name);//�ҵ��˺�����Ӧ�ķ��ű�������
				for (int k = 0; k < globalRegPool.size(); k++)
					globalRegPool[k] = { false,"","",false };
				labelInstr(funcLabel(func2num[func_name]), "function " + func_name);//���������±�
				//$sp�½�������Ϊ�����ռ�����λ�ã���ν�����ռ䣬��ָ$ra��$fp���ں������õĹ�����Ҫ���������
				emit("subi", "$sp", "$sp", int2str(MEM_SAVE * 4), " # make space for $fp and $ra");
				//Լ���ɵ�fp���µ�fp��ָ���λ�ã����ɵ�ra�ھɵ�fp����
				emit("sw", "$fp", int2str((MEM_SAVE - 1) * 4) + "($sp)", " # save $fp");//����fpָ��
				emit("sw", "$ra", int2str((MEM_SAVE - 2) * 4) + "($sp)", " # save $ra");//���溯�����ص�ַ
				emit("addi", "$fp", "$sp", ((MEM_SAVE - 1) * 4 + symbolTable[curFuncIdx].dimen * 4), " # $fp = old $sp");//����fp�䵱��λ��base
				if (btab[curBtabIdx].isize * 4 + calTemp(i) * 4 - btab[curBtabIdx].psize * 4 != 0)
					emit("subi", "$sp", "$sp", (btab[curBtabIdx].isize * 4 + calTemp(i) * 4 - btab[curBtabIdx].psize * 4),
						" # make space for locals and temps");//Ϊ�ֲ������Լ���ʱ�������ռ�
			}
			else if (quadCodeTable[i].op == "endFunc") {//��������	exit_Func_1 # exit function foo
				regPoolSpillAll();
				spilled = true;
				labelInstr("exit_" + funcLabel(func2num[quadCodeTable[i].left]), "exit function " + quadCodeTable[i].left);
				if ((btab[curBtabIdx].isize - btab[curBtabIdx].psize + tempVar.size()) * 4 != 0)
					emit("addi", "$sp", "$sp", ((btab[curBtabIdx].isize - btab[curBtabIdx].psize + tempVar.size()) * 4), " # pop locals and temps");//��ջ
				emit("lw", "$fp", int2str((MEM_SAVE - 1) * 4) + "($sp)", "");
				emit("lw", "$ra", int2str((MEM_SAVE - 2) * 4) + "($sp)", "");
				emit("addi", "$sp", "$sp", ((symbolTable[curFuncIdx].dimen + MEM_SAVE) * 4), "");//�ָ�������
				if (quadCodeTable[i].left != "main") //�����������ŷ���
					emit("jr", "$ra");
				else {
					emit("addi", "$sp", "$sp", (btab[0].isize * 4), "");//���������践�أ�ֱ�ӻָ�spָ�룬���Ǹղű�gp�½�ռ�õĿռ�
					emit("li", "$gp", "0x10008000", " # restore $gp");//�ָ�gpΪĬ��ֵ
				}
				//�����Ż������в����˶������Ԫʽ��ͨ��������뽫���Թ�
				while (i + 1 < quadCodeTable.size() && !isQuadFunc(quadCodeTable[i + 1]))//������һ���������ֻ���������Ԫʽ���������
					i++;
			}
			else if (quadCodeTable[i].op == "const") {//�ֲ���������
				int itemAddr = cgGlobalSearch(quadCodeTable[i].right);
				BaseBlock* tempBB = curFuncBlock->innerBlocks[curBBIdx];
				//���������out����use�������棬����Ҫ������и�ֵ
				if (tempBB->outSet.find(quadCodeTable[i].right) != tempBB->outSet.end() ||
					tempBB->useSet.find(quadCodeTable[i].right) != tempBB->useSet.end()) {
					emit("li", "$t0", int2str(symbolTable[itemAddr].value), " # const " + quadCodeTable[i].right + "=" + quadCodeTable[i].result);
					// �����symbolTable[itemAddr].addr���Ǽ�����MEM_SAVE֮��ģ����������ټ���
					emit("sw", "$t0", int2str(-(symbolTable[itemAddr].addr) * 4) + "($fp)", "");
				}
			}
			else if (quadCodeTable[i].op == "push") {//������ѹ��ջ��
				string _tn;
				fillOperand(_tn, quadCodeTable[i].left, {});
				emit("subi", "$sp", "$sp", "4", "");//ѹջ
				emit("sw", _tn, "0($sp)", "");//�������
			}
			else if (quadCodeTable[i].op == "call") {//�������ã�ʵ���Ͼ���һ����ת
				regPoolSpillAll();//ȫ��д��
				spilled = true;
				for (int k = 0; k < globalRegPool.size(); k++) {
					if (globalRegPool[k].inUse && curFuncBlock->innerBlocks[curBBIdx]->outSet.find(globalRegPool[k].id) !=
						curFuncBlock->innerBlocks[curBBIdx]->outSet.end())
						emit("sw", "$s" + int2str(k), globalRegPool[k].mem, " # save " + globalRegPool[k].id + "(global reg)");//�������
				}
				emit("jal", funcLabel(func2num[quadCodeTable[i].left]));
				for (int k = 0; k < globalRegPool.size(); k++) {
					if (globalRegPool[k].inUse && curFuncBlock->innerBlocks[curBBIdx]->outSet.find(globalRegPool[k].id) !=
						curFuncBlock->innerBlocks[curBBIdx]->outSet.end())
						emit("lw", "$s" + int2str(k), globalRegPool[k].mem, " # save " + globalRegPool[k].id + "(global reg)");//�������
				}
			}
			else if (isQuadComp(quadCodeTable[i])) {//��ϵ�����
				branchInstr(i);
			}
			else if (isQuadLabel(quadCodeTable[i])) {//Label����ָ��
				cout << quadCodeTable[i].op << endl;//ֱ�������ż���
			}
			else if (quadCodeTable[i].op == "GOTO") {//GOTO����ָ��
				regPoolSpillAll();
				spilled = true;
				emit("j", quadCodeTable[i].left);
			}
			else if (quadCodeTable[i].op == "ret") {
				if (quadCodeTable[i].left != "") { //ֻ�е���������Ϊ0��ʱ��ż��������������void�����ķ������
					//Լ����$v1����ֵ
					if (isConstant(quadCodeTable[i].left)) {
						emit("li", "$v1", quadCodeTable[i].left, " # @RET = " + quadCodeTable[i].left);
					}
					else if (quadCodeTable[i].left != "@RET") {//�����Ԫʽ��left����@RET����ô��Ҫ��$v1���и�ֵ��������Ҫ
						if (findInRegPool(quadCodeTable[i].left) == "")
							emit("lw", "$v1", getItemAddr(quadCodeTable[i].left), " # @RET = " + quadCodeTable[i].left);
						else
							emit("addu", "$v1", "$0", findInRegPool(quadCodeTable[i].left), " # @RET = " + quadCodeTable[i].left);
					}
				}
				regPoolSpillAll();//��ռĴ�����
				spilled = true;
				emit("j", "exit_" + funcLabel(func2num[symbolTable[curFuncIdx].name]));//�뿪����
			}
			else if (isQuadCalc(quadCodeTable[i])) {//������ָ��
				calcuInstr(i);
			}
			else if (quadCodeTable[i].op == "=") {//�򵥸�ֵ
				string lop, dest;
				if (isConstant(quadCodeTable[i].left)) {
					dest = fillResult(quadCodeTable[i].result, {});
					emit("li", dest, quadCodeTable[i].left, " # " + dest + "=" + quadCodeTable[i].left);
				}
				else if (quadCodeTable[i].left == "@RET") {
					dest = fillResult(quadCodeTable[i].result, {});
					emit("addu", dest, "$0", "$v1", " # " + dest + "=" + quadCodeTable[i].left);//��ɸ�ֵ
				}
				else {
					fillOperand(lop, quadCodeTable[i].left, {});//��ȡ��������Ĵ���
					dest = fillResult(quadCodeTable[i].result, { lop });//��ȡ�����ʶ����Ӧ�Ĵ���
					emit("addu", dest, "$0", lop, " # " + dest + "=" + quadCodeTable[i].left);//��ɸ�ֵ
				}
			}
			else if (quadCodeTable[i].op == "=[]") {//��������ȡֵ
				string lop, rop, dest;//����������
				string arrIdx;//�����±�
				fillOperand(rop, quadCodeTable[i].right, {});
				arrIdx = regAlloc("", { rop }, false);
				emit("sll", arrIdx, rop, "2", "");//����4�õ�ƫ����
				lop = regAlloc("", { arrIdx,rop }, false);//������������dirty��id��Ҳʼ����""�����Բ��ᱻ��д��Ҳ���ᱻ��������
				int itemIdx = cgGlobalSearch(quadCodeTable[i].left);
				//��ȡ����ĵ�ַ
				if (symbolTable[itemIdx].lev == 0) { //ȫ��
					emit("addiu", lop, "$gp", (4 * symbolTable[itemIdx].addr), "");
					emit("addu", lop, lop, arrIdx, "");
				}
				else { //�ֲ�
					emit("subi", lop, "$fp", (4 * symbolTable[itemIdx].addr), "");
					emit("subu", lop, lop, arrIdx, "");
				}
				//���ҽ�������Ƿ��ڼĴ����д���
				dest = fillResult(quadCodeTable[i].result, { lop,rop,arrIdx });
				//����lop���Ԫ�ص�ַ
				emit("lw", dest, "0(" + lop + ")",
					" # " + quadCodeTable[i].result + "=" + quadCodeTable[i].left + "[" + quadCodeTable[i].right + "]");
			}
			else if (quadCodeTable[i].op == "[]=") {//��ֵ������Ԫ��,[]=,#reg0,#reg1,a		a[#reg0] = #reg1
				string lop, rop, dest;//���Ҳ�������Ŀ���
				string arrIdx;//�����±�
				fillOperand(lop, quadCodeTable[i].left, {});//��ȡ�±����ڵļĴ���
				arrIdx = regAlloc("", { lop }, false);//�����µļĴ�������±�*4��ֵ
				emit("sll", arrIdx, lop, "2", "");//�õ������±�
				dest = regAlloc("", { lop,arrIdx }, false);//��ȡĿ���ַ
				int itemIdx = cgGlobalSearch(quadCodeTable[i].result);
				if (symbolTable[itemIdx].lev == 0) { //ȫ��
					emit("addiu", dest, "$gp", (4 * symbolTable[itemIdx].addr), "");
					emit("addu", dest, dest, arrIdx, "");
				}
				else { //�ֲ�
					emit("subi", dest, "$fp", (4 * symbolTable[itemIdx].addr), "");
					emit("subu", dest, dest, arrIdx, "");
				}
				fillOperand(rop, quadCodeTable[i].right, { lop,dest,arrIdx });//��ȡд�������ֵ
				emit("sw", rop, "0(" + dest + ")",
					" # " + quadCodeTable[i].result + "[" + quadCodeTable[i].left + "] = " + quadCodeTable[i].right);//�����д�������Ӧλ��
				regPoolSpillAll();//��ռĴ���
				spilled = true;
			}
			else if (quadCodeTable[i].op == "scanfInt" || quadCodeTable[i].op == "scanfChr") {//�������������ַ�
				string dest;
				regPoolSpillAll();//��ռĴ���
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
				if (curFuncBlock->id2Greg.find(quadCodeTable[i].left) != curFuncBlock->id2Greg.end()) {//����Ƿ���ȫ�ּĴ����ı���
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
				if (isConstant(quadCodeTable[i].left)) {  //������ֳ������
					emit("li", "$a0", quadCodeTable[i].left, "");
				}
				else if (quadCodeTable[i].left == "@RET") {//���@RET�����
					emit("addu", "$a0", "$0", "$v1", "");
				}
				else {//��ԼĴ�����ֵ���
					if (findInRegPool(quadCodeTable[i].left) == "") //�Ĵ�������û�У����ò������ڴ�
						emit("lw", "$a0", getItemAddr(quadCodeTable[i].left), "");
					else //�Ĵ��������Ѿ��У�ֱ��ʹ��
						emit("addu", "$a0", "$0", findInRegPool(quadCodeTable[i].left), "");
				}
				emit("syscall", "");
			}
			else if (quadCodeTable[i].op == "printStr") {//����ַ���
				emit("li", "$v0", "4", " # print string");
				regV0 = "4";
				emit("la", "$a0", "_" + quadCodeTable[i].left.substr(1), "");
				emit("syscall", "");
			}
#ifdef PRINTLN
			else if (quadCodeTable[i].op == "println") {//�������
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
				//�����һ����Ԫʽ�ǻ������ͷ�������Ҵ˻�����û�н��й��Ĵ����ص���գ���ô��Ҫ��գ�
				//����ᷢ����������ļĴ����ص���һ�����������յ����
				regPoolSpillAll();
			}
			i++;
		}
	}
};

class NaiveGenerator {//�����Ż��Ĵ���������
private:
	int curBtab;//�ڷֳ�����еĺ���ָ�룬ָ��ǰ�ǵڼ�������
	int curFunc;//��ǰ�����ڷ��ű��е�λ��
	map<int, int> tempVar;//����ʱ�����ı��ӳ��Ϊ�����ڱ���ĵ�ַ
	map<string, int > funcIdx;
	int funcNum;
	int regPoolPointer;//�Ĵ�������ָ��
public:
	NaiveGenerator() {
		curBtab = 0;
		curFunc = 0;
		funcNum = 0;
		regPoolPointer = 0;
	}
	//CodeGenerator���ȫ��������������Ӧ��ʶ���ڷ��ű�������
	int cgGlobalSearch(string itemName) {
		symbolTable[0].name = itemName;//�����ڱ�
		int i, curIdx = btab[curBtab].lastItem;//��ȡ��ǰ���ں���������һ�����ű���
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
				if (symbolTable[i].link == 0) curIdx = i - 1;//�����i���link��0��
															 //˵����i����Ǳ����һ�����Ҫ��curIdx����Ϊi-1�Է�����һ�����һ���һ����ǵ�ǰ����
				i = symbolTable[i].link;
			}
			if (curIdx == 0 || i != 0) break;//���curIdx=0˵��������ű��0���ʾû�ҵ�����i!=0��ʾ�ҵ��˶�Ӧ�ķ��ű���
		}
		return i;
	}
	//���ݵ����Ĵ��������ֻ�ȡ���ı�ţ��ж��Ƿ���regSet�У����������regMap�����Ҹ���regIdx
	void singleTemp(string& regName, set<int>& regSet, int& regIdx) {
		if (regName.size() > 0 && regName[0] == '#') {
			int tempRegId = getIntValue(false, regName.substr(regPrefix.size()));
			if (regSet.find(tempRegId) == regSet.end()) {//��������в����������ʱ�����ı��
				regSet.insert(tempRegId);
				tempVar[tempRegId] = regIdx++;
			}
		}
	}
	//������ʱ������ռ�ռ䣬���ݵ�ǰ��Ԫʽ�������������ʱ����
	int calTemp(int curQuadIdx) {
		set<int> regSet;
		int regIdx = 0;
		while (curQuadIdx < quadCodeTable.size()) {
			if (quadCodeTable[curQuadIdx].op == "endFunc") break;
			singleTemp(quadCodeTable[curQuadIdx].left, regSet, regIdx);//left,right,result�����ܴ����ʱ�Ĵ���
			singleTemp(quadCodeTable[curQuadIdx].right, regSet, regIdx);
			singleTemp(quadCodeTable[curQuadIdx].result, regSet, regIdx);
			curQuadIdx++;
		}
		return regIdx;
	}
	//������Ԫʽ�и������������������ڱ����ջ�еĵ�ַ
	string getItemAddr(string& itemName) {
		stringstream ss;
		if (itemName.size() > 0 && itemName[0] == '#') {//��һ����ʱ����
			ss << (-4 * (MEM_SAVE + tempVar[getIntValue(false, itemName.substr(regPrefix.size()))] + btab[curBtab].isize)) << "($fp)";
		}
		else if (itemName == "") {
			return "";
		}
		else {
			int itemIdx = cgGlobalSearch(itemName);
			if (symbolTable[itemIdx].lev == 0) //ȫ��
				ss << (4 * symbolTable[itemIdx].addr) << "($gp)";
			else //�ֲ�
				ss << (-4 * symbolTable[itemIdx].addr) << "($fp)";
		}
		return ss.str();
	}
	//������Ԫʽ������������תָ��
	void branchInstr(int quadIdx) {
		if (isConstant(quadCodeTable[quadIdx].left))//�������
			CodeGenerator::emit("li", "$t0", quadCodeTable[quadIdx].left, "");
		else if (quadCodeTable[quadIdx].left == "@RET")//RET���
			CodeGenerator::emit("addu", "$t0", "$0", "$v1", "");
		else
			CodeGenerator::emit("lw", "$t0", getItemAddr(quadCodeTable[quadIdx].left), "");//�������
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
	//����ָ��
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
	//����MIPSָ�����
	void generateMips() {
		int i;
		//����д������û��һ����Ԫʽ��������mips����
		if (errCnt != 0 || quadCodeTable.size() == 0) return;
		cout << "############################## Global String ##############################" << endl;
		cout << ".data" << endl;
		for (i = 0; i < stringTable.size(); i++) //�ַ�������
			cout << "_str" << i << " : " << " .asciiz " << CodeGenerator::processStr(stringTable[i].val) << endl;
		i = 0;
		cout << "##############################  Text Segment ##############################" << endl;
		cout << ".text" << endl;
		//Ϊ������ֵ������ָ��gp�䵱����ַ
		cout << "# initialize global constant" << endl;
		cout << "addu $gp,$sp,$0 # let $gp=$sp to get more space" << endl;
		cout << "subu $gp,$gp," << (btab[0].isize * 4) << " # make space for global variabel and constant" << endl;
		while (true) {//Ϊȫ�ֳ�������ʼֵ
			if (quadCodeTable[i].op == "const") {
				cout << "li $t0," << quadCodeTable[i].result << " # " 
					<< quadCodeTable[i].right << "=" << quadCodeTable[i].result << endl;
				cout << "sw $t0," << (i * 4) << "($gp)" << endl;
			}
			else break;
			i++;
		}
		for (int i = 0; i < quadCodeTable.size(); i++) {
			if (quadCodeTable[i].right == "()")  //����
				funcIdx[quadCodeTable[i].left] = funcNum++;//���º�������
		}
		cout << "addu $sp,$gp,$0 # update $sp" << endl;
		cout << "j " << CodeGenerator::funcLabel(funcIdx["main"]) << " # jump to main function" << endl;//��ת����������ʼִ��
		while (i < quadCodeTable.size() && !isQuadFunc(quadCodeTable[i])) //��ת����������
			i++;
		while (i < quadCodeTable.size()) {
			if (isQuadFunc(quadCodeTable[i])) {//����������ע�����Ƕ���ȫ�ֵ�
				tempVar.clear();//�����ʱ������������ӳ���ϵ
				string func_name = quadCodeTable[i].left;//������
				curBtab++;//����������һ
				curFunc = cgGlobalSearch(func_name);//�ҵ��˺�����Ӧ�ķ��ű�������
				cout << endl;
				CodeGenerator::labelInstr(CodeGenerator::funcLabel(funcIdx[func_name]), "function " + func_name);//���������±�
				CodeGenerator::emit("subu", "$sp", "$sp", "8", " # make space for $fp and $ra");
				CodeGenerator::emit("sw", "$fp", "4($sp)", " # save $fp");//����fpָ��
				CodeGenerator::emit("sw", "$ra", "0($sp)", " # save $ra");//���溯�����ص�ַ
														   //��һ�е�һ������4��Ϊ��Ѱַ���㣬ƫ����1λ
				CodeGenerator::emit("addu", "$fp", "$sp", (4 + symbolTable[curFunc].dimen * 4), " # fp = new base");//����fp�䵱��λ��base
				CodeGenerator::emit("subu", "$sp", "$sp", (btab[curBtab].isize * 4 + calTemp(i) * 4 - btab[curBtab].psize * 4),
					" # make space for locals and temps");;//Ϊ�ֲ������Լ���ʱ�������ռ�
			}
			else if (quadCodeTable[i].op == "endFunc") {//��������
														//exit_Func_1 # exit function foo
				CodeGenerator::labelInstr("exit_" + CodeGenerator::funcLabel(funcIdx[quadCodeTable[i].left]), "exit function " + quadCodeTable[i].left);
				CodeGenerator::emit("addu", "$sp", "$sp", ((btab[curBtab].isize - btab[curBtab].psize + tempVar.size()) * 4), " # pop locals and temps");//��ջ
				CodeGenerator::emit("lw", "$fp", "4($sp)", "");
				CodeGenerator::emit("lw", "$ra", "0($sp)", "");
				CodeGenerator::emit("addu", "$sp", "$sp", ((btab[curBtab].psize + MEM_SAVE) * 4), "");;//�ָ�������
				if (quadCodeTable[i].left != "main") //�����������ŷ���
					CodeGenerator::emit("jr", "$ra");
				else {
					CodeGenerator::emit("addu", "$sp", "$sp", (btab[0].isize * 4), "");//���������践�أ�ֱ�ӻָ�spָ�룬���Ǹղű�gp�½�ռ�õĿռ�
					CodeGenerator::emit("li", "$gp", "0x10008000", " # restore $gp");//�ָ�gpΪĬ��ֵ
				}
				//�����Ż������в����˶������Ԫʽ��ͨ��������뽫���Թ�
				while (i + 1 < quadCodeTable.size() && !isQuadFunc(quadCodeTable[i + 1]))//������һ���������ֻ���������Ԫʽ���������
					i++;
			}
			else if (quadCodeTable[i].op == "const") {//�ֲ���������
				int itemAddr = cgGlobalSearch(quadCodeTable[i].right);
				CodeGenerator::emit("li", "$t0", CodeGenerator::int2str(symbolTable[itemAddr].value), " # " + quadCodeTable[i].right + "=" + quadCodeTable[i].result);
				CodeGenerator::emit("sw", "$t0", CodeGenerator::int2str(-(symbolTable[itemAddr].addr) * 4) + "($fp)", "");
			}
			else if (quadCodeTable[i].op == "push") {//������ѹ��ջ��
				if (isConstant(quadCodeTable[i].left))
					CodeGenerator::emit("li", "$t0", quadCodeTable[i].left, " # push " + quadCodeTable[i].left);
				else if (quadCodeTable[i].left == "@RET")
					CodeGenerator::emit("addu", "$t0", "$0", "$v1", "");//RET���
				else
					CodeGenerator::emit("lw", "$t0", getItemAddr(quadCodeTable[i].left), " # push " + quadCodeTable[i].left);
				CodeGenerator::emit("subu", "$sp", "$sp", "4", "");
				CodeGenerator::emit("sw", "$t0", "0($sp)", "");
			}
			else if (quadCodeTable[i].op == "call") {//�������ã�ʵ���Ͼ���һ����ת
				CodeGenerator::emit("jal", CodeGenerator::funcLabel(funcIdx[quadCodeTable[i].left]));
				CodeGenerator::emit("addu", "$v1", "$0", "$v0", " # save the return value");
			}
			else if (isQuadComp(quadCodeTable[i])) {//��ϵ�����
				branchInstr(i);
			}
			else if (isQuadLabel(quadCodeTable[i])) {//Label����ָ��
				cout << quadCodeTable[i].op << endl;//ֱ�������ż���
			}
			else if (quadCodeTable[i].op == "GOTO") {//GOTO����ָ��
				CodeGenerator::emit("j", quadCodeTable[i].left);
			}
			else if (quadCodeTable[i].op == "ret") {
				if (quadCodeTable[i].left != "") { //ֻ�е���������Ϊ0��ʱ��ż��������������void�����ķ������
					if (isConstant(quadCodeTable[i].left))
						CodeGenerator::emit("li", "$v0", quadCodeTable[i].left, " # return" + quadCodeTable[i].left);
					else if (quadCodeTable[i].left == "@RET")
						CodeGenerator::emit("addu", "$v0", "$0", "$v1", "");//RET���
					else
						CodeGenerator::emit("lw", "$v0", getItemAddr(quadCodeTable[i].left), "");
				}
				CodeGenerator::emit("j", "exit_" + CodeGenerator::funcLabel(funcIdx[symbolTable[curFunc].name]));//�뿪����
			}
			else if (isQuadCalc(quadCodeTable[i])) {//������ָ��
				calcuInstr(i);
			}
			else if (quadCodeTable[i].op == "=") {//�򵥸�ֵ
				if (quadCodeTable[i].left == "@RET") {//���ڷ���ֵ
					CodeGenerator::emit("sw", "$v1", getItemAddr(quadCodeTable[i].result), " # " + quadCodeTable[i].result + "=@RET");
				}
				else if (isConstant(quadCodeTable[i].left)) { //���ڳ���
					CodeGenerator::emit("li", "$t0", quadCodeTable[i].left, " # " + quadCodeTable[i].result + "=" + quadCodeTable[i].left);
					CodeGenerator::emit("sw", "$t0", getItemAddr(quadCodeTable[i].result), "");
				}
				else {//�����ǵ���һ����ʶ��������ʱ����
					CodeGenerator::emit("lw", "$t0", getItemAddr(quadCodeTable[i].left),
						" # " + quadCodeTable[i].result + "=" + quadCodeTable[i].left);
					CodeGenerator::emit("sw", "$t0", getItemAddr(quadCodeTable[i].result), "");
				}
			}
			else if (quadCodeTable[i].op == "=[]") {//��������ȡֵ
				string arrIdx = quadCodeTable[i].right;
				if (isConstant(arrIdx)) //cout << "li $t0," << arrIdx << endl;//��ȡ�����±�
					CodeGenerator::emit("li", "$t0", arrIdx, "");
				else if (arrIdx == "@RET")
					CodeGenerator::emit("addu", "$t0", "$0", "$v1", "");
				else //cout << "lw $t0," << getItemAddr(arrIdx) << endl;
					CodeGenerator::emit("lw", "$t0", getItemAddr(arrIdx), "");
				CodeGenerator::emit("sll", "$t0", "$t0", "2", "");
				int itemIdx = cgGlobalSearch(quadCodeTable[i].left);
				if (symbolTable[itemIdx].lev == 0) { //ȫ��
					CodeGenerator::emit("addu", "$t1", "$gp", (4 * symbolTable[itemIdx].addr), "");
					CodeGenerator::emit("addu", "$t1", "$t1", "$t0", "");
				}
				else { //�ֲ�
					CodeGenerator::emit("subu", "$t1", "$fp", (4 * symbolTable[itemIdx].addr), "");
					CodeGenerator::emit("subu", "$t1", "$t1", "$t0", "");
				}
				//����$t1���Ԫ�ص�ַ
				CodeGenerator::emit("lw", "$t0", "0($t1)", "");
				CodeGenerator::emit("sw", "$t0", getItemAddr(quadCodeTable[i].result), "");
			}
			else if (quadCodeTable[i].op == "[]=") {//��ֵ������Ԫ��,[]=,#reg0,#reg1,a		a[#reg0] = #reg1
				string arrIdx = quadCodeTable[i].left;
				if (isConstant(arrIdx))
					CodeGenerator::emit("li", "$t0", arrIdx, "");//��ȡ�����±�
				else if (arrIdx == "@RET")
					CodeGenerator::emit("addu", "$t0", "$0", "$v1", "");
				else
					CodeGenerator::emit("lw", "$t0", getItemAddr(arrIdx), "");
				CodeGenerator::emit("sll", "$t0", "$t0", "2", "");
				int itemIdx = cgGlobalSearch(quadCodeTable[i].result);
				if (symbolTable[itemIdx].lev == 0) { //ȫ��
					CodeGenerator::emit("addu", "$t1", "$gp", (4 * symbolTable[itemIdx].addr), "");
					CodeGenerator::emit("addu", "$t1", "$t1", "$t0", "");
				}
				else { //�ֲ�
					CodeGenerator::emit("subu", "$t1", "$fp", (4 * symbolTable[itemIdx].addr), "");
					CodeGenerator::emit("subu", "$t1", "$t1", "$t0", "");
				}
				if (isConstant(quadCodeTable[i].right))
					CodeGenerator::emit("li", "$t0", quadCodeTable[i].right, "");//��ȡ��Ӧ��ֵ
				else if (quadCodeTable[i].right == "@RET")
					CodeGenerator::emit("addu", "$t0", "$0", "$v1", "");
				else
					CodeGenerator::emit("lw", "$t0", getItemAddr(quadCodeTable[i].right), "");
				CodeGenerator::emit("sw", "$t0", "0($t1)", "");//�����д�������Ӧλ��
			}
			else if (quadCodeTable[i].op == "scanfInt" || quadCodeTable[i].op == "scanfChr") {//�����������ͱ�ʶ��
				if (quadCodeTable[i].op == "scanfInt") CodeGenerator::emit("li", "$v0", "5", "# read int");
				else CodeGenerator::emit("li", "$v0", "12", "# read char");
				CodeGenerator::emit("syscall", "");
				CodeGenerator::emit("sw", "$v0", getItemAddr(quadCodeTable[i].left), " # scanfChr " + quadCodeTable[i].left);
			}
			else if (quadCodeTable[i].op == "printInt" || quadCodeTable[i].op == "printChr") {
				if (quadCodeTable[i].op == "printInt") CodeGenerator::emit("li", "$v0", "1", "# print int " + quadCodeTable[i].left);
				else CodeGenerator::emit("li", "$v0", "11", " # print char : " + quadCodeTable[i].left);
				if (isConstant(quadCodeTable[i].left))   //������ֳ������
					CodeGenerator::emit("li", "$a0", quadCodeTable[i].left, "");
				else if (quadCodeTable[i].left == "@RET") //Ҫ�������v0�е�ֵ
					CodeGenerator::emit("addu", "$a0", "$0", "$v1", "");//v0���ܱ�֮ǰ�Ĵ����޸ģ�����ѡ��v1��v1һֱ���ָղŵ��ú����Ľ��
				else //��ԼĴ�����ֵ���
					CodeGenerator::emit("lw", "$a0", getItemAddr(quadCodeTable[i].left), "");
				CodeGenerator::emit("syscall", "");
			}
			else if (quadCodeTable[i].op == "printStr") {//����ַ���
				CodeGenerator::emit("li", "$v0", "4", " # print string");
				CodeGenerator::emit("la", "$a0", "_" + quadCodeTable[i].left.substr(1), "");
				CodeGenerator::emit("syscall", "");
			}
#ifdef PRINTLN
			else if (quadCodeTable[i].op == "println") {//�������
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