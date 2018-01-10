#ifndef OPTIMIZATION_H
#define OPTIMIZATION_H
#include "lexical.h"
//#define BASEBLOCK_DEBUG	//是否开启基本块调试选项
#define NEW_QUAD_OUT
#ifdef NEW_QUAD_OUT//是否将优化后的四元式输出
ofstream OptimizedQuad;
#endif

class dagTableNode {//dag表节点，即出现在nodeTable中的对象
public:
	string value;//该节点的名字或者值
	int dagId;//该节点对应的dag树节点的编号
};//DAG表节点

class dagTreeNode {//dag树节点
public:
	string value;//树节点对应的字符串
	string op;//树节点的操作符
	int quadId;//此节点对应的四元式索引
	int dagId;//树节点的dagId
	int ref;//引用次数
	dagTreeNode* leftson;
	dagTreeNode* rightson;
	vector<dagTreeNode*> parent;
	dagTreeNode(int _dagId) {
		value = "";
		op = "";
		dagId = _dagId;
		leftson = NULL;
		rightson = NULL;
		ref = 0;
	}
};

class BaseBlock;
//函数块，包含一组基本块
class FuncBaseBlock {
public:
	vector<BaseBlock*> innerBlocks;//属于当前函数块的内部块
								   //生成此函数块内基本块的in，out集合
	map<string, int> id2Greg;//将标识符映射到全局寄存器
	set<string> glbAndParam;//全局变量和参数
	void genInOutSet();
};
//基本块类
class BaseBlock {
public:
	vector<FuncBaseBlock*> funcBlocks;//以函数为单位划分基本块
	vector<quadCode> quadTable;//此基本块所包含的四元式
	vector<quadCode> newQuads;
	vector<dagTableNode*> nodeTable;//包含DAG节点的节点表
	vector<dagTreeNode*> treeNodes;//树节点表
	int dagNodeCnt = 0;//dag树节点计数器
	set<int> prevBlocks;//此基本块的前驱基本块索引
	set<int> nextBlocks;//此基本块的后继基本块索引
	set<string> useSet;//此基本块使用集合
	set<string> defSet;//此基本块定义集合
	set<string> inSet;//此基本块in集合
	set<string> outSet;//此基本块out集合
	set<string> arrSet;//数组集合，存储此基本块中的数组名
	void printQuad(quadCode& qc);
	bool isFuncEnd(quadCode& qc);
	void buildRel(FuncBaseBlock* fbb);
	void genBaseBlock();
	void enterDefSet(string arg);
	void enterUseSet(string arg);
	void genUseDefSet();
	int searchInNodeTable(string name);
	int searchInNodeTable(int dagId);
	int searchInTreeTable(string op, int src1Id, int src2Id);
	int searchInTreeTable(int dagId);
	bool checkDagNodeQueue(vector<dagTreeNode*>& dagNodeQueue);
	bool checkEnqueue(vector<dagTreeNode*>& dagNodeQueue, dagTreeNode* dtn);
	void enterNodeTable(string name);
	int dagNodeQueueFront(vector<dagTreeNode *>& que);//求出要用dag树导出的第一条四元式的索引
	bool isVarGlobal(vector<quadCode>& globalQuad, string& varname);//判断变量是否为全局变量
	void checkFuture(int st, string& result, map<int, vector<int>>& curDagId);
	void DAG(vector<quadCode>& globalQuad);
};
void BaseBlock::printQuad(quadCode& qc) {
	quadCode newqc = qc;
	newQuads.push_back(newqc);
#ifdef NEW_QUAD_OUT
	if (qc.op != "dagBegin") {
		OptimizedQuad << setw(20) << qc.op << setw(20) << qc.left
			<< setw(20) << qc.right << setw(20) << qc.result
			<< endl;
	}
#endif // NEW_QUAD_OUT
}
//将集合a和集合b的结果并起来存入集合c
void _union_set(set<string> a, set<string> b, set<string>&c) {
	c.clear();
	for (set<string>::iterator ite = a.begin(); ite != a.end(); ite++)
		c.insert(*ite);
	for (set<string>::iterator ite = b.begin(); ite != b.end(); ite++)
		c.insert(*ite);
}
//求集合a与b的差集，结果放在c中,c = a-b
void _difference_set(set<string> a, set<string> b, set<string>& c) {
	c.clear();
	for (set<string>::iterator ite = a.begin(); ite != a.end(); ite++)
		if (b.find(*ite) == b.end()) {
			c.insert(*ite);//在a中而不在b中的元素
		}
}
//获取in和out集合
void FuncBaseBlock::genInOutSet() {
	bool changed;
	for (int i = 0; i < innerBlocks.size(); i++) {
		innerBlocks[i]->genUseDefSet();//每个基本块生成use和def集合
		innerBlocks[i]->inSet.clear();//初始化的in和out集合设置为空集
		innerBlocks[i]->outSet.clear();
	}
	do {
		changed = false;
		for (int i = innerBlocks.size() - 1; i >= 0; i--) {
			BaseBlock* btemp = innerBlocks[i];
			set<string> oldOutSet = btemp->outSet;
			for (set<int>::iterator ite = btemp->nextBlocks.begin(); ite != btemp->nextBlocks.end(); ite++) {
				//遍历后继基本块，利用它们的inset更新当前的outset
				BaseBlock* ntemp = innerBlocks[*ite];
				_union_set(ntemp->inSet, btemp->outSet, btemp->outSet);//更新outSet
			}
			if (oldOutSet != btemp->outSet)
				changed = true;//btemp的outSet被更新了
			set<string> odd;
			_difference_set(btemp->outSet, btemp->defSet, odd);//out - def
			_union_set(odd, btemp->useSet, btemp->inSet); // in = use U (out - def)
		}
	} while (changed);
}
//判断该四元式是否为函数结尾
bool BaseBlock::isFuncEnd(quadCode& qc) {
	if (qc.op == "endFunc") return true;
	return false;
}
//为一个函数基本块的基本块建立联系
void BaseBlock::buildRel(FuncBaseBlock* fbb) {
	if (fbb == NULL) return;
	int len = fbb->innerBlocks.size();
	for (int i = 0; i < len; i++) {
		BaseBlock* temp = fbb->innerBlocks[i];//temp表示这个函数块的第i块基本块
		if (temp->quadTable.back().op == "GOTO" || temp->quadTable.back().op == "BZ") {
			//如果该块的最后一条四元式是一条跳转指令
			string targetLabel = temp->quadTable.back().left;//获取跳转目标，以便求得其后继块
			for (int j = 0; j < len; j++) {//遍历本函数的所有基本块，找到目标跳转位置
				if (fbb->innerBlocks[j]->quadTable.size() > 0 &&
					isQuadLabel(fbb->innerBlocks[j]->quadTable[0])) {
					string curLabel = fbb->innerBlocks[j]->quadTable[0].op;
					curLabel = curLabel.substr(0, curLabel.size() - 1);//去掉末尾的":"
					if (curLabel == targetLabel) {//与目标标签相符，说明正是我们要找的基本块
						fbb->innerBlocks[i]->nextBlocks.insert(j);//i的后继节点为j
						fbb->innerBlocks[j]->prevBlocks.insert(i);//j的前驱节点为i
						break;//一个跳转指令只能跳到一个标签，所以找到了一个就可以停止搜索了
					}
				}
			}
		}
		if (temp->quadTable.back().op == "ret") {//如果是return语句，那么就需要找到endFunc所在的基本块作为后继
			bool found = false;
			for (int j = 0; j < len; j++) {
				for (int k = 0; k < fbb->innerBlocks[j]->quadTable.size(); k++) {
					if (fbb->innerBlocks[j]->quadTable[k].op == "endFunc") {//找到了endFunc所在的基本块
						fbb->innerBlocks[i]->nextBlocks.insert(j);
						fbb->innerBlocks[j]->prevBlocks.insert(i);
						found = true;
						break;
					}
				}
				if (found) break;
			}
		}
		if (temp->quadTable.back().op != "GOTO" && temp->quadTable.back().op != "ret") {
			//如果不是GOTO也不是ret，说明可能i+1也是i的后继块
			if (i + 1 < len) {
				fbb->innerBlocks[i]->nextBlocks.insert(i + 1);
				fbb->innerBlocks[i + 1]->prevBlocks.insert(i);
			}
		}
	}
}
//产生并且划分基本块
void BaseBlock::genBaseBlock() {
	int i;
	vector<int> blockTags(quadCodeTable.size(), 0);//初始化基本块标签
	for (i = 0; i < quadCodeTable.size(); i++) {
		if (i == 0) {//第0行自动成为入口语句
			blockTags[i] = 1;
		}
		else if (isQuadLabel(quadCodeTable[i]) || isQuadFunc(quadCodeTable[i])) {//如果是标签或者函数
			blockTags[i] = 1;//那么本行就是入口语句
		}
		else if (quadCodeTable[i].op == "GOTO" || quadCodeTable[i].op == "BZ" ||
			quadCodeTable[i].op == "call" || quadCodeTable[i].op == "ret" ||
			quadCodeTable[i].op == "endFunc" || quadCodeTable[i].op == "[]=")
		{
			if (i + 1 < quadCodeTable.size())
				blockTags[i + 1] = 1;
		}
		else if (quadCodeTable[i].op == "scanfInt" || quadCodeTable[i].op == "scanfChr") {
			blockTags[i] = 1;
		}
	}
	i = 0;
#ifdef NEW_QUAD_OUT
	OptimizedQuad << "----------------------------------DAG--------------------------------------" << endl;
#endif // NEW_QUAD_OUT
	while (quadCodeTable[i].op == "const" || quadCodeTable[i].op == "var" ||
		quadCodeTable[i].op == "arrayVar") { //跳过全局常量以及变量声明
		printQuad(quadCodeTable[i]);
		i++;
	}
	int btabIdx = 0;
	while (i < quadCodeTable.size()) {
		btabIdx++;
		FuncBaseBlock* tempFuncBlock = new FuncBaseBlock();
		while (true) {
			BaseBlock* tempBlock = new BaseBlock();
			tempBlock->quadTable.push_back(quadCodeTable[i]);//入口语句一定是基本块
			i++;
			while (i < quadCodeTable.size() && blockTags[i] != 1) {//插入四元式
				tempBlock->quadTable.push_back(quadCodeTable[i]);
				i++;
			}
			tempFuncBlock->innerBlocks.push_back(tempBlock);
			if (isFuncEnd(tempBlock->quadTable.back())) //如果是函数结尾四元式，那么当前函数基本块已经填完，退出循环
				break;
		}
		buildRel(tempFuncBlock);//建立此函数块内基本块之间的联系
		int k = 1;//为函数块填写全局变量和参数变量
		while (symbolTable[k].objTyp != function) {
			tempFuncBlock->glbAndParam.insert(symbolTable[k].name);
			k++;
		}
		k = btab[btabIdx].lastItem;
		while (symbolTable[k].lev != 0) {
			if (symbolTable[k].objTyp == parameter || symbolTable[k].objTyp == constant) {
				tempFuncBlock->glbAndParam.insert(symbolTable[k].name);
			}
			k = symbolTable[k].link;
		}
		funcBlocks.push_back(tempFuncBlock);//将此函数块加入函数块数组。
	}
}
//将参数插入定义集合，前提是它在使用集合中不存在
void BaseBlock::enterDefSet(string arg) {
	if (arg == "") return;
	if (useSet.find(arg) == useSet.end())
		defSet.insert(arg);
}
//将参数插入使用集合，前提是它在定义集合中不存在
void BaseBlock::enterUseSet(string arg) {
	if (arg == "") return;
	if (defSet.find(arg) == defSet.end())
		useSet.insert(arg);
}
//产生使用-定义集合
void BaseBlock::genUseDefSet() {
	defSet.clear();
	useSet.clear();
	for (int i = 0; i < quadTable.size(); i++) {
		if (isQuadCalc(quadTable[i]) || isQuadComp(quadTable[i]) ||
			quadTable[i].op == "=[]" || quadTable[i].op == "[]=") {
			if (!isConstant(quadTable[i].left) && quadTable[i].left != "@RET")// && !isTemp(quadTable[i].left)) 
				enterUseSet(quadTable[i].left);//操作数插入使用集合
			if (!isConstant(quadTable[i].right) && quadTable[i].right != "@RET")// && !isTemp(quadTable[i].right)) 
				enterUseSet(quadTable[i].right);
			enterDefSet(quadTable[i].result);//结果插入定义集合
			if (quadTable[i].op == "=[]") this->arrSet.insert(quadTable[i].left);
			if (quadTable[i].op == "[]=") this->arrSet.insert(quadTable[i].result);
		}
		else if (quadTable[i].op == "=") {//简单赋值
			if (!isConstant(quadTable[i].left) && quadTable[i].left != "@RET")//参数插入使用集合
				enterUseSet(quadTable[i].left);
			enterDefSet(quadTable[i].result);//结果插入定义集合
		}
		else if (quadTable[i].op == "scanfInt" || quadTable[i].op == "scanfChr") {
			if (!isConstant(quadTable[i].left) && quadTable[i].left != "@RET")
				enterDefSet(quadTable[i].left);
		}
		else if (quadTable[i].op == "ret" || quadTable[i].op == "push" || quadTable[i].op == "printInt" ||
			quadTable[i].op == "printChr") { // || quadTable[i].op == "printStr") {
			if (!isConstant(quadTable[i].left) && quadTable[i].left != "@RET")//返回结果不为常数，也不为空
				enterUseSet(quadTable[i].left);
		}
	}
}
//在表节点表中查询相应变量是否存在，按照变量名
int BaseBlock::searchInNodeTable(string name) {
	for (int i = 0; i < nodeTable.size(); i++) {
		if (nodeTable[i]->value == name) {
			return i;
		}
	}
	return -1;
}
//在表节点表中查询相应变量是否存在，按照dagId
int BaseBlock::searchInNodeTable(int dagId) {
	for (int i = 0; i < nodeTable.size(); i++)
		if (nodeTable[i]->dagId == dagId) return i;
	return -1;
}
//在树节点表中查询相应的中间节点是否存在
int BaseBlock::searchInTreeTable(string op, int src1Id, int src2Id) {
	for (int i = 0; i < treeNodes.size(); i++) {
		if (treeNodes[i]->op == op &&
			treeNodes[i]->leftson != NULL && treeNodes[i]->leftson->dagId == src1Id &&
			treeNodes[i]->rightson != NULL && treeNodes[i]->rightson->dagId == src2Id) {
			return i;
		}
	}
	return -1;
}
//在树节点表中查询相应的dagId的节点是否存在
int BaseBlock::searchInTreeTable(int dagId) {
	for (int i = 0; i < treeNodes.size(); i++) {
		if (treeNodes[i]->dagId == dagId) return i;
	}
	return -1;
}
//检查节点队列是否已经装填完毕所有中间节点
bool BaseBlock::checkDagNodeQueue(vector<dagTreeNode*>& dagNodeQueue) {
	int cnt = 0;
	for (int i = 0; i < treeNodes.size(); i++) {
		if (treeNodes[i]->leftson != NULL &&
			treeNodes[i]->rightson != NULL) //这意味着其为一个中间节点
			cnt++;
	}
	return cnt > dagNodeQueue.size();//如果nodeTable中中间节点个数比dagNodeQueue中多，说明还有中间节点未进入队列
}
//检查中间节点是否可以入队，条件是其父节点已经全部入队或者其没有父节点
bool BaseBlock::checkEnqueue(vector<dagTreeNode*>& dagNodeQueue, dagTreeNode* dtn) {
	if (dtn->leftson == NULL &&
		dtn->rightson == NULL)//前提必须是一个中间节点
		return false;
	for (int i = 0; i < dagNodeQueue.size(); i++) {//dtn需要不在dagNodeQueue中
		if (dtn->dagId == dagNodeQueue[i]->dagId)
			return false;
	}
	for (int i = 0; i < dtn->parent.size(); i++) {
		dagTreeNode* par = dtn->parent[i];
		bool parentIn = false;
		for (int j = 0; j < dagNodeQueue.size(); j++) {
			if (dagNodeQueue[j]->dagId == par->dagId)
				parentIn = true;
		}
		if (!parentIn) return false;//说明此父节点未入队
	}
	return true;
}
//新建一个表节点和树节点，插入相应的队列中
void BaseBlock::enterNodeTable(string name) {
	dagTreeNode* treenode = new dagTreeNode(dagNodeCnt++);
	dagTableNode* tablenode = new dagTableNode();
	treenode->value = name;
	tablenode->value = name;
	tablenode->dagId = treenode->dagId;
	nodeTable.push_back(tablenode);
	treeNodes.push_back(treenode);
}
//获取计算节点队列的四元式的最前面的一个四元式
int BaseBlock::dagNodeQueueFront(vector<dagTreeNode*>& que) {
	int ret = this->quadTable.size();
	for (int i = 0; i < que.size(); i++) {
		if (que[i]->quadId < ret)
			ret = que[i]->quadId;
	}
	return ret;
}
//判断变量是不是全局的
bool BaseBlock::isVarGlobal(vector<quadCode>& globalQuad, string& varname) {
	bool isGlobal = false;
	for (int tot = 0; tot < globalQuad.size(); tot++) {
		if (varname == globalQuad[tot].right) {//如果跟全局变量重名，那么赋值，即使是重名但不是去全局变量，那么赋值也不影响正确性
			isGlobal = true;
			break;
		}
	}
	return isGlobal;
}
void BaseBlock::checkFuture(int st, string& result, map<int, vector<int>>& curDagId) {
	//判断变量result的原始值在将来是否会用到，如果是则需要保存其原值
	bool ret = false;
	for (int i = st + 1; i < quadTable.size(); i++) {
		if (quadTable[i].op == "printInt" || quadTable[i].op == "printChr" || quadTable[i].op == "push" ||
			quadTable[i].op == "ret" || quadTable[i].op == "=") {
			if (quadTable[i].left== result && curDagId[i][0] != curDagId[st][0]) {
				ret = true;
				break;
			}
		}
		else if (quadTable[i].op == "=[]" || quadTable[i].op == "[]=" || 
			isQuadComp(quadTable[i]) || isQuadCalc(quadTable[i])) {
			if (quadTable[i].left == result && curDagId[i][0] != curDagId[st][0]) {
				ret = true;
				break;
			}
			if (quadTable[i].right == result && curDagId[i][1] != curDagId[st][0]) {
				ret = true;
				break;
			}
		}
	}
	if (ret && curDagId[st][0] != curDagId[st][1]) {
		string tempreg = buildRegName(allocReg());
		quadCode qc = { "=",quadTable[st].result,"",tempreg };
		printQuad(qc);
		treeNodes[curDagId[st][1]]->value = tempreg;
	}
}
//构建有向无环图以消除局部公共子表达式
void BaseBlock::DAG(vector<quadCode>& globalQuad) {
#ifdef NEW_QUAD_OUT
	OptimizedQuad << "----------------------------------DAG--------------------------------------" << endl;
#endif // NEW_QUAD_OUT
	quadCode qc = { "dagBegin","","","" };
	printQuad(qc);
	map<int, vector<int>> curDagId;//做一个四元式编号到dagId集合的映射，即将需要用到寄存器的四元式(print类等)映射到当时它们所使用的寄存器号
	//生成DAG图
	for (int i = 0; i < quadTable.size(); i++) {
		if (isQuadCalc(quadTable[i]) || quadTable[i].op == "=[]") {//处理运算类四元式，=[]跟运算量四元式很像故也将其归类到运算类四元式中
			//需要用到两个寄存器
			string src1 = quadTable[i].left; //获取操作符以及结果
			string src2 = quadTable[i].right;
			string oprt = quadTable[i].op;
			string result = quadTable[i].result;
			int src1Id, src2Id;
			if (searchInNodeTable(src1) == -1) //左操作数未找到
				enterNodeTable(src1);//新建表节点并将其插入节点表，同时新建树节点
			src1Id = nodeTable[searchInNodeTable(src1)]->dagId;//左操作数节点dag编号
			treeNodes[src1Id]->ref++;
			if (searchInNodeTable(src2) == -1) //右操作数未找到
				enterNodeTable(src2);//新建表节点并将其插入节点表，同时新建树节点
			src2Id = nodeTable[searchInNodeTable(src2)]->dagId;//右操作数节点dag编号
			treeNodes[src2Id]->ref++;
			if (searchInTreeTable(oprt, src1Id, src2Id) == -1) {//中间节点未找到
				dagTreeNode* treenode = new dagTreeNode(dagNodeCnt++);//新建树节点
				treenode->quadId = i;//设置四元式标号
				treenode->op = oprt;//设置操作符
				treenode->value = result;//暂时设置节点value
				treenode->leftson = treeNodes[searchInTreeTable(src1Id)];//链接子节点
				treenode->rightson = treeNodes[searchInTreeTable(src2Id)];
				treeNodes[searchInTreeTable(src1Id)]->parent.push_back(treenode);//链接父节点
				treeNodes[searchInTreeTable(src2Id)]->parent.push_back(treenode);
				treeNodes.push_back(treenode);//新的树节点插入树节点表
			}
			if (searchInNodeTable(result) == -1) { //结果节点在表中未找到
				dagTableNode* tablenode = new dagTableNode();
				tablenode->value = result;
				tablenode->dagId = treeNodes[searchInTreeTable(oprt, src1Id, src2Id)]->dagId;
				nodeTable.push_back(tablenode);//插入表节点表
			}
			int resultIdx = searchInNodeTable(result);
			//将结果id写入节点表中
			nodeTable[resultIdx]->dagId = treeNodes[searchInTreeTable(oprt, src1Id, src2Id)]->dagId;
			curDagId[i] = {};//插入当时所需的值在dag树中的编号
			curDagId[i].push_back(src1Id);
			curDagId[i].push_back(src2Id);
		}
		else if (quadTable[i].op == "=") { //单纯的赋值
			//需要用到一个寄存器
			string src = quadTable[i].left;
			string result = quadTable[i].result;
			int srcId, resultId;
			if (searchInNodeTable(src) == -1)
				enterNodeTable(src);
			srcId = nodeTable[searchInNodeTable(src)]->dagId;
			treeNodes[srcId]->ref++;
			int resultIdx = searchInNodeTable(result);
			if (resultIdx == -1) {//只需要新建表节点，无需新建树节点
				dagTableNode* tablenode = new dagTableNode();
				tablenode->value = result;
				tablenode->dagId = srcId;
				nodeTable.push_back(tablenode);
				resultId = srcId;
			}
			else {
				resultId = nodeTable[resultIdx]->dagId;
				nodeTable[resultIdx]->dagId = srcId;
			}
			curDagId[i] = {};
			curDagId[i].push_back(srcId);
			curDagId[i].push_back(resultId);
		}
		else if (quadTable[i].op == "printInt" || quadTable[i].op == "printChr" || quadTable[i].op == "ret" || quadTable[i].op == "push") {
			//需要用到一个寄存器
			curDagId[i] = {};
			if (searchInNodeTable(quadTable[i].left) != -1) {
				int lidx = nodeTable[searchInNodeTable(quadTable[i].left)]->dagId;
				treeNodes[lidx]->ref++;
				curDagId[i].push_back(lidx);
			}
			else {
				curDagId[i].push_back(-1);//为-1表示就是直接输出四元式中的字符串即可
			}
		}
		else if (isQuadComp(quadTable[i]) || quadTable[i].op == "[]=") {
			//需要用到两个寄存器
			curDagId[i] = {};
			if (searchInNodeTable(quadTable[i].left) != -1) {
				int lidx = nodeTable[searchInNodeTable(quadTable[i].left)]->dagId;
				treeNodes[lidx]->ref++;
				curDagId[i].push_back(lidx);
			}
			else {
				curDagId[i].push_back(-1);
			}
			if (searchInNodeTable(quadTable[i].right) != -1) {
				int ridx = nodeTable[searchInNodeTable(quadTable[i].right)]->dagId;
				treeNodes[ridx]->ref++;
				curDagId[i].push_back(ridx);
			}
			else {
				curDagId[i].push_back(-1);
			}
		}
	}

	//获得计算顺序
	vector<dagTreeNode*> dagNodeQueue;
	while (checkDagNodeQueue(dagNodeQueue)) {//检测树节点表是否装满
		for (int i = 0; i < treeNodes.size(); i++) {
			if (checkEnqueue(dagNodeQueue, treeNodes[i])) {//检查该节点是否能插入树节点表
				dagNodeQueue.push_back(treeNodes[i]);//该节点入队
				break;
			}
		}
		dagTreeNode* temp = treeNodes.back();
		while (temp->leftson != NULL && checkEnqueue(dagNodeQueue, temp->leftson)) {
			dagNodeQueue.push_back(temp->leftson);
			temp = temp->leftson;
		}
	}

	int i = 0, dagNodeFront = dagNodeQueueFront(dagNodeQueue);
	set<string> tempOutSet = outSet;//临时的out集合,由于全局变量的特殊性,将在此基本块中被赋值的全局变量也放入其中
	for (int tot = 0; tot < quadTable.size(); tot++) {
		if (quadTable[tot].op == "=" && isVarGlobal(globalQuad, quadTable[tot].result))
			tempOutSet.insert(quadTable[tot].result);
	}
	//输出基本块头部四元式
	while (i < quadTable.size() && (isQuadLabel(quadTable[i]) || isQuadFunc(quadTable[i]) ||
		quadTable[i].op == "var" || quadTable[i].op == "arrayVar" || quadTable[i].op == "const" ||
		quadTable[i].op == "param")) {
		printQuad(quadTable[i]);
		i++;
	}
	set<string> defvar;//已经定义过的标识符
	if (dagNodeQueue.size() != 0) {//有中间节点需要计算
		//*输出*在运算类四元式之前的四元式，比如函数、label、常量以及变量声明等等。
		while (i < quadTable.size() && i != dagNodeFront) {//只输出在运算类四元式之前的四元式
			if (quadTable[i].op == "=") {//检测赋值语句是否有输出的必要
				int searchIdx = searchInNodeTable(treeNodes[curDagId[i][0]]->value);
				if (searchIdx != -1 && nodeTable[searchIdx]->dagId != curDagId[i][0] &&
					treeNodes[(curDagId[i][0])]->leftson == NULL &&
					treeNodes[(curDagId[i][0])]->rightson == NULL) {
					//如果赋值符号的右部是叶结点，而且其对应的值在后面发生了变化，那么需要保存它的值
					quadTable[i].result = buildRegName(allocReg());
					printQuad(quadTable[i]);
					treeNodes[(curDagId[i][0])]->value = quadTable[i].result;
				}
			}
			else if (quadTable[i].op == "printInt" || quadTable[i].op == "printChr" || quadTable[i].op == "push") {
				if (!curDagId[i].empty()) {
					quadTable[i].left = treeNodes[curDagId[i][0]]->value;
				}
				printQuad(quadTable[i]);
			}
			else if (!(isQuadCalc(quadTable[i]) || quadTable[i].op == "=[]")) {
				printQuad(quadTable[i]);
			}
			i++;
		}
		for (int j = dagNodeQueue.size() - 1; j >= 0; j--) {
			//保存叶结点的值
			dagTreeNode* lchild = dagNodeQueue[j]->leftson;
			dagTreeNode* rchild = dagNodeQueue[j]->rightson;
			int lidx = searchInNodeTable(lchild->value);
			int ridx = searchInNodeTable(rchild->value);
			if (lchild->leftson == NULL && lchild->rightson == NULL &&
				lidx != -1 && nodeTable[lidx]->dagId != lchild->dagId && 
				treeNodes[lchild->dagId]->ref > 1) {
				//应该只对引用次数大于1的叶结点进行保存
				string lreg = buildRegName(allocReg());
				quadCode qc = { "=",treeNodes[lchild->dagId]->value,"",lreg };
				treeNodes[lchild->dagId]->value = lreg;
				printQuad(qc);
			}
			if (rchild->leftson == NULL && rchild->rightson == NULL &&
				ridx != -1 && nodeTable[ridx]->dagId != rchild->dagId &&
				treeNodes[rchild->dagId]->ref > 1) {
				string rreg = buildRegName(allocReg());
				quadCode qc = { "=",treeNodes[rchild->dagId]->value,"",rreg };
				treeNodes[rchild->dagId]->value = rreg;
				printQuad(qc);
			}
		}
		//*输出*将DAG导出为四元式
		while (!dagNodeQueue.empty()) {
			int lopId = curDagId[dagNodeQueue.back()->quadId][0];//左右操作数的值
			int ropId = curDagId[dagNodeQueue.back()->quadId][1];
			string result;
			int resultIdx = searchInNodeTable(dagNodeQueue.back()->dagId);
			if (resultIdx == -1) {
				result = buildRegName(allocReg());
			}
			else {
				result = quadTable[dagNodeQueue.back()->quadId].result;
				resultIdx = searchInNodeTable(result);
				if (resultIdx != -1 && nodeTable[resultIdx]->dagId != dagNodeQueue.back()->dagId) {
					result = nodeTable[searchInNodeTable(dagNodeQueue.back()->dagId)]->value;
				}
			}
			treeNodes[dagNodeQueue.back()->dagId]->value = result;
			quadCode qc = { dagNodeQueue.back()->op,treeNodes[lopId]->value,treeNodes[ropId]->value,result };
			if (searchInNodeTable(result) != -1 && dagNodeQueue.back()->dagId == nodeTable[searchInNodeTable(result)]->dagId)//避免重复赋值
				defvar.insert(result);
			printQuad(qc);
			dagNodeQueue.pop_back();
		}
		//跳过其他运算类，数组取值类，以及赋值型四元式，它们没有计算的必要
		while (i < quadTable.size() && (isQuadCalc(quadTable[i]) || quadTable[i].op == "=" || quadTable[i].op == "=[]")) {
			i++;
		}
		//*输出*处理tempOutset中的变量，给它们赋正确的值，这里的outSet除了本基本块的outSet以外，还需要包括全局变量
		for (set<string>::iterator ite = tempOutSet.begin(); ite != tempOutSet.end(); ite++) {
			//此变量名不允许在已经定义过的变量中出现，也不能是数组名，也不能是当前节点表里面没有的，
			//当前节点表里面没有则说明该变量在以前的基本块中已经被定义
			if (defvar.find(*ite) == defvar.end() && arrSet.find(*ite) == arrSet.end() &&
				searchInNodeTable(*ite) != -1) {
				quadCode temp;
				temp.op = "=";
				//temp.left = nodeTable[searchInNodeTable(nodeTable[searchInNodeTable(*ite)]->dagId)]->value;//取其在DAG树中的值
				temp.left = treeNodes[nodeTable[searchInNodeTable(*ite)]->dagId]->value;//取其在DAG树中的值
				temp.result = (*ite);
				if (temp.result != temp.left) { //如果赋值的两边符号相同，那么没有赋值的必要
					defvar.insert(temp.result);
					printQuad(temp);
				}
			}
		}
	}

	//输出剩余四元式
	while (i < quadTable.size()) {
		if (isQuadComp(quadTable[i]) || quadTable[i].op == "[]=") {//left与right域都需要更新的四元式
			if (curDagId[i][0] != -1)
				quadTable[i].left = treeNodes[curDagId[i][0]]->value;
			if (curDagId[i][1] != -1)
				quadTable[i].right = treeNodes[curDagId[i][1]]->value;
		}
		else if (quadTable[i].op == "=") {
			int lidx = searchInNodeTable(quadTable[i].left);
			if (curDagId[i][0] != nodeTable[lidx]->dagId) { //左节点的值不久后会改变
				quadTable[i].left = treeNodes[curDagId[i][0]]->value;
				treeNodes[curDagId[i][0]]->value = quadTable[i].result;
				checkFuture(i, quadTable[i].result, curDagId);//检测赋值符号右部的原值是否在将来会被用到，如果用到那么保存一下原值
				printQuad(quadTable[i]);
			}
			else if (tempOutSet.find(quadTable[i].result) != tempOutSet.end()) { //如果以后会用到，那么输出
				quadTable[i].left = treeNodes[curDagId[i][0]]->value;
				checkFuture(i, quadTable[i].result, curDagId);
				printQuad(quadTable[i]);
			}
		}
		else if (quadTable[i].op == "printInt" || quadTable[i].op == "printChr" ||
			quadTable[i].op == "push" || quadTable[i].op == "ret") {//这些是仅仅left域需要更新的四元式
			if (curDagId[i][0] != -1) 
				quadTable[i].left = treeNodes[curDagId[i][0]]->value;
		}
		if (!(isQuadCalc(quadTable[i]) || quadTable[i].op == "=[]" || quadTable[i].op == "=")) {
			//不输出运算类四元式
			printQuad(quadTable[i]);
		}
		i++;
	}
}
#endif // !OPTIMIZATION_H
