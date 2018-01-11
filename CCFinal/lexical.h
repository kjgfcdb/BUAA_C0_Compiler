#ifndef LEXICAL_H
#define LEXICAL_H

#include "syntax.h"
#include "semantic.h"
//初始化关键字表
void init() {
	//初始化错误情况表
	errormap[0] = "undef identifer";//标识符未定义
	errormap[1] = "multi def identifier";//标识符重定义
	errormap[2] = "identifier";//必须是标识符
	errormap[3] = "(";//接下来代表应该是相应的字符
	errormap[4] = ")";
	errormap[5] = "[";
	errormap[6] = "]";
	errormap[7] = "{";
	errormap[8] = "}";
	errormap[9] = ":";
	errormap[10] = ";";
	errormap[11] = "=";
	errormap[12] = "+/-";
	errormap[13] = "integer constant";//必须是整数常量
	errormap[14] = "char constant";//必须是字符常量
	errormap[15] = "factor first symbol";//必须是factor的first符号
	errormap[16] = "parameter unclosed";//参数声明没有以逗号或者右括号结尾
	errormap[17] = "parameter first symbol";//必须是参数first符号
	errormap[18] = "const def first symbol";//必须是常量定义first符号
	errormap[19] = "var def unclosed";//变量定义结尾符号非法
	errormap[20] = "var def first symbol";//必须是变量定义first符号
	errormap[21] = "statement first symbol";//必须是语句first符号
	errormap[22] = "main.void";//main函数必须是void类型
	errormap[23] = "func.name";//函数名只能是标识符和main
	errormap[24] = "case must have at least 1 label";//case必须至少有一个情况
	errormap[25] = "case label must be constant";//case后必须是常量
	errormap[26] = "default";//必须是default符号
	errormap[27] = "lack of main function";//程序缺少main函数
	errormap[28] = "extra symbol at the end of program";//程序尾部多余字符
	errormap[29] = "quote doesn't match";//括号不匹配
	errormap[30] = "illegal character";//非法字符
	errormap[31] = "illegal symbol";//非法符号
	errormap[32] = "illegal symbol in return statement";//返回语句中非法符号
	errormap[33] = "func def in compStmt";//复合语句中出现函数定义
	errormap[34] = "unchangeable item";//该标识符不可改变，包括不能赋值以及不能用输入改变其值
	errormap[35] = "unsubscriptable item";//不可取下标
	errormap[36] = "uncallable item";//不是函数，不可调用
	errormap[37] = "invalid type";//类型无效，可能是返回了错误类型或者在无返回值函数中返回了，
								  //或者在有返回值函数中没有返回，还可能是switch语句的表达式类型与其常量类型不一致
								  //或者是给变量进行了不匹配类型的赋值
	errormap[38] = "wrong func param num";//函数参数个数不匹配
	errormap[39] = "duplicated label value";//case语句的标签值与前面有重复的
	errormap[40] = "index out of range or invalid index type";//数组索引超出下标或者数组索引类型不对

	//插入关键字表
	ksy.push_back({ "INT_SY",intSy ,"int" });
	ksy.push_back({ "INT_SY",charSy ,"char" });
	ksy.push_back({ "VOID_SY",voidSy ,"void" });
	ksy.push_back({ "MAIN_SY",mainSy ,"main" });
	ksy.push_back({ "PRINTF_SY",printfSy ,"printf" });
	ksy.push_back({ "SCANF_SY",scanfSy ,"scanf" });
	ksy.push_back({ "CONST_SY",constSy ,"const" });
	ksy.push_back({ "FOR_SY",forSy ,"for" });
	ksy.push_back({ "IF_SY",ifSy ,"if" });
	ksy.push_back({ "ELSE_SY",elseSy ,"else" });
	ksy.push_back({ "SWITCH_SY",switchSy ,"switch" });
	ksy.push_back({ "CASE_SY",caseSy ,"case" });
	ksy.push_back({ "DEFAULT_SY",defaultSy ,"default" });
	ksy.push_back({ "RETURN_SY",returnSy ,"return" });
	ksy.push_back({ "LEFT_BRACE",lBrace ,"{" });
	ksy.push_back({ "RIGHT_BRACE",rBrace ,"}" });
	ksy.push_back({ "LEFT_BRACKET",lBracket ,"[" });
	ksy.push_back({ "RIGHT_BRACKET",rBracket ,"]" });
	ksy.push_back({ "LEFT_PARENT",lParent ,"(" });
	ksy.push_back({ "RIGHT_PARENT",rParent ,")" });
	ksy.push_back({ "GTR",gtr,">" });
	ksy.push_back({ "LSS",lss,"<" });
	ksy.push_back({ "GEQ",geq,">=" });
	ksy.push_back({ "LEQ",leq,"<=" });
	ksy.push_back({ "NEQ",neq,"!=" });
	ksy.push_back({ "EQL",eql,"==" });
	ksy.push_back({ "ASSIGN",assign,"=" });
	ksy.push_back({ "PLUS",plusSy,"+" });
	ksy.push_back({ "MINUS",minusSy,"-" });
	ksy.push_back({ "MULT",multSy,"*" });
	ksy.push_back({ "DIVISION",divSy,"/" });
	ksy.push_back({ "SEMICOLON",semicolon,";" });
	ksy.push_back({ "COMMA",comma,"," });
	ksy.push_back({ "COLON",colon,":" });
	ksy.push_back({ "SINGLE_QUOTE",singleQuo,"'" });
	ksy.push_back({ "DOUBLE_QUOTE",doubleQuo,"\"" });
	//保留字登记符号表
	display[0] = 0;
	symbolTableItem placeHolder;
	symbolTable.push_back(placeHolder);//占位符

	//向分程序表中插入一个块，这就是主块
	bTabItem temp;
	temp.lastItem = symbolTable.size() - 1;
	temp.lastPar = 0;
	temp.psize = 0;
	temp.isize = 0;
	btab[btabIndex] = temp;
	display[curlev] = btabIndex;
	btabIndex++;
	
	//初始化四元式寄存器数组
	regUsed = 0;
}
//获取下一个符号，存入全局的string变量中
void getNextSymbol() {
	curSymbol.clear();
	// 跳过无关字符
	while ((buffer[cursor] == ' ' || buffer[cursor] == '\n' || buffer[cursor] == '\t'
		|| buffer[cursor] == '\0' || buffer[cursor] == '\r') && cursor < TOTAL_LENGTH) {
		if (buffer[cursor] == '\n') 
			curLineCnt++;
		cursor++;
	}
	if (cursor >= TOTAL_LENGTH) {
		curSymbol = "";
		return;
	}
	char temp = buffer[cursor++];

	if (temp == '!' || temp == '>' || temp == '<' || temp == '=') {
		curSymbol += temp;
		if (buffer[cursor] == '=') {
			cursor++;
			curSymbol += '=';
		}
	}
	else if (temp == ',' || temp == ':' || temp == ';' || temp == '{' || temp == '}' ||
		temp == '[' || temp == ']' || temp == '(' || temp == ')' ||
		temp == '+' || temp == '-' || temp == '*' || temp == '/') {
		curSymbol += temp;
	}
	else if (temp == '\'' || temp == '\"') {
		curSymbol += temp;
		char save_temp = temp;
		temp = buffer[cursor++];
		while (temp != save_temp && temp != '\n' && cursor < TOTAL_LENGTH) {
			curSymbol += temp;
			temp = buffer[cursor++];
		}
		if (temp != save_temp) {//这种情况说明遇到了换行或者文件已经读完,那么表示字符或者字符串匹配的失败
			error(29);
			return;
		}
		curSymbol += temp;
	}
	else if (isalpha(temp) || temp == '_') {
		do {
			curSymbol += temp;
			temp = buffer[cursor++];
		} while ((isalnum(temp) || temp == '_') && cursor < TOTAL_LENGTH);
		cursor--;
	}
	else if (isdigit(temp)) {
		do {
			curSymbol += temp;
			temp = buffer[cursor++];
		} while (isdigit(temp) && cursor < TOTAL_LENGTH);
		cursor--;
	}
	else {
		error(30);
	}
}
//检查整型常量是否合法
bool checkIntConst(string iCon) {
	if (iCon[0] == '0') {//以单个0开头的只能是有一个0
		if (iCon.size() == 1) return true;
		else return false;
	}
	//接下来不可能是0开头
	for (int i = 0; i < iCon.size(); i++) {
		if (!isdigit(iCon[i])) {
			return false;
		}
	}
	return true;
}
//检查字符型常量是否合法
bool checkCharConst(string cCon) {
	if (cCon.size() != 3)
		return false;
	if (cCon[0] != '\'' || cCon[cCon.size() - 1] != '\'')
		return false;
	if (!(isalnum(cCon[1]) ||
		cCon[1] == '+' || cCon[1] == '-' ||
		cCon[1] == '*' || cCon[1] == '/' ||
		cCon[1] == '_'
		)) {
		return false;
	}
	return true;
}
//检查字符串常量是否合法
bool checkStrConst(string sCon) {
	if (sCon.size() < 2) //检查长度
		return false;
	if (sCon[0] != '\"' || sCon[sCon.size() - 1] != '\"') //检查头尾
		return false;
	for (int i = 1; i < sCon.size() - 1; i++) {
		if (!(sCon[i] == 32 || sCon[i] == 33 ||
			(sCon[i] >= 35 && sCon[i] <= 126))) {
			return false;
		}
	}
	return true;
}
//检查标识符是否合法
bool checkIdent(string iden) {
	// 标识符以字母或者'_'开头
	if (!(isalpha(iden[0]) || iden[0] == '_'))
		return false;
	// 标识符剩余部分必须是字母或者数字或者'_'
	for (int i = 1; i < iden.size(); i++) {
		if (!(isalnum(iden[i]) || iden[i] == '_'
			)) {
			return false;
		}
	}
	return true;
}
//获取当前词
void getNextWord() {
	getNextSymbol();
	curWord = { "",noSym,"" };
	for (int i = 0; i < ksy.size(); i++) {//是关键字
		if (ksy[i].value == curSymbol) {
			curWord = ksy[i];
			return;
		}
	}
	curWord.value = curSymbol;
	if (curSymbol.size() == 0) {//空串
		return;
	}
	else if (checkIntConst(curWord.value)) {//是整型常量
		curWord.sy = intCon;
		curWord.alpha = "INT_CONST";
	}
	else if (checkCharConst(curWord.value)) {//是字符常量
		curWord.sy = charCon;
		curWord.alpha = "CHAR_CONST";
	}
	else if (checkStrConst(curWord.value)) {//是字符串常量
		curWord.sy = strCon;
		str_insert(curWord.value);
		curWord.alpha = "STRING_CONST";
	}
	else if (checkIdent(curWord.value)) {//是标识符
		curWord.sy = ident;
		curWord.alpha = "IDENTIFIER";
	}
	else {
		error(31);
		curWord.value = "";
	}
}
//词法分析过程
//void cifaParse() {
//	int row_count = 1;
//	while (1) {
//		getNextWord();
//		if (curWord.sy == noSym) break;
//		cout << row_count << "\t";
//		printf("%-15s\t", curWord.alpha.data());
//		cout << curWord.value << endl;
//		row_count++;
//	}
//}
#endif // !LEXICAL_H