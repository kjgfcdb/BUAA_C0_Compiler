#ifndef SEMANTIC_H
#define SEMANTIC_H
#include "syntax.h"
typedef struct {
	string op;
	string left;
	string right;
	string result;
} quadCode;
//四元式表
vector<quadCode> quadCodeTable;
string operand;//表达式处理中要用到的操作数
string leftOperand;//表达式处理中要用到的左操作数
string rightOperand;//表达式处理中要用到的右操作数
bool isExprChar;//表达式结果是否为char类型
bool isExprStatic;//判断表达式是否为静态，静态意味着可以求出其值
int exprVal;//表达式下标的值
string regPrefix = "#reg";//临时变量的前缀
string labelPrefix = "Label_";//标签的前缀
//判断此四元式是否为运算类四元式
bool isQuadCalc(quadCode& qc) {
	if (qc.op == "+" || qc.op == "-" || qc.op == "*" || qc.op == "/")
		return true;
	return false;
}
//判断该四元式是否为比较四元式
bool isQuadComp(quadCode& qc) {
	if (qc.op == "==" || qc.op == "!=" || qc.op == "<=" || 
		qc.op == ">=" || qc.op == "<" || qc.op == ">")
		return true;
	return false;
}
//判断该四元式是否为标签
bool isQuadLabel(quadCode& qc) {
	if (qc.op.size() > labelPrefix.size() &&
		qc.op.substr(0, labelPrefix.size()) == labelPrefix)
		return true;
	return false;
}
//判断该四元式是否为函数
bool isQuadFunc(quadCode& qc) {
	return qc.right == "()";
}
//判断字符串ic是否代表常量，也就是123或者-1或者'a'
bool isConstant(string& ic) {
	if (ic.size() > 0 && (isdigit(ic[0]) || ic[0] == '+' ||
		ic[0] == '-' || ic[0] == '\'')) {
		return true;
	}
	return false;
}
//判断字符串it是为临时变量字符串
bool isTemp(string& it) {
	if (it.size() > 0 && it[0] == '#')
		return true;
	return false;
}
//产生寄存器名
string buildRegName(int regId) {
	stringstream ss;
	ss << regPrefix << regId;
	return ss.str();
}
int regUsed;//被使用的寄存器数量
//生成标号
int genLabel() {
	static int labelCnt = 0;
	return labelCnt++;
}
//分配寄存器
int allocReg() {//直接让寄存器号加一
	return regUsed++;
}
//在当前符号表中查找标识符
int localSearch(string name) {
	int j;
	symbolTable[0].name = name; //建立哨兵
	j = btab[display[curlev]].lastItem;//获取当前层最后一个符号表项位置
	while (symbolTable[j].name != name)//不断搜索本层
		j = symbolTable[j].link;
	return j;//返回索引位置
}
//在全局符号表中查找标识符
int globalSearch(string name) {
	int i = curlev, j;
	symbolTable[0].name = name; //建立哨兵
	while (true) {
		j = btab[display[i]].lastItem;//获取当前层最后一个符号表项位置
		while (symbolTable[j].name != name)//不断搜索本层
			j = symbolTable[j].link;
		i--;
		if (i < 0 || j != 0) break;//如果搜索完所有符号表项或者找到了相应标识符
	}
	return j;//返回索引位置
}
//生成四元式
void genQuadCode(string op, string src1, string src2, string result) {
	quadCodeTable.push_back({ op,src1,src2,result });
}
//登记符号表
void enter_symtab(string name, types t, objType objt, int value, int addr) {
	symbolTableItem temp;
	temp.name = name;
	temp.type = t;
	temp.objTyp = objt;
	temp.value = value;
	temp.link = btab[display[curlev]].lastItem;
	temp.lev = curlev;
	temp.ref = 0;
	symbolTable.push_back(temp);
	btab[display[curlev]].lastItem = symbolTable.size() - 1;
	if (objt == parameter) {
		//上一层的最后一个符号表项就是当前层的函数，这里给它的dimen加一，表示参数个数加一
		symbolTable[btab[display[curlev - 1]].lastItem].dimen += 1;
		//如果是参数，那么还需要修改参数指针
		btab[display[curlev]].lastPar = symbolTable.size() - 1;
		btab[display[curlev]].psize += 1;
		btab[display[curlev]].isize += 1;
	}
	else if (objt == constant || objt == variable) { //占用空间加一
		btab[display[curlev]].isize += 1;
	}
	else if (objt == arrayTyp) {
		//如果是数组类型，那么应该将分程序表的isize增加数组的大小
		btab[display[curlev]].isize += value;//这里的value是数组的维数
	}
}
//将字符串插入字符串表中
void str_insert(string str) {
	stringTableItem temp;
	temp.val = str;
	stringTable.push_back(temp);
}
//将常量插入符号表
void const_insert(string name, types t, string value, int lev) {
	if (localSearch(name) != 0) {//标识符与本层标识符重名
		error(1);
		return;
	}
	else {
		if (curlev > 0 && symbolTable[btab[display[curlev - 1]].lastItem].name == name) {//常量与其所在函数重名
			error(1);
			return;
		}
		//int idx = globalSearch(name);
		//if (idx != 0 && symbolTable[idx].objTyp == function) {//标识符与函数重名
		//	error(1);
		//	return;
		//}
	}
	//将value转换为int
	int _value = 0;
	if (t == charTyp && value.size() > 1) {
		_value = value[1];
		enter_symtab(name, t, constant, value[1], 0);
	}
	else if (t == intTyp) {
		int sum = 0;
		if (value.size() > 0 && value[0] == '-') {
			for (int i = 1; i < value.size(); i++)
				sum = sum * 10 + value[i] - '0';
			sum = -sum;
		}
		else {
			for (int i = 0; i < value.size(); i++)
				sum = sum * 10 + value[i] - '0';
		}
		_value = sum;
		enter_symtab(name, t, constant, sum, 0);
	}
	symbolTable.back().addr = MEM_IDX;
	MEM_IDX++;
	//输出四元式
	stringstream ss;
	ss << _value;
	genQuadCode("const", (t == intTyp ? "int" : "char"), name, ss.str());
}
//将变量插入符号表
void var_insert(string name, types t, objType objt, int dimen) {
	if (localSearch(name) != 0) {//本层标识符重名
		error(1);
		return;
	}
	else {
		//当前层不能是第0层(最顶层)
		if (curlev > 0 && symbolTable[btab[display[curlev - 1]].lastItem].name == name) {//变量与其所在函数重名
			error(1);
			return;
		}
	}
	enter_symtab(name, t, objt, dimen, 0);//利用value项将数组的维数传入
	symbolTable.back().addr = MEM_IDX;
	if (objt == variable) {//变量
		genQuadCode("var", (t == intTyp ? "int" : "char"), name, "");
		MEM_IDX++;
	}
	else {//数组
		stringstream ss;
		ss << dimen;
		genQuadCode("arrayVar", (t == intTyp ? "int" : "char"), name, ss.str());
		MEM_IDX = MEM_IDX + dimen;
	}
}
//函数声明
void func_insert(string name, types t) {
	if (localSearch(name) != 0) {
		error(1);
		return;
	}
	enter_symtab(name, t, function, 0, 0);
	genQuadCode((t == intTyp ? "int" : (t == charTyp ? "char" : "void")), name, "()", "");
	symbolTable.back().ref = btabIndex;
	curlev++;

	//处理相应的分程序表，display区，并将寄存器表初始化为0，空间占用偏移设置为0
	bTabItem temp;
	temp.lastItem = 0;
	temp.lastPar = 0;
	temp.psize = 0;
	temp.isize = 0;
	btab[btabIndex] = temp;
	display[curlev] = btabIndex;
	btabIndex++;
	//TODO:此处表示每个函数进入的时候的为了存放一些栈指针所留出的空间
	MEM_IDX = MEM_SAVE;
}
//退出当前函数
void func_exit(string funcName) {
	curlev--;
	genQuadCode("endFunc", funcName, "", "");
}
//产生参数列表
void param_insert(string name, types t) {
	if (localSearch(name) != 0) {
		error(1);
		return;
	}
	else {
		if (curlev > 0 && symbolTable[btab[display[curlev - 1]].lastItem].name == name) {//变量与其所在函数重名
			error(1);
			return;
		}
	}
	enter_symtab(name, t, parameter, 0, 0);
	//TODO:参数需要减去函数的保留空间，因为它们并不是放在保留区后面，而是在保留区前面，所以其索引无需加上MEM_SAVE
	symbolTable.back().addr = MEM_IDX - MEM_SAVE;
	MEM_IDX++;
	genQuadCode("param", (t == intTyp ? "int" : "char"), name, "");
}
//产生运算类四元式
void calcu_quad(string op, int src1, int src2, int& result) {
	if (src1 < 0 && src2 < 0) {//如果操作符都是不是寄存器，那么分配一个新的寄存器
		result = allocReg();
		genQuadCode(op, leftOperand, rightOperand, buildRegName(result));
	}
	else if (src1 < 0 && src2 >= 0) {//否则直接利用其中的一个寄存器
		result = src2;
		genQuadCode(op, leftOperand, buildRegName(src2), buildRegName(result));
	}
	else if (src1 >= 0 && src2 < 0) {
		result = src1;
		genQuadCode(op, buildRegName(src1), rightOperand, buildRegName(result));
	}
	else {
		result = src1;
		genQuadCode(op, buildRegName(src1), buildRegName(src2), buildRegName(result));
	}
}
//函数调用
void funcCall(string funcName, int& regId, bool ifRet) {
	genQuadCode("call", funcName, "", "");//调用函数
	if (ifRet) {
		//如果要返回，才输出返回四元式
		regId = allocReg();
		genQuadCode("=", "@RET", "", buildRegName(regId));//获取返回值
	}
}
//调用函数之前参数入栈
void push_param(int regId) {
	genQuadCode("push", buildRegName(regId), "", "");
}
//生成条件四元式
void cond_quad(string relOp, string src1, string src2, int& newLabel, bool genNewLabel) {
	genQuadCode(relOp, src1, src2, "");
	if (genNewLabel) {//是否生成新的标签
		newLabel = genLabel();
	}
	stringstream ss;
	ss << labelPrefix << newLabel;
	genQuadCode("BZ", ss.str(), "", "");//如果之前的表达式为假那么跳转到标号
}
//无条件跳转指令
void jmp_quad(int& newLabel, bool generate) {
	if (generate) newLabel = genLabel();//是否新建标号
	stringstream ss;
	ss << labelPrefix << newLabel;
	genQuadCode("GOTO", ss.str(), "", "");
}
//放置标号
void placeLabel(int newLabel) {
	stringstream ss;
	ss << labelPrefix << newLabel << ":";
	genQuadCode(ss.str(), "", "", "");
}
//处理for语句中的步长赋值
void stepAssign(string op, string src1, string src2, string result) {
	genQuadCode(op, src1, src2, result);
}
//赋值语句四元式
void assign_quad(string src, string dest) {
	int search_result = globalSearch(dest);
	if (search_result == 0) {
		error(0);//未定义
		return;
	}
	genQuadCode("=", src, "", dest);
}
//为数组元素赋值
void assign_arr_quad(string arrName, string idx, string value) {
	genQuadCode("[]=", idx, value, arrName);
}
//读语句
void scanf_quad(string readItem) {
	int search_result = globalSearch(readItem);
	if (search_result == 0) {
		error(0);//未定义
		return;
	}
	else if (!(symbolTable[search_result].objTyp == parameter ||
		symbolTable[search_result].objTyp == variable)) {
		error(34);//数据不可读写
		return;
	}
	if (symbolTable[search_result].type == intTyp)
		genQuadCode("scanfInt", readItem, "", "");
	else
		genQuadCode("scanfChr", readItem, "", "");
}
//写语句，根据是否为字符串，是否为字符来决定相应输出
void printf_quad(int id, bool isStr, bool isChar) {
	if (isStr) {
		stringstream ss;
		ss << ".str" << id;
		genQuadCode("printStr", ss.str(), "", "");//输出字符串，id代表在字符串常量表中的编号
	}
	else if (isChar)
		genQuadCode("printChr", buildRegName(id), "", "");//输出字符
	else
		genQuadCode("printInt", buildRegName(id), "", "");
}
//写换行语句
void println_quad() {
	genQuadCode("println", "", "", "");
}
//返回语句四元式
void return_quad(string retItem) {//注意retItem可能是空字符串，也可能是正常函数名
	genQuadCode("ret", retItem, "", "");
}
//生成case的条件判断四元式，输入表达式寄存器以及常量字符串
void case_cond_quad(int exprReg, string con, int &nextLabel) {
	genQuadCode("==", buildRegName(exprReg), con, "");
	nextLabel = genLabel();
	stringstream ss;
	ss << labelPrefix << nextLabel;
	genQuadCode("BZ", ss.str(), "", "");//如果不满足条件，跳转到下一个label
}
#endif // !SEMANTIC_H
