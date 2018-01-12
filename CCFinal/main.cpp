#include "codeGenerate.h"

//#define CHECK_INT_TO_CHAR // int类型向char类型转换是否要检测
#define OPTIMIZE_ON //是否开启优化
void error(int errNo) { //输出错误信息，包括错误数量统计以及错误所在行数
	errCnt++;
	printf("Error %d : %s at line %d\n", errCnt, errormap[errNo].c_str(), curLineCnt);
}
//跳到jumpset中的符号为止，并且根据errNo报错
void skip(set<int> jumpset, int errNo) {
	error(errNo);
	while (true) {
		if (cursor >= TOTAL_LENGTH) break;
		if (jumpset.find(curWord.sy) != jumpset.end()) break;//如果找到了就停止
		getNextWord();
	}
}
//判断当前符号是否属于target集合，如果不是，跳读到target以及stop符号集合为止
void test(set<int> target, set<int> stop, int errNo) {
	if (target.find(curWord.sy) == target.end()) {//target中没有当前符号
		target.insert(stop.begin(), stop.end());
		skip(target, errNo);
	}
	else getNextWord();
}
//获取整数常量的值
int getIntValue(bool isNegative, string value) {
	int ret = 0;
	if (value.size() > 0) {
		if (value[0] == '+') {//带+号标签
			for (int i = 1; i < value.size(); i++) ret = ret * 10 + value[i] - '0';
		}
		else if (value[0] == '-') {//带-号标签
			for (int i = 1; i < value.size(); i++) ret = ret * 10 + value[i] - '0';
			ret = -ret;
		}
		else if (value.size() > 1 && value[0] == '\'')  //字符常量
			ret = value[1];
		else //正常数字
			for (int i = 0; i < value.size(); i++) ret = ret * 10 + value[i] - '0';
		return isNegative ? -ret : ret;
	}
	else return 0;
}
//处理值参数列表
void valueParam(string funcName) {
	int tempReg, paramNum = 0, paramIdx;
	int funcIdx = globalSearch(funcName);
	paramIdx = funcIdx;
	while (curWord.sy != rParent) {
		expression(tempReg);
		push_param(tempReg);
		paramIdx++;
		if (symbolTable[paramIdx].objTyp == parameter) {//只判断是参数的情况
			if (symbolTable[paramIdx].type == charTyp && !isExprChar)//参数类型与表达式类型不匹配
				error(37);
			else if (symbolTable[paramIdx].type == intTyp && isExprChar)//参数类型与表达式类型不匹配
				error(37);
		}
		paramNum++;
		while (curWord.sy == comma) {
			getNextWord();
			expression(tempReg);
			push_param(tempReg);
			paramIdx++;
			if (symbolTable[paramIdx].objTyp == parameter) {//只判断是参数的情况
				if (symbolTable[paramIdx].type == charTyp && !isExprChar)//参数类型与表达式类型不匹配
					error(37);
				else if (symbolTable[paramIdx].type == intTyp && isExprChar)//参数类型与表达式类型不匹配
					error(37);
			}
			paramNum++;
		}
	}
	if (symbolTable[funcIdx].dimen != paramNum)
		error(38);//报错，参数不匹配
}
//处理因子
void factor(int& regId) {
	static int regCnt = 0;
	int tempBool = false;
	int tempReg;
	if (curWord.sy == ident) {//标识符
		int search_result = globalSearch(curWord.value);
		if (search_result == 0) {
			skip({ comma,semicolon }, 0);//未定义直接报错
			return;
		}
		if (search_result != 0 &&
			symbolTable[search_result].type == charTyp) {
			//如果在符号表中该标识符登记的是char类型，
			//那么无论是char数组还是char函数或者简单的char变常量，返回的都是字符型
			tempBool = true;
		}
		string ident_save = curWord.value;
		getNextWord();
		if (curWord.sy == lBracket) {//数组元素
			if (symbolTable[search_result].objTyp != arrayTyp) {
				skip({ semicolon }, 35); //不是数组，不可取下标
				return;
			}
			getNextWord();
			exprVal = -1;
			isExprStatic = false;
			expression(tempReg);//表达式的值存放在tempReg中
			if (isExprStatic && (isExprChar || exprVal < 0 || exprVal >= symbolTable[search_result].dimen)) {
				//如果下标为静态并且求出的值超出范围，或者下标类型就是char类型，那么报错
				skip({ semicolon }, 40); //数组下标超过范围
				return;
			}
			leftOperand = ident_save;//分配左右操作符
			rightOperand = buildRegName(tempReg);
			calcu_quad("=[]", -1, -1, regId);//生成四元式数组取值,	=[],a,i,result
			test({ rBracket }, EXPR_FOLLOW, 6);
		}
		else if (curWord.sy == lParent) {//有返回值函数调用
			if (symbolTable[search_result].objTyp != function)
				error(36);//不是函数，不能调用
			if (symbolTable[search_result].type == voidTyp)
				error(37);//为void类型，不能产生返回值
			getNextWord();
			valueParam(ident_save);
			funcCall(ident_save, regId, true);
			test({ rParent }, EXPR_FOLLOW, 4);
		}
		else {//否则只是一个单标识符，在四元式表达中不分配寄存器，直接用标识符名
			if (symbolTable[search_result].objTyp != variable &&//单个标识符只能是变量、参数或者常量，而函数、数组类型在这里不允许
				symbolTable[search_result].objTyp != parameter &&
				symbolTable[search_result].objTyp != constant) {
				error(37);
			}
			regId = -1;//不分配寄存器
			operand = ident_save;
		}
		isExprStatic = false;//暂时将静态值标志设为false
		isExprChar = tempBool;
		if (symbolTable[search_result].objTyp == constant) {
			exprVal = symbolTable[search_result].value;//如果是常量就直接取其值
			isExprStatic = true;//常量则可以设置为静态
		}
	}
	else if (curWord.sy == charCon || curWord.sy == intCon) {
		isExprChar = curWord.sy == charCon;
		exprVal = getIntValue(false, curWord.value);//更新表达式的值
		isExprStatic = true;
		regId = -1;//常量也不分配寄存器
		operand = curWord.value;
		getNextWord();
	}
	else if (curWord.sy == plusSy || curWord.sy == minusSy) {
		string op = curWord.sy == minusSy ? "-" : "+";
		getNextWord();
		if (curWord.sy == intCon) {
			leftOperand = "0";
			rightOperand = curWord.value;
			exprVal = getIntValue(false, op + curWord.value);//更新表达式的值
			isExprStatic = true;
			isExprChar = false;
			calcu_quad(op, -1, -1, regId);//产生#regi = 0-2类型的运算
			getNextWord();
		}
		else test({ intCon }, EXPR_FOLLOW, 13);
	}
	else if (curWord.sy == lParent) {
		getNextWord();
		expression(tempReg);
		test({ rParent }, EXPR_FOLLOW, 4);
		regId = tempReg;//直接使用表达式的返回寄存器
	}
	else {
		skip(EXPR_FOLLOW, 15);
	}
}
//处理项
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
		if (tempIsExprStatic) tempExprVal = op == "*" ? tempExprVal * exprVal : tempExprVal / exprVal;//求出表达式的值
		if (tempReg == -1) rop = operand;//保存操作数
		leftOperand = lop;
		rightOperand = rop;
		calcu_quad(op, regId, tempReg, regId);
		isExprChar = false;//参与了其他运算，不是char类型
	}
	exprVal = tempExprVal;
	isExprStatic = tempIsExprStatic;
}
//处理表达式，当前符号为+,-或者标识符,整数,单引号(表示字符),左括号
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
	if (isNegtive) {//处理第一项的正负号
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
		if (tempIsExprStatic) tempExprVal = op == "+" ? tempExprVal + exprVal : tempExprVal - exprVal;//求出表达式的值
		if (tempReg == -1) rop = operand;//保存操作数
		leftOperand = lop;
		rightOperand = rop;
		calcu_quad(op, regId, tempReg, regId);
		isExprChar = false;//参与了运算，不是char类型
	}
	if (regId == -1) {//如果regId还是没有被分配，那么强行分配一个寄存器
		leftOperand = lop;
		rightOperand = "";
		calcu_quad("=", -1, -1, regId);
	}
	exprVal = tempExprVal;//最后将临时的表达式的值赋给exprVal
	isExprStatic = tempIsExprStatic;
}
//处理函数参数
void param() {
	if (curWord.sy == rParent) {//参数表为空，直接结束
		return;
	}
	else if (curWord.sy == intSy || curWord.sy == charSy) {//参数表不为空，开始处理参数
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
//处理单个常量定义,    ＜常量定义＞ ::=  int＜标识符＞＝＜整数＞{,＜标识符＞＝＜整数＞} | char＜标识符＞＝＜字符＞{ ,＜标识符＞＝＜字符＞ }
void singleConstDef(symbol intOrchar) {
	string name = curWord.value;//标识符名字
	if (intOrchar == intSy) {
		//是否为负数
		bool isNegative = false;
		// 接下来是标识符
		test({ ident }, { comma, semicolon }, 2);
		// 标识符后面是赋值符号=
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
		// 标识符后面是赋值符号=
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
//处理常量声明，包含常量定义,  ＜常量说明＞ ::=  const＜常量定义＞;{ const＜常量定义＞;}
void constdec() {
	getNextWord();//当前已经是const
				  // 以int或者char开头
	if (curWord.sy == intSy || curWord.sy == charSy) {
		//保存当前符号
		symbol save_type = curWord.sy;
		getNextWord();
		singleConstDef(save_type);//处理单个常量定义
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
		//跳转到分号，const，int，char，void或者右大括号为止
		skip(CONST_FOLLOW, 10);
		if (endSym.find(curWord.sy) != endSym.end()) getNextWord();
	}
}
//复合语句处理
void compoundStatement() {
	int placeholder = 0;//占位符，无实际意义
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
//处理函数体，当前已经处理完了左括号，准备进入参数
void funcBody(types funcType, string funcName) {
	param();//处理函数参数
	test({ rParent }, { lBrace }, 4);
	test({ lBrace }, COMP_STMT_FIRST, 7);
	curFuncType = funcType == intTyp ? intSy : (funcType == charTyp ? charSy : voidSy);
	funcRet = false;//初始设定当前函数没有返回值
	compoundStatement();//处理复合语句
	if (curFuncType != voidSy && funcRet == false) //当前函数不是void类型却没有返回语句，报错
		error(37);
	curFuncType = noSym;//复位
	test({ rBrace }, { intSy, charSy, voidSy }, 8);
	func_exit(funcName);//退出函数
}
//处理变量声明，当前识别到int或者char
void variabledec(int &funcDeclared) {
	//当前这些变量的类型，为intsy或者charsy
	types c_type = curWord.sy == intSy ? intTyp : charTyp;
	int flag = 0;
	getNextWord();
	//当前字符需要是标识符
	while (curWord.sy == ident) {
		string save_ident = curWord.value;
		getNextWord();
		flag = 1;
		if (curWord.sy == comma) {//遇到逗号
			var_insert(save_ident, c_type, variable, 0);//插入变量
			getNextWord();
		}
		else if (curWord.sy == semicolon) {//遇到分号
			var_insert(save_ident, c_type, variable, 0);//插入变量
			getNextWord();
			break;
		}
		else if (curWord.sy == lBracket) {//遇到左中括号，可能是数组
			getNextWord();//得到数字
			int dimen = getIntValue(false, curWord.value);//数组维数
			getNextWord();//得到右中括号
			test({ rBracket }, VAR_FOLLOW, 6);
			var_insert(save_ident, c_type, arrayTyp, dimen);//插入变量
			symbolTable.back().dimen = dimen;//更新数组维数
			if (curWord.sy == comma) getNextWord();
			else if (curWord.sy == semicolon) {
				getNextWord();
				break;
			}
			else error(19);
		}
		else if (curWord.sy == lParent) {//遇到左小括号，可能是函数
			funcDeclared = 1;
			func_insert(save_ident, c_type);//函数声明开始
			getNextWord();
			funcBody(c_type, save_ident);//输入类型与函数名,处理函数体
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
//处理函数定义，当前识别到void,int或者char
void functiondec() {
	symbol savesymbol = curWord.sy;
	types save_type = curWord.sy == intSy ? intTyp : (curWord.sy == charSy ? charTyp : voidTyp);
	getNextWord();
	string funcName = curWord.value;
	if (curWord.sy == ident) {
		func_insert(funcName, save_type);//函数声明开始
		getNextWord();
		test({ lParent }, { intSy, charSy }, 3);
		funcBody(save_type, funcName);
	}
	else if (curWord.sy == mainSy) {
		if (savesymbol == voidSy) {
			func_insert(curWord.value, save_type);//插入函数声明四元式
			mainFuncFlag = true;
			getNextWord();
			test({ lParent }, { rParent }, 3);
			funcBody(voidTyp, "main");
		}
		else skip({ intSy, charSy, voidSy, lBrace }, 22);//报错，main函数不是void类型
	}
	else {
		//报错，不是标识符或者main
		skip({ ident, mainSy, lBrace }, 23);
	}
}
//处理语句
void statements() {
	if (curWord.sy == ifSy) {//条件语句
		ifstatement();
	}
	else if (curWord.sy == forSy) {//循环语句
		forstatement();
	}
	else if (curWord.sy == lBrace) {//语句列
		getNextWord();
		statementlist();
		test({ rBrace }, STMT_FIRST, 7);
	}
	else if (curWord.sy == ident) {//可能是赋值语句，有返回值函数调用，无返回值函数调用
		string ident_save = curWord.value;
		int search_result = globalSearch(ident_save);
		if (search_result == 0)
			skip({ semicolon }, 0);//未定义
		types ident_type = symbolTable[search_result].type;//标识符类型
		getNextWord();
		//判断是不是等号、左中括号或者左括号
		int tempReg;
		if (curWord.sy == assign) { //应该是赋值语句
			if (!(symbolTable[search_result].objTyp == parameter ||
				symbolTable[search_result].objTyp == variable)) {
				error(34);//不可被赋值
			}
			getNextWord();
			expression(tempReg);
#ifdef CHECK_INT_TO_CHAR
			if (ident_type == charTyp && (!isExprChar)) error(37);//类型不正确，报错，注意
#endif
																  //只针对将int赋值给char的报错，如果是char赋值给int当然是可以的
			quadCodeTable.back().result = ident_save;
		}
		else if (curWord.sy == lBracket) {//给数组元素赋值
			if (symbolTable[search_result].objTyp != arrayTyp) {
				skip({ semicolon }, 35);//不是数组，不能取下标
				return;
			}
			getNextWord();
			exprVal = -1;
			isExprStatic = false;
			expression(tempReg);//获取数组下标表达式
			if (isExprStatic && (isExprChar || exprVal < 0 || exprVal >= symbolTable[search_result].dimen)) {
				skip({ semicolon }, 40);//如果是静态表达式，并且下标超过范围或者下标是char类型，那么报错
				return;
			}
			int save_reg = tempReg;
			test({ rBracket }, { assign }, 6);
			test({ assign }, EXPR_FIRST, 11);
			expression(tempReg);
#ifdef CHECK_INT_TO_CHAR
			if (ident_type == charTyp && (!isExprChar)) error(37);//类型不正确，报错
#endif
			assign_arr_quad(ident_save, buildRegName(save_reg), buildRegName(tempReg));//---为数组元素赋值
		}
		else if (curWord.sy == lParent) {//应该是函数调用语句，有无返回值均可
			if (symbolTable[search_result].objTyp != function)
				skip({ semicolon }, 36);//不是函数，不能调用
			getNextWord();
			valueParam(ident_save);
			int tot;//占位符，无实际意义
			funcCall(ident_save, tot, false);
			test({ rParent }, { semicolon }, 4);
		}
		test({ semicolon }, STMT_FIRST, 10);
	}
	else if (curWord.sy == scanfSy) {//读语句
		readstatement();
		test({ semicolon }, STMT_FIRST, 10);
	}
	else if (curWord.sy == printfSy) {//写语句
		writestatement();
		test({ semicolon }, STMT_FIRST, 10);
	}
	else if (curWord.sy == semicolon) {//空语句，直接跳过分号
		getNextWord();
	}
	else if (curWord.sy == switchSy) {//情况语句
		switchstatement();
	}
	else if (curWord.sy == returnSy) {//返回语句
		returnstatement();
		test({ semicolon }, STMT_FIRST, 10);
	}
	else {
		skip({ rBrace }, 21);
	}
}
//处理情况表
void casetable(int exprReg, int& linkLabel, int exitLabel) {
	set<int> labelValue;//存储标签的值，避免重复标签
	types caseLabelTyp = noTyp;//标签类型
	types switchTyp = isExprChar ? charTyp : intTyp;
	string con;
	placeLabel(linkLabel);//放置第一个情况的标签
	test({ caseSy }, { intCon, charCon }, 24);
	if (curWord.sy == plusSy || curWord.sy == minusSy) {//如果是'+'或者'-'，那么判断接下来是否为整数
		caseLabelTyp = intTyp;
		con += curWord.value;
		getNextWord();
		con += curWord.value;
		test({ intCon }, { colon }, 25);//接下来只能是一个整数
	}
	else {
		con += curWord.value;
		caseLabelTyp = curWord.sy == intCon ? intTyp : charTyp;
		test({ intCon,charCon }, { colon }, 25);//否则接下来可能是不带'+'或者'-'的整数以及字符常量
	}
	labelValue.insert(getIntValue(false, con));//将标签的值插入集合
	if (caseLabelTyp != switchTyp)
		error(37);//类型不对报错
	test({ colon }, STMT_FIRST, 9);
	int nextLabel;
	case_cond_quad(exprReg, con, nextLabel);
	statements();
	jmp_quad(exitLabel, false);//此情况执行完毕，结束switch
	while (curWord.sy == caseSy) {//处理更多的情况子语句
		caseLabelTyp = noTyp;
		placeLabel(nextLabel);
		getNextWord();
		con.clear();//清空表示常量的字符串
		if (curWord.sy == plusSy || curWord.sy == minusSy) {//如果是'+'或者'-'，那么判断接下来是否为整数
			caseLabelTyp = intTyp;
			con += curWord.value;
			getNextWord();
			con += curWord.value;
			test({ intCon }, { colon }, 25);//接下来只能是一个整数
		}
		else {
			caseLabelTyp = curWord.sy == intCon ? intTyp : charTyp;
			con += curWord.value;
			test({ intCon,charCon }, { colon }, 25);//否则接下来可能是不带'+'或者'-'的整数以及字符常量
		}
		if (caseLabelTyp != switchTyp) error(37);//报错，类型不匹配
		if (labelValue.find(getIntValue(false, con)) != labelValue.end()) {//标签值重复，报错
			error(39);
		}
		labelValue.insert(getIntValue(false, con));
		test({ colon }, STMT_FIRST, 9);
		case_cond_quad(exprReg, con, nextLabel);
		statements();
		jmp_quad(exitLabel, false);
	}
	linkLabel = nextLabel;//最后将linkLabel传递出去留给default语句使用
}
//处理缺省语句，只会在switch语句中出现
void defaultstatement(int linkLabel, int exitLabel) {
	test({ defaultSy }, { colon }, 26);
	test({ colon }, STMT_FIRST, 9);
	placeLabel(linkLabel);
	statements();
	jmp_quad(exitLabel, false);//跳转到结束
}
//处理switch语句，当前符号为switch
void switchstatement() {
	int tempReg;//存放表达式结果的临时寄存器
	getNextWord();
	test({ lParent }, EXPR_FIRST, 3);//左括号缺失容错
	expression(tempReg);
	test({ rParent }, { lBrace, caseSy }, 4);//右括号缺失容错
	test({ lBrace }, { caseSy,defaultSy,rBrace }, 7);//左大括号缺失容错
	int exitLabel = genLabel();//生成结束的label
	int linkLabel = genLabel();//生成第一个情况的label
	casetable(tempReg, linkLabel, exitLabel);//处理情况表
	defaultstatement(linkLabel, exitLabel);//处理缺省情况
	placeLabel(exitLabel);//放置结束标号
	test({ rBrace }, STMT_FIRST, 8);//右大括号缺失容错
}
//处理返回语句，当前符号为return
void returnstatement() {
	int tempReg;//临时寄存器
	getNextWord();
	if (curWord.sy == semicolon) {//直接是分号，应该是void类型函数
		if (curFuncType != voidSy) error(37);//非void类型函数不能返回空
		return_quad("");
		return;
	}
	else if (curWord.sy == lParent) {
		if (curFuncType == voidSy) error(37);//返回类型错误，void函数不能有返回值
		getNextWord();
		expression(tempReg);
		if (isExprChar && curFuncType == intSy) //int函数不能返回char类型
			error(37);
		else if ((!isExprChar) && curFuncType == charSy) //char函数不能返回int类型
			error(37);
		funcRet = true;//确定当前函数有返回语句
		return_quad(buildRegName(tempReg));
		test({ rParent }, { semicolon }, 4);
	}
	else error(32);//返回语句中出现非法符号
}
//处理读语句，当前是scanf
void readstatement() {
	getNextWord();
	test({ lParent }, { ident }, 3);//没有左括号试着往后寻找标识符或者左括号
	scanf_quad(curWord.value);//输入单个标识符
	test({ ident }, { rParent, comma, semicolon }, 2);//没有标识符试着往后寻找右括号，标识符，逗号
	while (curWord.sy == comma) {
		getNextWord();
		scanf_quad(curWord.value);
		test({ ident }, { rParent, comma, semicolon }, 2);
	}
	test({ rParent }, { semicolon }, 4);
}
//处理写语句，当前是printf
void writestatement() {
	int tempReg;
	getNextWord();
	//设置检测集合为左括号后面的符号，包括表达式first集合以及字符串常量符号
	set<int> checkLparent = EXPR_FIRST;
	checkLparent.insert(strCon);
	test({ lParent }, checkLparent, 3);
	if (curWord.sy == strCon) {//字符串
		int lastStrIdx = stringTable.size() - 1;
		getNextWord();//获取下一个符号
		if (curWord.sy == comma) {//字符串后面跟表达式
			getNextWord();
			expression(tempReg);
			printf_quad(lastStrIdx, true, false);//输出字符串
			printf_quad(tempReg, false, isExprChar);//输出表达式
		}
		else {
			printf_quad(lastStrIdx, true, false);//输出字符串
		}
	}
	else {
		expression(tempReg);
		printf_quad(tempReg, false, isExprChar);//输出表达式
	}
	test({ rParent }, { semicolon }, 4);
	println_quad();//生成换行
}
//处理for语句，当前符号为for
void forstatement() {
	int tempReg, forLabel;
	string ident_save;
	getNextWord();
	test({ lParent }, { ident }, 3);//漏掉左小括号或者左小括号前有非法符号容错
	if (curWord.sy == ident) {//检测是否为标识符
		ident_save = curWord.value;
		if (globalSearch(curWord.value) == 0)
			error(0);//未定义
		getNextWord();
	}
	else
		skip({ ident }, 2);
	set<int> assign_check = EXPR_FIRST;
	assign_check.insert(semicolon);
	test({ assign }, assign_check, 11);//缺少赋值符号容错
	expression(tempReg);
	assign_quad(buildRegName(tempReg), ident_save);//为循环变量赋初值,e.g. i=0
	test({ semicolon }, {}, 10);//分号之前异常字符容错
	forLabel = genLabel();//生成for循环唯一的一个标号
	placeLabel(forLabel);//放置for循环的标号

	int saveCursor = cursor;
	word saveCurWord = curWord;
	while (curWord.sy != semicolon && cursor < TOTAL_LENGTH) //不断读取，直到读取分号为止
		getNextWord();
	test({ semicolon }, { ident }, 10);//分号前异常字符或者缺失分号容错

	string lIdent = curWord.value;//步长变量
	test({ ident }, { assign }, 2);
	test({ assign }, { ident }, 10);//赋值符号
	string rIdent = curWord.value;//步长的右值标识符
	test({ ident }, { plusSy, minusSy }, 2);
	if (curWord.sy == plusSy || curWord.sy == minusSy) {
		string op = curWord.value;//获取当前操作符，只能为+或者-
		getNextWord();
		string step = curWord.value;//获取步长
		test({ intCon }, { rParent }, 13);//步长，容错到右括号
		test({ rParent }, { STMT_FIRST }, 4);//右括号，容错到语句起始字符
		statements();
		stepAssign(op, rIdent, step, lIdent);//更新计数变量
		int tempSaveCursor = cursor;//保存当前字符下标和当前的word
		word tempSaveCurWord = curWord;
		curWord = saveCurWord;//回到条件判断那里
		cursor = saveCursor;
		condition(forLabel, false);//这里是条件为真才跳转
		curWord = tempSaveCurWord;//恢复当前字符下标和当前word
		cursor = tempSaveCursor;
	}
	else error(12);
}
//处理条件，条件被用于if以及for语句
void condition(int& newLabel, bool branchIfFalse) {//先测试一下条件，如果不满足跳到标号newLabel
	int tempReg, saveReg;
	string relOp;
	expression(tempReg);
	saveReg = tempReg;
	if (curWord.sy == gtr || curWord.sy == geq || curWord.sy == lss
		|| curWord.sy == leq || curWord.sy == neq || curWord.sy == eql) {
		relOp = curWord.value;
		getNextWord();
		expression(tempReg);
		if (!branchIfFalse) {//当前条件为真才跳转
			relOp = relOp == "==" ? "!=" :
				relOp == "!=" ? "==" :
				relOp == ">" ? "<=" :
				relOp == "<" ? ">=" :
				relOp == ">=" ? "<" :
				">";
		}
		cond_quad(relOp, buildRegName(saveReg), buildRegName(tempReg), newLabel, branchIfFalse);//进行条件判断
	}
	else {
		relOp = "!=";
		if (!branchIfFalse) {//当前条件为真就跳转
			relOp = "==";
		}
		cond_quad(relOp, buildRegName(saveReg), "0", newLabel, branchIfFalse);//不为0那么就为真
	}
}
//处理if语句，当前是if
void ifstatement() {
	int label1, label2;
	getNextWord();
	test({ lParent }, EXPR_FIRST, 3);//(
	condition(label1, true);//处理条件，label1是else语句的开始入口
	test({ rParent }, STMT_FIRST, 4);//)
	statements();//处理语句
	jmp_quad(label2, true);
	placeLabel(label1);
	if (curWord.sy == elseSy) {
		getNextWord();
		statements();
	}
	placeLabel(label2);
}
//处理语句列
void statementlist() {
	//语句列包含0或者多条语句
	while (curWord.sy != rBrace && cursor < TOTAL_LENGTH) {
		if (STMT_FIRST.find(curWord.sy) == STMT_FIRST.end()) {
			skip({ semicolon }, 21);//跳转到分号
		}
		statements();
	}
}
// 处理程序    ＜程序＞    ::= ［＜常量说明＞］［＜变量说明＞］{＜有返回值函数定义＞|＜无返回值函数定义＞}＜主函数＞
void program() {
	int funcDeclared = 0;//标记位，表示函数是否定义过，控制变量声明与函数定义的顺序
	getNextWord();
	if (cursor >= TOTAL_LENGTH) return;
	// 处理常量声明
	while (curWord.sy == constSy)
		constdec();
	// 变量只允许在函数未定义之前出现，否则报错
	while ((curWord.sy == intSy || curWord.sy == charSy) && funcDeclared == 0)
		variabledec(funcDeclared);
	// 函数定义，包括主函数
	while ((curWord.sy == intSy || curWord.sy == charSy || curWord.sy == voidSy) && !mainFuncFlag) {
		functiondec();
	}
	if (!mainFuncFlag)//如果缺少main函数，那么报错
		error(27);
	else
		//说明当前还有符号，即main函数结束后还有多余字符
		if (curWord.sy != noSym) error(28);
}
//主函数
int main() {
	printf("Please input your source file's name\n");
	string file_name;// 读入文件，保存于buffer数组中
	getline(cin, file_name);
	FILE* infile = fopen(file_name.data(), "r");

#ifdef FILE_OUTPUT
	fout.open("out.txt");//旧的四元式
#endif // FILE_OUTPUT
#ifdef ASM_OUTPUT
	asmout.open("mips.asm");//mips代码
#endif // ASM_OUTPUT
#ifdef NEW_QUAD_OUT
	OptimizedQuad.open("newQuad.txt");//优化后的四元式
#endif // NEW_QUAD_OUT
	if (infile != NULL) {
		TOTAL_LENGTH = fread(buffer, sizeof(char), BUFFER_SIZE, infile);
		buffer[TOTAL_LENGTH] = 0;
		init(); // 初始化关键字集
		program(); // 分析程序
#ifdef OUTPUT_QUADCODE
		for (int i = 0; i < quadCodeTable.size(); i++) {
			if (quadCodeTable[i].right != "()") fout << setw(20) << "";
			fout << setw(20) << quadCodeTable[i].op << setw(20) << quadCodeTable[i].left
				<< setw(20) << quadCodeTable[i].right << setw(20) << quadCodeTable[i].result
				<< endl;
		}
#endif
#ifdef OPTIMIZE_ON
		BaseBlock bb;//基本块优化模块
		bb.genBaseBlock();//为全体四元式划分基本块，包括划分若干函数块，每个函数块再划分若干基本块
						  //遍历函数块
		vector<quadCode> dagQuad;
		for (int i = 0; i < bb.funcBlocks.size(); i++) {
			bb.funcBlocks[i]->genInOutSet();//函数块内的基本块建立in/out集
			for (int j = 0; j < bb.funcBlocks[i]->innerBlocks.size(); j++) {
				bb.funcBlocks[i]->innerBlocks[j]->DAG(i+1);//传入全局变量
				for (int k = 0; k < bb.funcBlocks[i]->innerBlocks[j]->newQuads.size(); k++)
					dagQuad.push_back(bb.funcBlocks[i]->innerBlocks[j]->newQuads[k]);
			}
		}
		quadCodeTable.clear();
		for (int i = 0; i < bb.newQuads.size(); i++)//添加新的全局声明四元式
			quadCodeTable.push_back(bb.newQuads[i]);
		for (int i = 0; i < dagQuad.size(); i++)//添加各个函数的基本块四元式
			quadCodeTable.push_back(dagQuad[i]);
		graphColorer gc;//图染色器
		for (int i = 0; i < bb.funcBlocks.size(); i++) {
			gc.buildInterferenceGraph(bb.funcBlocks[i]);
		}
		CodeGenerator cg(bb.funcBlocks);//代码生成器
		cg.genOptMips();
#else
		NaiveGenerator ng;//未优化版本
		ng.generateMips();
#endif // OPTIMIZE_ON
		printSymbolTable();
		printBtab();
		fclose(infile);// 关闭文件
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
