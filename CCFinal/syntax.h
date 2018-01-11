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
#define MEM_SAVE 2 //���뺯��ʱ�����Ŀռ䣬����$ra�Լ�$fp��
#define BUFFER_SIZE 100000000 //���Դ�ļ��ַ����Ĵ�С
#define FILE_OUTPUT //�����Ƿ�������ļ�
#define OUTPUT_QUADCODE//�����Ƿ������Ԫʽ
using namespace std;

#ifdef FILE_OUTPUT
ofstream fout;
#define cout fout
#endif // FILE_OUTPUT

//���Ŷ���
typedef enum {
	noSym,//�շ���
	ident,//��ʶ��

	intCon,//���ͳ���
	charCon,//�ַ�����
	strCon,//�ַ�������

	intSy,//int
	charSy,//char
	voidSy,//void����
	mainSy,//main����
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

	lBrace,//������
	rBrace,//�һ�����
	lBracket,//��������
	rBracket,//��������
	lParent,//��С����
	rParent,//��С����
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
	semicolon,//�ֺ�
	comma,//����
	colon,//ð��
	singleQuo,//������
	doubleQuo,//˫����
} symbol;
//����
typedef enum {
	noTyp,//δ��������
	voidTyp,//void����
	intTyp,//����
	charTyp//�ַ���
} types;
//��������
typedef enum {
	noObjTyp,
	parameter,//����
	constant,//����
	variable,//����
	function, //����
	arrayTyp,//������
	reservedTyp//��������
} objType;
//word �ṹ��,���ڱ�ʾ��������
typedef struct {
	string alpha;
	symbol sy;
	string value;
} word;
//���ű���
typedef struct {
	string name = ""; // ���ű�����
	types type = noTyp; //���ͣ�����int,char����void
	objType objTyp = noObjTyp;//�������ͣ�Ϊ���������������������顢����֮һ
	int value = 0; //������int����char��ֵ
	int link = 0;//��ǰ����ű���ǰһ�����ڷ��ű��е�������Ϊ0��ʾ�Ǳ����һ����
	int lev = 0; //���ڲ��
	int ref = 0;//����Ǻ�������ô�����ڷֳ�����е�����
	int dimen = 0;//��������飬���������ά��������Ǻ��������������������
	int addr = 0; //�ڴ��ַ
} symbolTableItem;
//���ű�
vector<symbolTableItem> symbolTable;
//�ַ�������
typedef struct {
	string val;//�ַ�������
} stringTableItem;
vector<stringTableItem> stringTable;
//�ֳ������
typedef struct {
	int lastItem;//��ǰ�ֳ������һ����ʶ���ڷ��ű���λ��
	int lastPar;//��ǰ�ֳ������һ�������ڷ��ű���λ��
	int psize;//��ǰ�ֳ����в�����ռ�Ŀռ�ParamSize
	int isize;//��ǰ�ֳ�������������ռ�ռ�ItemSize,ItemSize >= ParamSize
} bTabItem;
//�ֳ����
int btabIndex = 0;
bTabItem btab[10000];
//display��������ָʾ��Ӧlevel��btab��λ��
int display[100];
//Դ����ǰָ��
int cursor = 0;
//Դ�����ܳ���
int TOTAL_LENGTH = 0;
//Դ��������
char buffer[BUFFER_SIZE];
//����ö������symbol,���Դ�������г��ֵĸ��ַ���
vector<word> ksy;
//��ǰ���Ŵ���Ϊstring����
string curSymbol;
//��ǰtoken
word curWord;
//��ǰ����
int curLineCnt = 1;
//��ǰ�����
int curlev = 0;
//�������
int errCnt = 0;
//��������ʶ��
bool mainFuncFlag = false;
//��ǰ�������ͣ������жϺ����ķ�������Ƿ���Ϻ�������
symbol curFuncType = noSym;
//��ǰ�����Ƿ��з������
bool funcRet = false;
//�ڴ�ƫ����
int MEM_IDX = 0;
//��������������skip�����Ĳ�����ɲ���
set<int> CONST_FOLLOW = { semicolon, constSy, intSy, charSy, voidSy, rBrace };
set<int> VAR_FOLLOW = { semicolon, comma, intSy, charSy, voidSy, rBrace };
set<int> FUNC_FOLLOW = { intSy, charSy, voidSy, rBrace };
set<int> EXPR_FOLLOW = { rParent, rBracket, semicolon, comma };
set<int> FACTOR_FIRST = { intCon, charCon, ident, lParent, plusSy, minusSy };
set<int> endSym = { semicolon, rParent, rBracket, rBrace, comma, colon };
//FIRST����
set<int> EXPR_FIRST = { plusSy, minusSy, ident, intCon, charCon, lParent };//���ʽ��head���ż���
set<int> STMT_FIRST = { lBrace, ifSy, forSy, ident, scanfSy, printfSy, semicolon, switchSy, returnSy };
set<int> COMP_STMT_FIRST = { ifSy, forSy, lBrace, ident, scanfSy, printfSy, semicolon, switchSy, returnSy, intSy, charSy, constSy };
//���󼯺�
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
//������ű�
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
//����ֳ����
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
//����ַ�����
void printStrTable() {
	for (int i = 0; i < stringTable.size(); i++) {
		cout << setw(4) << i + 1 << stringTable[i].val << endl;
	}
}
#endif // !SYNTAX_H