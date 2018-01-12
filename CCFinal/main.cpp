#include "codeGenerate.h"

//#define CHECK_INT_TO_CHAR // int������char����ת���Ƿ�Ҫ���
#define OPTIMIZE_ON //�Ƿ����Ż�
void error(int errNo) { //���������Ϣ��������������ͳ���Լ�������������
	errCnt++;
	printf("Error %d : %s at line %d\n", errCnt, errormap[errNo].c_str(), curLineCnt);
}
//����jumpset�еķ���Ϊֹ�����Ҹ���errNo����
void skip(set<int> jumpset, int errNo) {
	error(errNo);
	while (true) {
		if (cursor >= TOTAL_LENGTH) break;
		if (jumpset.find(curWord.sy) != jumpset.end()) break;//����ҵ��˾�ֹͣ
		getNextWord();
	}
}
//�жϵ�ǰ�����Ƿ�����target���ϣ�������ǣ�������target�Լ�stop���ż���Ϊֹ
void test(set<int> target, set<int> stop, int errNo) {
	if (target.find(curWord.sy) == target.end()) {//target��û�е�ǰ����
		target.insert(stop.begin(), stop.end());
		skip(target, errNo);
	}
	else getNextWord();
}
//��ȡ����������ֵ
int getIntValue(bool isNegative, string value) {
	int ret = 0;
	if (value.size() > 0) {
		if (value[0] == '+') {//��+�ű�ǩ
			for (int i = 1; i < value.size(); i++) ret = ret * 10 + value[i] - '0';
		}
		else if (value[0] == '-') {//��-�ű�ǩ
			for (int i = 1; i < value.size(); i++) ret = ret * 10 + value[i] - '0';
			ret = -ret;
		}
		else if (value.size() > 1 && value[0] == '\'')  //�ַ�����
			ret = value[1];
		else //��������
			for (int i = 0; i < value.size(); i++) ret = ret * 10 + value[i] - '0';
		return isNegative ? -ret : ret;
	}
	else return 0;
}
//����ֵ�����б�
void valueParam(string funcName) {
	int tempReg, paramNum = 0, paramIdx;
	int funcIdx = globalSearch(funcName);
	paramIdx = funcIdx;
	while (curWord.sy != rParent) {
		expression(tempReg);
		push_param(tempReg);
		paramIdx++;
		if (symbolTable[paramIdx].objTyp == parameter) {//ֻ�ж��ǲ��������
			if (symbolTable[paramIdx].type == charTyp && !isExprChar)//������������ʽ���Ͳ�ƥ��
				error(37);
			else if (symbolTable[paramIdx].type == intTyp && isExprChar)//������������ʽ���Ͳ�ƥ��
				error(37);
		}
		paramNum++;
		while (curWord.sy == comma) {
			getNextWord();
			expression(tempReg);
			push_param(tempReg);
			paramIdx++;
			if (symbolTable[paramIdx].objTyp == parameter) {//ֻ�ж��ǲ��������
				if (symbolTable[paramIdx].type == charTyp && !isExprChar)//������������ʽ���Ͳ�ƥ��
					error(37);
				else if (symbolTable[paramIdx].type == intTyp && isExprChar)//������������ʽ���Ͳ�ƥ��
					error(37);
			}
			paramNum++;
		}
	}
	if (symbolTable[funcIdx].dimen != paramNum)
		error(38);//����������ƥ��
}
//��������
void factor(int& regId) {
	static int regCnt = 0;
	int tempBool = false;
	int tempReg;
	if (curWord.sy == ident) {//��ʶ��
		int search_result = globalSearch(curWord.value);
		if (search_result == 0) {
			skip({ comma,semicolon }, 0);//δ����ֱ�ӱ���
			return;
		}
		if (search_result != 0 &&
			symbolTable[search_result].type == charTyp) {
			//����ڷ��ű��иñ�ʶ���Ǽǵ���char���ͣ�
			//��ô������char���黹��char�������߼򵥵�char�䳣�������صĶ����ַ���
			tempBool = true;
		}
		string ident_save = curWord.value;
		getNextWord();
		if (curWord.sy == lBracket) {//����Ԫ��
			if (symbolTable[search_result].objTyp != arrayTyp) {
				skip({ semicolon }, 35); //�������飬����ȡ�±�
				return;
			}
			getNextWord();
			exprVal = -1;
			isExprStatic = false;
			expression(tempReg);//���ʽ��ֵ�����tempReg��
			if (isExprStatic && (isExprChar || exprVal < 0 || exprVal >= symbolTable[search_result].dimen)) {
				//����±�Ϊ��̬���������ֵ������Χ�������±����;���char���ͣ���ô����
				skip({ semicolon }, 40); //�����±곬����Χ
				return;
			}
			leftOperand = ident_save;//�������Ҳ�����
			rightOperand = buildRegName(tempReg);
			calcu_quad("=[]", -1, -1, regId);//������Ԫʽ����ȡֵ,	=[],a,i,result
			test({ rBracket }, EXPR_FOLLOW, 6);
		}
		else if (curWord.sy == lParent) {//�з���ֵ��������
			if (symbolTable[search_result].objTyp != function)
				error(36);//���Ǻ��������ܵ���
			if (symbolTable[search_result].type == voidTyp)
				error(37);//Ϊvoid���ͣ����ܲ�������ֵ
			getNextWord();
			valueParam(ident_save);
			funcCall(ident_save, regId, true);
			test({ rParent }, EXPR_FOLLOW, 4);
		}
		else {//����ֻ��һ������ʶ��������Ԫʽ����в�����Ĵ�����ֱ���ñ�ʶ����
			if (symbolTable[search_result].objTyp != variable &&//������ʶ��ֻ���Ǳ������������߳��������������������������ﲻ����
				symbolTable[search_result].objTyp != parameter &&
				symbolTable[search_result].objTyp != constant) {
				error(37);
			}
			regId = -1;//������Ĵ���
			operand = ident_save;
		}
		isExprStatic = false;//��ʱ����ֵ̬��־��Ϊfalse
		isExprChar = tempBool;
		if (symbolTable[search_result].objTyp == constant) {
			exprVal = symbolTable[search_result].value;//����ǳ�����ֱ��ȡ��ֵ
			isExprStatic = true;//�������������Ϊ��̬
		}
	}
	else if (curWord.sy == charCon || curWord.sy == intCon) {
		isExprChar = curWord.sy == charCon;
		exprVal = getIntValue(false, curWord.value);//���±��ʽ��ֵ
		isExprStatic = true;
		regId = -1;//����Ҳ������Ĵ���
		operand = curWord.value;
		getNextWord();
	}
	else if (curWord.sy == plusSy || curWord.sy == minusSy) {
		string op = curWord.sy == minusSy ? "-" : "+";
		getNextWord();
		if (curWord.sy == intCon) {
			leftOperand = "0";
			rightOperand = curWord.value;
			exprVal = getIntValue(false, op + curWord.value);//���±��ʽ��ֵ
			isExprStatic = true;
			isExprChar = false;
			calcu_quad(op, -1, -1, regId);//����#regi = 0-2���͵�����
			getNextWord();
		}
		else test({ intCon }, EXPR_FOLLOW, 13);
	}
	else if (curWord.sy == lParent) {
		getNextWord();
		expression(tempReg);
		test({ rParent }, EXPR_FOLLOW, 4);
		regId = tempReg;//ֱ��ʹ�ñ��ʽ�ķ��ؼĴ���
	}
	else {
		skip(EXPR_FOLLOW, 15);
	}
}
//������
void term(int& regId) {
	string lop, rop;
	int tempExprVal;
	bool tempIsExprStatic = false;
	factor(regId);
	tempExprVal = exprVal;
	tempIsExprStatic = isExprStatic;
	if (regId == -1) lop = operand;
	while (curWord.sy == multSy || curWord.sy == divSy) {
		string op = curWord.sy == multSy ? "*" : "/";
		int tempReg;
		getNextWord();
		factor(tempReg);
		tempIsExprStatic = tempIsExprStatic && isExprStatic;
		if (tempIsExprStatic) tempExprVal = op == "*" ? tempExprVal * exprVal : tempExprVal / exprVal;//������ʽ��ֵ
		if (tempReg == -1) rop = operand;//���������
		leftOperand = lop;
		rightOperand = rop;
		calcu_quad(op, regId, tempReg, regId);
		isExprChar = false;//�������������㣬����char����
	}
	exprVal = tempExprVal;
	isExprStatic = tempIsExprStatic;
}
//������ʽ����ǰ����Ϊ+,-���߱�ʶ��,����,������(��ʾ�ַ�),������
void expression(int& regId) {
	int tempExprVal;
	string lop, rop;
	bool isNegtive = false, tempIsExprStatic = false;
	if (curWord.sy == plusSy || curWord.sy == minusSy) {
		isNegtive = curWord.sy == minusSy ? true : false;
		getNextWord();
	}
	term(regId);
	tempExprVal = exprVal;
	tempIsExprStatic = isExprStatic;
	if (regId == -1) lop = operand;
	if (isNegtive) {//�����һ���������
		leftOperand = "0";
		rightOperand = lop;
		calcu_quad("-", -1, regId, regId);
		tempExprVal = -tempExprVal;
	}
	while (curWord.sy == plusSy || curWord.sy == minusSy) {
		string op = curWord.sy == plusSy ? "+" : "-";
		int tempReg;
		getNextWord();
		term(tempReg);
		tempIsExprStatic = tempIsExprStatic && isExprStatic;
		if (tempIsExprStatic) tempExprVal = op == "+" ? tempExprVal + exprVal : tempExprVal - exprVal;//������ʽ��ֵ
		if (tempReg == -1) rop = operand;//���������
		leftOperand = lop;
		rightOperand = rop;
		calcu_quad(op, regId, tempReg, regId);
		isExprChar = false;//���������㣬����char����
	}
	if (regId == -1) {//���regId����û�б����䣬��ôǿ�з���һ���Ĵ���
		leftOperand = lop;
		rightOperand = "";
		calcu_quad("=", -1, -1, regId);
	}
	exprVal = tempExprVal;//�����ʱ�ı��ʽ��ֵ����exprVal
	isExprStatic = tempIsExprStatic;
}
//����������
void param() {
	if (curWord.sy == rParent) {//������Ϊ�գ�ֱ�ӽ���
		return;
	}
	else if (curWord.sy == intSy || curWord.sy == charSy) {//������Ϊ�գ���ʼ�������
		while (curWord.sy == intSy || curWord.sy == charSy) {
			types save_type = curWord.sy == intSy ? intTyp : charTyp;
			getNextWord();
			if (curWord.sy == ident) {
				param_insert(curWord.value, save_type);
			}
			test({ ident }, { comma, rParent }, 2);
			if (curWord.sy == comma) getNextWord();
			else if (curWord.sy == rParent) break;
			else {
				skip({ lBrace,comma, rParent }, 16);
			}
		}
	}
	else {
		error(17);
	}
}
//��������������,    ���������壾 ::=  int����ʶ��������������{,����ʶ��������������} | char����ʶ���������ַ���{ ,����ʶ���������ַ��� }
void singleConstDef(symbol intOrchar) {
	string name = curWord.value;//��ʶ������
	if (intOrchar == intSy) {
		//�Ƿ�Ϊ����
		bool isNegative = false;
		// �������Ǳ�ʶ��
		test({ ident }, { comma, semicolon }, 2);
		// ��ʶ�������Ǹ�ֵ����=
		if (curWord.sy != assign) {
			skip({ comma, semicolon }, 11);
		}
		else {
			getNextWord();
			if (curWord.sy == plusSy || curWord.sy == minusSy) {
				isNegative = curWord.sy == minusSy;
				getNextWord();
			}
			string value = isNegative ? "-" + curWord.value : curWord.value;
			test({ intCon }, { comma, semicolon }, 13);
			const_insert(name, intTyp, value, curlev);
		}
	}
	else if (intOrchar == charSy) {
		test({ ident }, { comma, semicolon }, 2);
		// ��ʶ�������Ǹ�ֵ����=
		if (curWord.sy != assign)
			skip({ comma, semicolon }, 11);
		else {
			getNextWord();
			string value = curWord.value;
			test({ charCon }, { comma, semicolon }, 14);
			const_insert(name, charTyp, value, curlev);
		}
	}
}
//������������������������,  ������˵���� ::=  const���������壾;{ const���������壾;}
void constdec() {
	getNextWord();//��ǰ�Ѿ���const
				  // ��int����char��ͷ
	if (curWord.sy == intSy || curWord.sy == charSy) {
		//���浱ǰ����
		symbol save_type = curWord.sy;
		getNextWord();
		singleConstDef(save_type);//��������������
		while (curWord.sy == comma) {
			getNextWord();
			singleConstDef(save_type);
		}
	}
	else {
		skip(CONST_FOLLOW, 18);
	}
	if (curWord.sy == semicolon) {
		getNextWord();
	}
	else {
		//��ת���ֺţ�const��int��char��void�����Ҵ�����Ϊֹ
		skip(CONST_FOLLOW, 10);
		if (endSym.find(curWord.sy) != endSym.end()) getNextWord();
	}
}
//������䴦��
void compoundStatement() {
	int placeholder = 0;//ռλ������ʵ������
	while (curWord.sy == constSy) {
		constdec();
	}
	while ((curWord.sy == intSy || curWord.sy == charSy)) {
		variabledec(placeholder);
		if (placeholder == 1)
			error(33);
	}
	statementlist();
}
//�������壬��ǰ�Ѿ��������������ţ�׼���������
void funcBody(types funcType, string funcName) {
	param();//����������
	test({ rParent }, { lBrace }, 4);
	test({ lBrace }, COMP_STMT_FIRST, 7);
	curFuncType = funcType == intTyp ? intSy : (funcType == charTyp ? charSy : voidSy);
	funcRet = false;//��ʼ�趨��ǰ����û�з���ֵ
	compoundStatement();//���������
	if (curFuncType != voidSy && funcRet == false) //��ǰ��������void����ȴû�з�����䣬����
		error(37);
	curFuncType = noSym;//��λ
	test({ rBrace }, { intSy, charSy, voidSy }, 8);
	func_exit(funcName);//�˳�����
}
//���������������ǰʶ��int����char
void variabledec(int &funcDeclared) {
	//��ǰ��Щ���������ͣ�Ϊintsy����charsy
	types c_type = curWord.sy == intSy ? intTyp : charTyp;
	int flag = 0;
	getNextWord();
	//��ǰ�ַ���Ҫ�Ǳ�ʶ��
	while (curWord.sy == ident) {
		string save_ident = curWord.value;
		getNextWord();
		flag = 1;
		if (curWord.sy == comma) {//��������
			var_insert(save_ident, c_type, variable, 0);//�������
			getNextWord();
		}
		else if (curWord.sy == semicolon) {//�����ֺ�
			var_insert(save_ident, c_type, variable, 0);//�������
			getNextWord();
			break;
		}
		else if (curWord.sy == lBracket) {//�����������ţ�����������
			getNextWord();//�õ�����
			int dimen = getIntValue(false, curWord.value);//����ά��
			getNextWord();//�õ���������
			test({ rBracket }, VAR_FOLLOW, 6);
			var_insert(save_ident, c_type, arrayTyp, dimen);//�������
			symbolTable.back().dimen = dimen;//��������ά��
			if (curWord.sy == comma) getNextWord();
			else if (curWord.sy == semicolon) {
				getNextWord();
				break;
			}
			else error(19);
		}
		else if (curWord.sy == lParent) {//������С���ţ������Ǻ���
			funcDeclared = 1;
			func_insert(save_ident, c_type);//����������ʼ
			getNextWord();
			funcBody(c_type, save_ident);//���������뺯����,��������
			return;
		}
		else {
			error(20);
		}
	}
	if (flag == 0) {
		error(2);
	}
}
//���������壬��ǰʶ��void,int����char
void functiondec() {
	symbol savesymbol = curWord.sy;
	types save_type = curWord.sy == intSy ? intTyp : (curWord.sy == charSy ? charTyp : voidTyp);
	getNextWord();
	string funcName = curWord.value;
	if (curWord.sy == ident) {
		func_insert(funcName, save_type);//����������ʼ
		getNextWord();
		test({ lParent }, { intSy, charSy }, 3);
		funcBody(save_type, funcName);
	}
	else if (curWord.sy == mainSy) {
		if (savesymbol == voidSy) {
			func_insert(curWord.value, save_type);//���뺯��������Ԫʽ
			mainFuncFlag = true;
			getNextWord();
			test({ lParent }, { rParent }, 3);
			funcBody(voidTyp, "main");
		}
		else skip({ intSy, charSy, voidSy, lBrace }, 22);//����main��������void����
	}
	else {
		//�������Ǳ�ʶ������main
		skip({ ident, mainSy, lBrace }, 23);
	}
}
//�������
void statements() {
	if (curWord.sy == ifSy) {//�������
		ifstatement();
	}
	else if (curWord.sy == forSy) {//ѭ�����
		forstatement();
	}
	else if (curWord.sy == lBrace) {//�����
		getNextWord();
		statementlist();
		test({ rBrace }, STMT_FIRST, 7);
	}
	else if (curWord.sy == ident) {//�����Ǹ�ֵ��䣬�з���ֵ�������ã��޷���ֵ��������
		string ident_save = curWord.value;
		int search_result = globalSearch(ident_save);
		if (search_result == 0)
			skip({ semicolon }, 0);//δ����
		types ident_type = symbolTable[search_result].type;//��ʶ������
		getNextWord();
		//�ж��ǲ��ǵȺš��������Ż���������
		int tempReg;
		if (curWord.sy == assign) { //Ӧ���Ǹ�ֵ���
			if (!(symbolTable[search_result].objTyp == parameter ||
				symbolTable[search_result].objTyp == variable)) {
				error(34);//���ɱ���ֵ
			}
			getNextWord();
			expression(tempReg);
#ifdef CHECK_INT_TO_CHAR
			if (ident_type == charTyp && (!isExprChar)) error(37);//���Ͳ���ȷ������ע��
#endif
																  //ֻ��Խ�int��ֵ��char�ı��������char��ֵ��int��Ȼ�ǿ��Ե�
			quadCodeTable.back().result = ident_save;
		}
		else if (curWord.sy == lBracket) {//������Ԫ�ظ�ֵ
			if (symbolTable[search_result].objTyp != arrayTyp) {
				skip({ semicolon }, 35);//�������飬����ȡ�±�
				return;
			}
			getNextWord();
			exprVal = -1;
			isExprStatic = false;
			expression(tempReg);//��ȡ�����±���ʽ
			if (isExprStatic && (isExprChar || exprVal < 0 || exprVal >= symbolTable[search_result].dimen)) {
				skip({ semicolon }, 40);//����Ǿ�̬���ʽ�������±곬����Χ�����±���char���ͣ���ô����
				return;
			}
			int save_reg = tempReg;
			test({ rBracket }, { assign }, 6);
			test({ assign }, EXPR_FIRST, 11);
			expression(tempReg);
#ifdef CHECK_INT_TO_CHAR
			if (ident_type == charTyp && (!isExprChar)) error(37);//���Ͳ���ȷ������
#endif
			assign_arr_quad(ident_save, buildRegName(save_reg), buildRegName(tempReg));//---Ϊ����Ԫ�ظ�ֵ
		}
		else if (curWord.sy == lParent) {//Ӧ���Ǻ���������䣬���޷���ֵ����
			if (symbolTable[search_result].objTyp != function)
				skip({ semicolon }, 36);//���Ǻ��������ܵ���
			getNextWord();
			valueParam(ident_save);
			int tot;//ռλ������ʵ������
			funcCall(ident_save, tot, false);
			test({ rParent }, { semicolon }, 4);
		}
		test({ semicolon }, STMT_FIRST, 10);
	}
	else if (curWord.sy == scanfSy) {//�����
		readstatement();
		test({ semicolon }, STMT_FIRST, 10);
	}
	else if (curWord.sy == printfSy) {//д���
		writestatement();
		test({ semicolon }, STMT_FIRST, 10);
	}
	else if (curWord.sy == semicolon) {//����䣬ֱ�������ֺ�
		getNextWord();
	}
	else if (curWord.sy == switchSy) {//������
		switchstatement();
	}
	else if (curWord.sy == returnSy) {//�������
		returnstatement();
		test({ semicolon }, STMT_FIRST, 10);
	}
	else {
		skip({ rBrace }, 21);
	}
}
//���������
void casetable(int exprReg, int& linkLabel, int exitLabel) {
	set<int> labelValue;//�洢��ǩ��ֵ�������ظ���ǩ
	types caseLabelTyp = noTyp;//��ǩ����
	types switchTyp = isExprChar ? charTyp : intTyp;
	string con;
	placeLabel(linkLabel);//���õ�һ������ı�ǩ
	test({ caseSy }, { intCon, charCon }, 24);
	if (curWord.sy == plusSy || curWord.sy == minusSy) {//�����'+'����'-'����ô�жϽ������Ƿ�Ϊ����
		caseLabelTyp = intTyp;
		con += curWord.value;
		getNextWord();
		con += curWord.value;
		test({ intCon }, { colon }, 25);//������ֻ����һ������
	}
	else {
		con += curWord.value;
		caseLabelTyp = curWord.sy == intCon ? intTyp : charTyp;
		test({ intCon,charCon }, { colon }, 25);//��������������ǲ���'+'����'-'�������Լ��ַ�����
	}
	labelValue.insert(getIntValue(false, con));//����ǩ��ֵ���뼯��
	if (caseLabelTyp != switchTyp)
		error(37);//���Ͳ��Ա���
	test({ colon }, STMT_FIRST, 9);
	int nextLabel;
	case_cond_quad(exprReg, con, nextLabel);
	statements();
	jmp_quad(exitLabel, false);//�����ִ����ϣ�����switch
	while (curWord.sy == caseSy) {//����������������
		caseLabelTyp = noTyp;
		placeLabel(nextLabel);
		getNextWord();
		con.clear();//��ձ�ʾ�������ַ���
		if (curWord.sy == plusSy || curWord.sy == minusSy) {//�����'+'����'-'����ô�жϽ������Ƿ�Ϊ����
			caseLabelTyp = intTyp;
			con += curWord.value;
			getNextWord();
			con += curWord.value;
			test({ intCon }, { colon }, 25);//������ֻ����һ������
		}
		else {
			caseLabelTyp = curWord.sy == intCon ? intTyp : charTyp;
			con += curWord.value;
			test({ intCon,charCon }, { colon }, 25);//��������������ǲ���'+'����'-'�������Լ��ַ�����
		}
		if (caseLabelTyp != switchTyp) error(37);//�������Ͳ�ƥ��
		if (labelValue.find(getIntValue(false, con)) != labelValue.end()) {//��ǩֵ�ظ�������
			error(39);
		}
		labelValue.insert(getIntValue(false, con));
		test({ colon }, STMT_FIRST, 9);
		case_cond_quad(exprReg, con, nextLabel);
		statements();
		jmp_quad(exitLabel, false);
	}
	linkLabel = nextLabel;//���linkLabel���ݳ�ȥ����default���ʹ��
}
//����ȱʡ��䣬ֻ����switch����г���
void defaultstatement(int linkLabel, int exitLabel) {
	test({ defaultSy }, { colon }, 26);
	test({ colon }, STMT_FIRST, 9);
	placeLabel(linkLabel);
	statements();
	jmp_quad(exitLabel, false);//��ת������
}
//����switch��䣬��ǰ����Ϊswitch
void switchstatement() {
	int tempReg;//��ű��ʽ�������ʱ�Ĵ���
	getNextWord();
	test({ lParent }, EXPR_FIRST, 3);//������ȱʧ�ݴ�
	expression(tempReg);
	test({ rParent }, { lBrace, caseSy }, 4);//������ȱʧ�ݴ�
	test({ lBrace }, { caseSy,defaultSy,rBrace }, 7);//�������ȱʧ�ݴ�
	int exitLabel = genLabel();//���ɽ�����label
	int linkLabel = genLabel();//���ɵ�һ�������label
	casetable(tempReg, linkLabel, exitLabel);//���������
	defaultstatement(linkLabel, exitLabel);//����ȱʡ���
	placeLabel(exitLabel);//���ý������
	test({ rBrace }, STMT_FIRST, 8);//�Ҵ�����ȱʧ�ݴ�
}
//��������䣬��ǰ����Ϊreturn
void returnstatement() {
	int tempReg;//��ʱ�Ĵ���
	getNextWord();
	if (curWord.sy == semicolon) {//ֱ���Ƿֺţ�Ӧ����void���ͺ���
		if (curFuncType != voidSy) error(37);//��void���ͺ������ܷ��ؿ�
		return_quad("");
		return;
	}
	else if (curWord.sy == lParent) {
		if (curFuncType == voidSy) error(37);//�������ʹ���void���������з���ֵ
		getNextWord();
		expression(tempReg);
		if (isExprChar && curFuncType == intSy) //int�������ܷ���char����
			error(37);
		else if ((!isExprChar) && curFuncType == charSy) //char�������ܷ���int����
			error(37);
		funcRet = true;//ȷ����ǰ�����з������
		return_quad(buildRegName(tempReg));
		test({ rParent }, { semicolon }, 4);
	}
	else error(32);//��������г��ַǷ�����
}
//�������䣬��ǰ��scanf
void readstatement() {
	getNextWord();
	test({ lParent }, { ident }, 3);//û����������������Ѱ�ұ�ʶ������������
	scanf_quad(curWord.value);//���뵥����ʶ��
	test({ ident }, { rParent, comma, semicolon }, 2);//û�б�ʶ����������Ѱ�������ţ���ʶ��������
	while (curWord.sy == comma) {
		getNextWord();
		scanf_quad(curWord.value);
		test({ ident }, { rParent, comma, semicolon }, 2);
	}
	test({ rParent }, { semicolon }, 4);
}
//����д��䣬��ǰ��printf
void writestatement() {
	int tempReg;
	getNextWord();
	//���ü�⼯��Ϊ�����ź���ķ��ţ��������ʽfirst�����Լ��ַ�����������
	set<int> checkLparent = EXPR_FIRST;
	checkLparent.insert(strCon);
	test({ lParent }, checkLparent, 3);
	if (curWord.sy == strCon) {//�ַ���
		int lastStrIdx = stringTable.size() - 1;
		getNextWord();//��ȡ��һ������
		if (curWord.sy == comma) {//�ַ�����������ʽ
			getNextWord();
			expression(tempReg);
			printf_quad(lastStrIdx, true, false);//����ַ���
			printf_quad(tempReg, false, isExprChar);//������ʽ
		}
		else {
			printf_quad(lastStrIdx, true, false);//����ַ���
		}
	}
	else {
		expression(tempReg);
		printf_quad(tempReg, false, isExprChar);//������ʽ
	}
	test({ rParent }, { semicolon }, 4);
	println_quad();//���ɻ���
}
//����for��䣬��ǰ����Ϊfor
void forstatement() {
	int tempReg, forLabel;
	string ident_save;
	getNextWord();
	test({ lParent }, { ident }, 3);//©����С���Ż�����С����ǰ�зǷ������ݴ�
	if (curWord.sy == ident) {//����Ƿ�Ϊ��ʶ��
		ident_save = curWord.value;
		if (globalSearch(curWord.value) == 0)
			error(0);//δ����
		getNextWord();
	}
	else
		skip({ ident }, 2);
	set<int> assign_check = EXPR_FIRST;
	assign_check.insert(semicolon);
	test({ assign }, assign_check, 11);//ȱ�ٸ�ֵ�����ݴ�
	expression(tempReg);
	assign_quad(buildRegName(tempReg), ident_save);//Ϊѭ����������ֵ,e.g. i=0
	test({ semicolon }, {}, 10);//�ֺ�֮ǰ�쳣�ַ��ݴ�
	forLabel = genLabel();//����forѭ��Ψһ��һ�����
	placeLabel(forLabel);//����forѭ���ı��

	int saveCursor = cursor;
	word saveCurWord = curWord;
	while (curWord.sy != semicolon && cursor < TOTAL_LENGTH) //���϶�ȡ��ֱ����ȡ�ֺ�Ϊֹ
		getNextWord();
	test({ semicolon }, { ident }, 10);//�ֺ�ǰ�쳣�ַ�����ȱʧ�ֺ��ݴ�

	string lIdent = curWord.value;//��������
	test({ ident }, { assign }, 2);
	test({ assign }, { ident }, 10);//��ֵ����
	string rIdent = curWord.value;//��������ֵ��ʶ��
	test({ ident }, { plusSy, minusSy }, 2);
	if (curWord.sy == plusSy || curWord.sy == minusSy) {
		string op = curWord.value;//��ȡ��ǰ��������ֻ��Ϊ+����-
		getNextWord();
		string step = curWord.value;//��ȡ����
		test({ intCon }, { rParent }, 13);//�������ݴ�������
		test({ rParent }, { STMT_FIRST }, 4);//�����ţ��ݴ������ʼ�ַ�
		statements();
		stepAssign(op, rIdent, step, lIdent);//���¼�������
		int tempSaveCursor = cursor;//���浱ǰ�ַ��±�͵�ǰ��word
		word tempSaveCurWord = curWord;
		curWord = saveCurWord;//�ص������ж�����
		cursor = saveCursor;
		condition(forLabel, false);//����������Ϊ�����ת
		curWord = tempSaveCurWord;//�ָ���ǰ�ַ��±�͵�ǰword
		cursor = tempSaveCursor;
	}
	else error(12);
}
//��������������������if�Լ�for���
void condition(int& newLabel, bool branchIfFalse) {//�Ȳ���һ������������������������newLabel
	int tempReg, saveReg;
	string relOp;
	expression(tempReg);
	saveReg = tempReg;
	if (curWord.sy == gtr || curWord.sy == geq || curWord.sy == lss
		|| curWord.sy == leq || curWord.sy == neq || curWord.sy == eql) {
		relOp = curWord.value;
		getNextWord();
		expression(tempReg);
		if (!branchIfFalse) {//��ǰ����Ϊ�����ת
			relOp = relOp == "==" ? "!=" :
				relOp == "!=" ? "==" :
				relOp == ">" ? "<=" :
				relOp == "<" ? ">=" :
				relOp == ">=" ? "<" :
				">";
		}
		cond_quad(relOp, buildRegName(saveReg), buildRegName(tempReg), newLabel, branchIfFalse);//���������ж�
	}
	else {
		relOp = "!=";
		if (!branchIfFalse) {//��ǰ����Ϊ�����ת
			relOp = "==";
		}
		cond_quad(relOp, buildRegName(saveReg), "0", newLabel, branchIfFalse);//��Ϊ0��ô��Ϊ��
	}
}
//����if��䣬��ǰ��if
void ifstatement() {
	int label1, label2;
	getNextWord();
	test({ lParent }, EXPR_FIRST, 3);//(
	condition(label1, true);//����������label1��else���Ŀ�ʼ���
	test({ rParent }, STMT_FIRST, 4);//)
	statements();//�������
	jmp_quad(label2, true);
	placeLabel(label1);
	if (curWord.sy == elseSy) {
		getNextWord();
		statements();
	}
	placeLabel(label2);
}
//���������
void statementlist() {
	//����а���0���߶������
	while (curWord.sy != rBrace && cursor < TOTAL_LENGTH) {
		if (STMT_FIRST.find(curWord.sy) == STMT_FIRST.end()) {
			skip({ semicolon }, 21);//��ת���ֺ�
		}
		statements();
	}
}
// �������    ������    ::= �ۣ�����˵�����ݣۣ�����˵������{���з���ֵ�������壾|���޷���ֵ�������壾}����������
void program() {
	int funcDeclared = 0;//���λ����ʾ�����Ƿ���������Ʊ��������뺯�������˳��
	getNextWord();
	if (cursor >= TOTAL_LENGTH) return;
	// ����������
	while (curWord.sy == constSy)
		constdec();
	// ����ֻ�����ں���δ����֮ǰ���֣����򱨴�
	while ((curWord.sy == intSy || curWord.sy == charSy) && funcDeclared == 0)
		variabledec(funcDeclared);
	// �������壬����������
	while ((curWord.sy == intSy || curWord.sy == charSy || curWord.sy == voidSy) && !mainFuncFlag) {
		functiondec();
	}
	if (!mainFuncFlag)//���ȱ��main��������ô����
		error(27);
	else
		//˵����ǰ���з��ţ���main�����������ж����ַ�
		if (curWord.sy != noSym) error(28);
}
//������
int main() {
	printf("Please input your source file's name\n");
	string file_name;// �����ļ���������buffer������
	getline(cin, file_name);
	FILE* infile = fopen(file_name.data(), "r");

#ifdef FILE_OUTPUT
	fout.open("out.txt");//�ɵ���Ԫʽ
#endif // FILE_OUTPUT
#ifdef ASM_OUTPUT
	asmout.open("mips.asm");//mips����
#endif // ASM_OUTPUT
#ifdef NEW_QUAD_OUT
	OptimizedQuad.open("newQuad.txt");//�Ż������Ԫʽ
#endif // NEW_QUAD_OUT
	if (infile != NULL) {
		TOTAL_LENGTH = fread(buffer, sizeof(char), BUFFER_SIZE, infile);
		buffer[TOTAL_LENGTH] = 0;
		init(); // ��ʼ���ؼ��ּ�
		program(); // ��������
#ifdef OUTPUT_QUADCODE
		for (int i = 0; i < quadCodeTable.size(); i++) {
			if (quadCodeTable[i].right != "()") fout << setw(20) << "";
			fout << setw(20) << quadCodeTable[i].op << setw(20) << quadCodeTable[i].left
				<< setw(20) << quadCodeTable[i].right << setw(20) << quadCodeTable[i].result
				<< endl;
		}
#endif
#ifdef OPTIMIZE_ON
		BaseBlock bb;//�������Ż�ģ��
		bb.genBaseBlock();//Ϊȫ����Ԫʽ���ֻ����飬�����������ɺ����飬ÿ���������ٻ������ɻ�����
						  //����������
		vector<quadCode> dagQuad;
		for (int i = 0; i < bb.funcBlocks.size(); i++) {
			bb.funcBlocks[i]->genInOutSet();//�������ڵĻ����齨��in/out��
			for (int j = 0; j < bb.funcBlocks[i]->innerBlocks.size(); j++) {
				bb.funcBlocks[i]->innerBlocks[j]->DAG(i+1);//����ȫ�ֱ���
				for (int k = 0; k < bb.funcBlocks[i]->innerBlocks[j]->newQuads.size(); k++)
					dagQuad.push_back(bb.funcBlocks[i]->innerBlocks[j]->newQuads[k]);
			}
		}
		quadCodeTable.clear();
		for (int i = 0; i < bb.newQuads.size(); i++)//����µ�ȫ��������Ԫʽ
			quadCodeTable.push_back(bb.newQuads[i]);
		for (int i = 0; i < dagQuad.size(); i++)//��Ӹ��������Ļ�������Ԫʽ
			quadCodeTable.push_back(dagQuad[i]);
		graphColorer gc;//ͼȾɫ��
		for (int i = 0; i < bb.funcBlocks.size(); i++) {
			gc.buildInterferenceGraph(bb.funcBlocks[i]);
		}
		CodeGenerator cg(bb.funcBlocks);//����������
		cg.genOptMips();
#else
		NaiveGenerator ng;//δ�Ż��汾
		ng.generateMips();
#endif // OPTIMIZE_ON
		printSymbolTable();
		printBtab();
		fclose(infile);// �ر��ļ�
	}
	else
		printf("File doesn't exist");
#ifdef FILE_OUTPUT
	fout.close();
#endif // FILE_OUTPUT
#ifdef ASM_OUTPUT
	asmout.close();
#endif // ASM_OUTPUT
#ifdef NEW_QUAD_OUT
	OptimizedQuad.close();
#endif // NEW_QUAD_OUT
	return 0;
}
