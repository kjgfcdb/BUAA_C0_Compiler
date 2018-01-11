#ifndef LEXICAL_H
#define LEXICAL_H

#include "syntax.h"
#include "semantic.h"
//��ʼ���ؼ��ֱ�
void init() {
	//��ʼ�����������
	errormap[0] = "undef identifer";//��ʶ��δ����
	errormap[1] = "multi def identifier";//��ʶ���ض���
	errormap[2] = "identifier";//�����Ǳ�ʶ��
	errormap[3] = "(";//����������Ӧ������Ӧ���ַ�
	errormap[4] = ")";
	errormap[5] = "[";
	errormap[6] = "]";
	errormap[7] = "{";
	errormap[8] = "}";
	errormap[9] = ":";
	errormap[10] = ";";
	errormap[11] = "=";
	errormap[12] = "+/-";
	errormap[13] = "integer constant";//��������������
	errormap[14] = "char constant";//�������ַ�����
	errormap[15] = "factor first symbol";//������factor��first����
	errormap[16] = "parameter unclosed";//��������û���Զ��Ż��������Ž�β
	errormap[17] = "parameter first symbol";//�����ǲ���first����
	errormap[18] = "const def first symbol";//�����ǳ�������first����
	errormap[19] = "var def unclosed";//���������β���ŷǷ�
	errormap[20] = "var def first symbol";//�����Ǳ�������first����
	errormap[21] = "statement first symbol";//���������first����
	errormap[22] = "main.void";//main����������void����
	errormap[23] = "func.name";//������ֻ���Ǳ�ʶ����main
	errormap[24] = "case must have at least 1 label";//case����������һ�����
	errormap[25] = "case label must be constant";//case������ǳ���
	errormap[26] = "default";//������default����
	errormap[27] = "lack of main function";//����ȱ��main����
	errormap[28] = "extra symbol at the end of program";//����β�������ַ�
	errormap[29] = "quote doesn't match";//���Ų�ƥ��
	errormap[30] = "illegal character";//�Ƿ��ַ�
	errormap[31] = "illegal symbol";//�Ƿ�����
	errormap[32] = "illegal symbol in return statement";//��������зǷ�����
	errormap[33] = "func def in compStmt";//��������г��ֺ�������
	errormap[34] = "unchangeable item";//�ñ�ʶ�����ɸı䣬�������ܸ�ֵ�Լ�����������ı���ֵ
	errormap[35] = "unsubscriptable item";//����ȡ�±�
	errormap[36] = "uncallable item";//���Ǻ��������ɵ���
	errormap[37] = "invalid type";//������Ч�������Ƿ����˴������ͻ������޷���ֵ�����з����ˣ�
								  //�������з���ֵ������û�з��أ���������switch���ı��ʽ�������䳣�����Ͳ�һ��
								  //�����Ǹ����������˲�ƥ�����͵ĸ�ֵ
	errormap[38] = "wrong func param num";//��������������ƥ��
	errormap[39] = "duplicated label value";//case���ı�ǩֵ��ǰ�����ظ���
	errormap[40] = "index out of range or invalid index type";//�������������±���������������Ͳ���

	//����ؼ��ֱ�
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
	//�����ֵǼǷ��ű�
	display[0] = 0;
	symbolTableItem placeHolder;
	symbolTable.push_back(placeHolder);//ռλ��

	//��ֳ�����в���һ���飬���������
	bTabItem temp;
	temp.lastItem = symbolTable.size() - 1;
	temp.lastPar = 0;
	temp.psize = 0;
	temp.isize = 0;
	btab[btabIndex] = temp;
	display[curlev] = btabIndex;
	btabIndex++;
	
	//��ʼ����Ԫʽ�Ĵ�������
	regUsed = 0;
}
//��ȡ��һ�����ţ�����ȫ�ֵ�string������
void getNextSymbol() {
	curSymbol.clear();
	// �����޹��ַ�
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
		if (temp != save_temp) {//�������˵�������˻��л����ļ��Ѿ�����,��ô��ʾ�ַ������ַ���ƥ���ʧ��
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
//������ͳ����Ƿ�Ϸ�
bool checkIntConst(string iCon) {
	if (iCon[0] == '0') {//�Ե���0��ͷ��ֻ������һ��0
		if (iCon.size() == 1) return true;
		else return false;
	}
	//��������������0��ͷ
	for (int i = 0; i < iCon.size(); i++) {
		if (!isdigit(iCon[i])) {
			return false;
		}
	}
	return true;
}
//����ַ��ͳ����Ƿ�Ϸ�
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
//����ַ��������Ƿ�Ϸ�
bool checkStrConst(string sCon) {
	if (sCon.size() < 2) //��鳤��
		return false;
	if (sCon[0] != '\"' || sCon[sCon.size() - 1] != '\"') //���ͷβ
		return false;
	for (int i = 1; i < sCon.size() - 1; i++) {
		if (!(sCon[i] == 32 || sCon[i] == 33 ||
			(sCon[i] >= 35 && sCon[i] <= 126))) {
			return false;
		}
	}
	return true;
}
//����ʶ���Ƿ�Ϸ�
bool checkIdent(string iden) {
	// ��ʶ������ĸ����'_'��ͷ
	if (!(isalpha(iden[0]) || iden[0] == '_'))
		return false;
	// ��ʶ��ʣ�ಿ�ֱ�������ĸ�������ֻ���'_'
	for (int i = 1; i < iden.size(); i++) {
		if (!(isalnum(iden[i]) || iden[i] == '_'
			)) {
			return false;
		}
	}
	return true;
}
//��ȡ��ǰ��
void getNextWord() {
	getNextSymbol();
	curWord = { "",noSym,"" };
	for (int i = 0; i < ksy.size(); i++) {//�ǹؼ���
		if (ksy[i].value == curSymbol) {
			curWord = ksy[i];
			return;
		}
	}
	curWord.value = curSymbol;
	if (curSymbol.size() == 0) {//�մ�
		return;
	}
	else if (checkIntConst(curWord.value)) {//�����ͳ���
		curWord.sy = intCon;
		curWord.alpha = "INT_CONST";
	}
	else if (checkCharConst(curWord.value)) {//���ַ�����
		curWord.sy = charCon;
		curWord.alpha = "CHAR_CONST";
	}
	else if (checkStrConst(curWord.value)) {//���ַ�������
		curWord.sy = strCon;
		str_insert(curWord.value);
		curWord.alpha = "STRING_CONST";
	}
	else if (checkIdent(curWord.value)) {//�Ǳ�ʶ��
		curWord.sy = ident;
		curWord.alpha = "IDENTIFIER";
	}
	else {
		error(31);
		curWord.value = "";
	}
}
//�ʷ���������
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