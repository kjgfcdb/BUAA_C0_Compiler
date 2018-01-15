#ifndef SEMANTIC_H
#define SEMANTIC_H
#include "syntax.h"
typedef struct {
	string op;
	string left;
	string right;
	string result;
} quadCode;
//��Ԫʽ��
vector<quadCode> quadCodeTable;
string operand;//���ʽ������Ҫ�õ��Ĳ�����
string leftOperand;//���ʽ������Ҫ�õ����������
string rightOperand;//���ʽ������Ҫ�õ����Ҳ�����
bool isExprChar;//���ʽ����Ƿ�Ϊchar����
bool isExprStatic;//�жϱ��ʽ�Ƿ�Ϊ��̬����̬��ζ�ſ��������ֵ
int exprVal;//���ʽ�±��ֵ
string regPrefix = "#reg";//��ʱ������ǰ׺
string labelPrefix = "Label_";//��ǩ��ǰ׺
//�жϴ���Ԫʽ�Ƿ�Ϊ��������Ԫʽ
bool isQuadCalc(quadCode& qc) {
	if (qc.op == "+" || qc.op == "-" || qc.op == "*" || qc.op == "/")
		return true;
	return false;
}
//�жϸ���Ԫʽ�Ƿ�Ϊ�Ƚ���Ԫʽ
bool isQuadComp(quadCode& qc) {
	if (qc.op == "==" || qc.op == "!=" || qc.op == "<=" || 
		qc.op == ">=" || qc.op == "<" || qc.op == ">")
		return true;
	return false;
}
//�жϸ���Ԫʽ�Ƿ�Ϊ��ǩ
bool isQuadLabel(quadCode& qc) {
	if (qc.op.size() > labelPrefix.size() &&
		qc.op.substr(0, labelPrefix.size()) == labelPrefix)
		return true;
	return false;
}
//�жϸ���Ԫʽ�Ƿ�Ϊ����
bool isQuadFunc(quadCode& qc) {
	return qc.right == "()";
}
//�ж��ַ���ic�Ƿ��������Ҳ����123����-1����'a'
bool isConstant(string& ic) {
	if (ic.size() > 0 && (isdigit(ic[0]) || ic[0] == '+' ||
		ic[0] == '-' || ic[0] == '\'')) {
		return true;
	}
	return false;
}
//�ж��ַ���it��Ϊ��ʱ�����ַ���
bool isTemp(string& it) {
	if (it.size() > 0 && it[0] == '#')
		return true;
	return false;
}
//�����Ĵ�����
string buildRegName(int regId) {
	stringstream ss;
	ss << regPrefix << regId;
	return ss.str();
}
int regUsed;//��ʹ�õļĴ�������
//���ɱ��
int genLabel() {
	static int labelCnt = 0;
	return labelCnt++;
}
//����Ĵ���
int allocReg() {//ֱ���üĴ����ż�һ
	return regUsed++;
}
//�ڵ�ǰ���ű��в��ұ�ʶ��
int localSearch(string name) {
	int j;
	symbolTable[0].name = name; //�����ڱ�
	j = btab[display[curlev]].lastItem;//��ȡ��ǰ�����һ�����ű���λ��
	while (symbolTable[j].name != name)//������������
		j = symbolTable[j].link;
	return j;//��������λ��
}
//��ȫ�ַ��ű��в��ұ�ʶ��
int globalSearch(string name) {
	int i = curlev, j;
	symbolTable[0].name = name; //�����ڱ�
	while (true) {
		j = btab[display[i]].lastItem;//��ȡ��ǰ�����һ�����ű���λ��
		while (symbolTable[j].name != name)//������������
			j = symbolTable[j].link;
		i--;
		if (i < 0 || j != 0) break;//������������з��ű�������ҵ�����Ӧ��ʶ��
	}
	return j;//��������λ��
}
//������Ԫʽ
void genQuadCode(string op, string src1, string src2, string result) {
	quadCodeTable.push_back({ op,src1,src2,result });
}
//�ǼǷ��ű�
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
		//��һ������һ�����ű�����ǵ�ǰ��ĺ��������������dimen��һ����ʾ����������һ
		symbolTable[btab[display[curlev - 1]].lastItem].dimen += 1;
		//����ǲ�������ô����Ҫ�޸Ĳ���ָ��
		btab[display[curlev]].lastPar = symbolTable.size() - 1;
		btab[display[curlev]].psize += 1;
		btab[display[curlev]].isize += 1;
	}
	else if (objt == constant || objt == variable) { //ռ�ÿռ��һ
		btab[display[curlev]].isize += 1;
	}
	else if (objt == arrayTyp) {
		//������������ͣ���ôӦ�ý��ֳ�����isize��������Ĵ�С
		btab[display[curlev]].isize += value;//�����value�������ά��
	}
}
//���ַ��������ַ�������
void str_insert(string str) {
	stringTableItem temp;
	temp.val = str;
	stringTable.push_back(temp);
}
//������������ű�
void const_insert(string name, types t, string value, int lev) {
	if (localSearch(name) != 0) {//��ʶ���뱾���ʶ������
		error(1);
		return;
	}
	else {
		if (curlev > 0 && symbolTable[btab[display[curlev - 1]].lastItem].name == name) {//�����������ں�������
			error(1);
			return;
		}
		//int idx = globalSearch(name);
		//if (idx != 0 && symbolTable[idx].objTyp == function) {//��ʶ���뺯������
		//	error(1);
		//	return;
		//}
	}
	//��valueת��Ϊint
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
	//�����Ԫʽ
	stringstream ss;
	ss << _value;
	genQuadCode("const", (t == intTyp ? "int" : "char"), name, ss.str());
}
//������������ű�
void var_insert(string name, types t, objType objt, int dimen) {
	if (localSearch(name) != 0) {//�����ʶ������
		error(1);
		return;
	}
	else {
		//��ǰ�㲻���ǵ�0��(���)
		if (curlev > 0 && symbolTable[btab[display[curlev - 1]].lastItem].name == name) {//�����������ں�������
			error(1);
			return;
		}
	}
	enter_symtab(name, t, objt, dimen, 0);//����value������ά������
	symbolTable.back().addr = MEM_IDX;
	if (objt == variable) {//����
		genQuadCode("var", (t == intTyp ? "int" : "char"), name, "");
		MEM_IDX++;
	}
	else {//����
		stringstream ss;
		ss << dimen;
		genQuadCode("arrayVar", (t == intTyp ? "int" : "char"), name, ss.str());
		MEM_IDX = MEM_IDX + dimen;
	}
}
//��������
void func_insert(string name, types t) {
	if (localSearch(name) != 0) {
		error(1);
		return;
	}
	enter_symtab(name, t, function, 0, 0);
	genQuadCode((t == intTyp ? "int" : (t == charTyp ? "char" : "void")), name, "()", "");
	symbolTable.back().ref = btabIndex;
	curlev++;

	//������Ӧ�ķֳ����display���������Ĵ������ʼ��Ϊ0���ռ�ռ��ƫ������Ϊ0
	bTabItem temp;
	temp.lastItem = 0;
	temp.lastPar = 0;
	temp.psize = 0;
	temp.isize = 0;
	btab[btabIndex] = temp;
	display[curlev] = btabIndex;
	btabIndex++;
	//TODO:�˴���ʾÿ�����������ʱ���Ϊ�˴��һЩջָ���������Ŀռ�
	MEM_IDX = MEM_SAVE;
}
//�˳���ǰ����
void func_exit(string funcName) {
	curlev--;
	genQuadCode("endFunc", funcName, "", "");
}
//���������б�
void param_insert(string name, types t) {
	if (localSearch(name) != 0) {
		error(1);
		return;
	}
	else {
		if (curlev > 0 && symbolTable[btab[display[curlev - 1]].lastItem].name == name) {//�����������ں�������
			error(1);
			return;
		}
	}
	enter_symtab(name, t, parameter, 0, 0);
	//TODO:������Ҫ��ȥ�����ı����ռ䣬��Ϊ���ǲ����Ƿ��ڱ��������棬�����ڱ�����ǰ�棬�����������������MEM_SAVE
	symbolTable.back().addr = MEM_IDX - MEM_SAVE;
	MEM_IDX++;
	genQuadCode("param", (t == intTyp ? "int" : "char"), name, "");
}
//������������Ԫʽ
void calcu_quad(string op, int src1, int src2, int& result) {
	if (src1 < 0 && src2 < 0) {//������������ǲ��ǼĴ�������ô����һ���µļĴ���
		result = allocReg();
		genQuadCode(op, leftOperand, rightOperand, buildRegName(result));
	}
	else if (src1 < 0 && src2 >= 0) {//����ֱ���������е�һ���Ĵ���
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
//��������
void funcCall(string funcName, int& regId, bool ifRet) {
	genQuadCode("call", funcName, "", "");//���ú���
	if (ifRet) {
		//���Ҫ���أ������������Ԫʽ
		regId = allocReg();
		genQuadCode("=", "@RET", "", buildRegName(regId));//��ȡ����ֵ
	}
}
//���ú���֮ǰ������ջ
void push_param(int regId) {
	genQuadCode("push", buildRegName(regId), "", "");
}
//����������Ԫʽ
void cond_quad(string relOp, string src1, string src2, int& newLabel, bool genNewLabel) {
	genQuadCode(relOp, src1, src2, "");
	if (genNewLabel) {//�Ƿ������µı�ǩ
		newLabel = genLabel();
	}
	stringstream ss;
	ss << labelPrefix << newLabel;
	genQuadCode("BZ", ss.str(), "", "");//���֮ǰ�ı��ʽΪ����ô��ת�����
}
//��������תָ��
void jmp_quad(int& newLabel, bool generate) {
	if (generate) newLabel = genLabel();//�Ƿ��½����
	stringstream ss;
	ss << labelPrefix << newLabel;
	genQuadCode("GOTO", ss.str(), "", "");
}
//���ñ��
void placeLabel(int newLabel) {
	stringstream ss;
	ss << labelPrefix << newLabel << ":";
	genQuadCode(ss.str(), "", "", "");
}
//����for����еĲ�����ֵ
void stepAssign(string op, string src1, string src2, string result) {
	genQuadCode(op, src1, src2, result);
}
//��ֵ�����Ԫʽ
void assign_quad(string src, string dest) {
	int search_result = globalSearch(dest);
	if (search_result == 0) {
		error(0);//δ����
		return;
	}
	genQuadCode("=", src, "", dest);
}
//Ϊ����Ԫ�ظ�ֵ
void assign_arr_quad(string arrName, string idx, string value) {
	genQuadCode("[]=", idx, value, arrName);
}
//�����
void scanf_quad(string readItem) {
	int search_result = globalSearch(readItem);
	if (search_result == 0) {
		error(0);//δ����
		return;
	}
	else if (!(symbolTable[search_result].objTyp == parameter ||
		symbolTable[search_result].objTyp == variable)) {
		error(34);//���ݲ��ɶ�д
		return;
	}
	if (symbolTable[search_result].type == intTyp)
		genQuadCode("scanfInt", readItem, "", "");
	else
		genQuadCode("scanfChr", readItem, "", "");
}
//д��䣬�����Ƿ�Ϊ�ַ������Ƿ�Ϊ�ַ���������Ӧ���
void printf_quad(int id, bool isStr, bool isChar) {
	if (isStr) {
		stringstream ss;
		ss << ".str" << id;
		genQuadCode("printStr", ss.str(), "", "");//����ַ�����id�������ַ����������еı��
	}
	else if (isChar)
		genQuadCode("printChr", buildRegName(id), "", "");//����ַ�
	else
		genQuadCode("printInt", buildRegName(id), "", "");
}
//д�������
void println_quad() {
	genQuadCode("println", "", "", "");
}
//���������Ԫʽ
void return_quad(string retItem) {//ע��retItem�����ǿ��ַ�����Ҳ����������������
	genQuadCode("ret", retItem, "", "");
}
//����case�������ж���Ԫʽ��������ʽ�Ĵ����Լ������ַ���
void case_cond_quad(int exprReg, string con, int &nextLabel) {
	genQuadCode("==", buildRegName(exprReg), con, "");
	nextLabel = genLabel();
	stringstream ss;
	ss << labelPrefix << nextLabel;
	genQuadCode("BZ", ss.str(), "", "");//�����������������ת����һ��label
}
#endif // !SEMANTIC_H
