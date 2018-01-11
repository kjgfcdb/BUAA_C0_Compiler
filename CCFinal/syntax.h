#ifndef SYNTAX_H
#define SYNTAX_H
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <set>
#include <map>
#include <cctype>
#include <string>
#include <sstream>
#include <cstdio>
#define MEM_SAVE 2 //进入函数时留出的空间，保存$ra以及$fp等
#define BUFFER_SIZE 100000000 //存放源文件字符串的大小
#define FILE_OUTPUT //控制是否输出到文件
#define OUTPUT_QUADCODE//控制是否输出四元式
using namespace std;

#ifdef FILE_OUTPUT
ofstream fout;
#define cout fout
#endif // FILE_OUTPUT

//符号定义
typedef enum {
	noSym,//空符号
	ident,//标识符

	intCon,//整型常量
	charCon,//字符常量
	strCon,//字符串常量

	intSy,//int
	charSy,//char
	voidSy,//void符号
	mainSy,//main符号
	printfSy,//printf
	scanfSy,//scanf
	constSy,//const
	forSy,//for
	ifSy,//if
	elseSy,//else
	switchSy,//switch
	caseSy,//case
	defaultSy,//default
	returnSy,//return

	lBrace,//左花括号
	rBrace,//右花括号
	lBracket,//左中括号
	rBracket,//右中括号
	lParent,//左小括号
	rParent,//右小括号
	gtr,//>
	lss,//<
	geq,//>=
	leq,//<=
	neq,//!=
	eql,//==
	assign,//=
	plusSy,//+
	minusSy,//-
	multSy,//*
	divSy,// /
	semicolon,//分号
	comma,//逗号
	colon,//冒号
	singleQuo,//单引号
	doubleQuo,//双引号
} symbol;
//类型
typedef enum {
	noTyp,//未定义类型
	voidTyp,//void类型
	intTyp,//整型
	charTyp//字符型
} types;
//对象类型
typedef enum {
	noObjTyp,
	parameter,//参数
	constant,//常量
	variable,//变量
	function, //函数
	arrayTyp,//数组型
	reservedTyp//保留类型
} objType;
//word 结构体,用于表示单个单词
typedef struct {
	string alpha;
	symbol sy;
	string value;
} word;
//符号表定义
typedef struct {
	string name = ""; // 符号表项名
	types type = noTyp; //类型，例如int,char或者void
	objType objTyp = noObjTyp;//对象类型，为常量、变量、函数、数组、参数之一
	int value = 0; //常量的int或者char的值
	int link = 0;//当前层符号表中前一个项在符号表中的索引，为0表示是本层第一个项
	int lev = 0; //所在层次
	int ref = 0;//如果是函数，那么是其在分程序表中的索引
	int dimen = 0;//如果是数组，这代表数组维数，如果是函数，这代表函数参数个数
	int addr = 0; //内存地址
} symbolTableItem;
//符号表
vector<symbolTableItem> symbolTable;
//字符串表项
typedef struct {
	string val;//字符串本体
} stringTableItem;
vector<stringTableItem> stringTable;
//分程序表项
typedef struct {
	int lastItem;//当前分程序最后一个标识符在符号表中位置
	int lastPar;//当前分程序最后一个参数在符号表中位置
	int psize;//当前分程序中参数所占的空间ParamSize
	int isize;//当前分程序中所有项所占空间ItemSize,ItemSize >= ParamSize
} bTabItem;
//分程序表
int btabIndex = 0;
bTabItem btab[10000];
//display区，用于指示对应level的btab项位置
int display[100];
//源程序当前指针
int cursor = 0;
//源程序总长度
int TOTAL_LENGTH = 0;
//源程序内容
char buffer[BUFFER_SIZE];
//定义枚举类型symbol,用以储存程序中出现的各种符号
vector<word> ksy;
//当前符号串，为string类型
string curSymbol;
//当前token
word curWord;
//当前行数
int curLineCnt = 1;
//当前层次数
int curlev = 0;
//错误计数
int errCnt = 0;
//主函数标识符
bool mainFuncFlag = false;
//当前函数类型，用于判断函数的返回语句是否符合函数类型
symbol curFuncType = noSym;
//当前函数是否有返回语句
bool funcRet = false;
//内存偏移量
int MEM_IDX = 0;
//结束符，用于做skip函数的参数组成部分
set<int> CONST_FOLLOW = { semicolon, constSy, intSy, charSy, voidSy, rBrace };
set<int> VAR_FOLLOW = { semicolon, comma, intSy, charSy, voidSy, rBrace };
set<int> FUNC_FOLLOW = { intSy, charSy, voidSy, rBrace };
set<int> EXPR_FOLLOW = { rParent, rBracket, semicolon, comma };
set<int> FACTOR_FIRST = { intCon, charCon, ident, lParent, plusSy, minusSy };
set<int> endSym = { semicolon, rParent, rBracket, rBrace, comma, colon };
//FIRST集合
set<int> EXPR_FIRST = { plusSy, minusSy, ident, intCon, charCon, lParent };//表达式的head符号集合
set<int> STMT_FIRST = { lBrace, ifSy, forSy, ident, scanfSy, printfSy, semicolon, switchSy, returnSy };
set<int> COMP_STMT_FIRST = { ifSy, forSy, lBrace, ident, scanfSy, printfSy, semicolon, switchSy, returnSy, intSy, charSy, constSy };
//错误集合
map<int, string> errormap;
void error(int errNo);
void skip(set<int> jumpset, int errNo);
void test(set<int> target, set<int> stop, int errNo);
int getIntValue(bool isNegative, string value);
void valueParam(string funcName);
void factor(int& regId);
void term(int& regId);
void expression(int &regId);
void param();
void constdec();
void compoundStatement();
void variabledec(int& funcDeclared);
void functiondec();
void statements();
void casetable(int exprReg,int& linkLabel,int exitLabel);
void defaultstatement(int linkLabel,int exitLabel);
void switchstatement();
void returnstatement();
void readstatement();
void writestatement();
void forstatement();
void condition(int& newLabel, bool branchIfFalse);
void ifstatement();
void statementlist();
void program();
//输出符号表
void printSymbolTable() {
	cout << endl << endl;
	cout << "--------Symbol Table--------" << endl;
	cout << setw(10) << "name"
		<< setw(10) << "type"
		<< setw(10) << "objtype"
		<< setw(10) << "value"
		<< setw(10) << "link"
		<< setw(10) << "lev"
		<< setw(10) << "ref"
		<< setw(10) << "dimen"
		<< setw(10) << "addr"
		<< endl;
	map<objType, string> objTypDict;
	objTypDict[noObjTyp] = "noObjTyp";
	objTypDict[parameter] = "parameter";
	objTypDict[constant] = "constant";
	objTypDict[variable] = "variable";
	objTypDict[function] = "function";
	objTypDict[arrayTyp] = "arrayTyp";
	map<types, string> typesDict;
	typesDict[noTyp] = "notyp";
	typesDict[voidTyp] = "void";
	typesDict[intTyp] = "int";
	typesDict[charTyp] = "char";

	for (int i = 1; i < symbolTable.size(); i++) {
		symbolTableItem temp = symbolTable[i];
		cout << setw(10) << temp.name
			<< setw(10) << typesDict[temp.type]
			<< setw(10) << objTypDict[temp.objTyp]
			<< setw(10) << temp.value
			<< setw(10) << temp.link
			<< setw(10) << temp.lev
			<< setw(10) << temp.ref
			<< setw(10) << temp.dimen
			<< setw(10) << temp.addr
			<< endl;
	}
}
//输出分程序表
void printBtab() {
	cout << endl << endl;
	cout << "--------Block Table--------" << endl;
	cout << setw(10) << "lastItem"
		<< setw(10) << "lastPara"
		<< setw(10) << "psize"
		<< setw(10) << "isize"
		<< endl;
	for (int i = 0; i < btabIndex; i++) {
		cout << setw(10) << btab[i].lastItem
			<< setw(10) << btab[i].lastPar
			<< setw(10) << btab[i].psize
			<< setw(10) << btab[i].isize
			<< endl;
	}
}
//输出字符串表
void printStrTable() {
	for (int i = 0; i < stringTable.size(); i++) {
		cout << setw(4) << i + 1 << stringTable[i].val << endl;
	}
}
#endif // !SYNTAX_H