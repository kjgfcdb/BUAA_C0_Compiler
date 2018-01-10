#ifndef OPTIMIZATION_H
#define OPTIMIZATION_H
#include "lexical.h"
//#define BASEBLOCK_DEBUG	//�Ƿ������������ѡ��
#define NEW_QUAD_OUT
#ifdef NEW_QUAD_OUT//�Ƿ��Ż������Ԫʽ���
ofstream OptimizedQuad;
#endif

class dagTableNode {//dag��ڵ㣬��������nodeTable�еĶ���
public:
	string value;//�ýڵ�����ֻ���ֵ
	int dagId;//�ýڵ��Ӧ��dag���ڵ�ı��
};//DAG��ڵ�

class dagTreeNode {//dag���ڵ�
public:
	string value;//���ڵ��Ӧ���ַ���
	string op;//���ڵ�Ĳ�����
	int quadId;//�˽ڵ��Ӧ����Ԫʽ����
	int dagId;//���ڵ��dagId
	int ref;//���ô���
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
//�����飬����һ�������
class FuncBaseBlock {
public:
	vector<BaseBlock*> innerBlocks;//���ڵ�ǰ��������ڲ���
								   //���ɴ˺������ڻ������in��out����
	map<string, int> id2Greg;//����ʶ��ӳ�䵽ȫ�ּĴ���
	set<string> glbAndParam;//ȫ�ֱ����Ͳ���
	void genInOutSet();
};
//��������
class BaseBlock {
public:
	vector<FuncBaseBlock*> funcBlocks;//�Ժ���Ϊ��λ���ֻ�����
	vector<quadCode> quadTable;//�˻���������������Ԫʽ
	vector<quadCode> newQuads;
	vector<dagTableNode*> nodeTable;//����DAG�ڵ�Ľڵ��
	vector<dagTreeNode*> treeNodes;//���ڵ��
	int dagNodeCnt = 0;//dag���ڵ������
	set<int> prevBlocks;//�˻������ǰ������������
	set<int> nextBlocks;//�˻�����ĺ�̻���������
	set<string> useSet;//�˻�����ʹ�ü���
	set<string> defSet;//�˻����鶨�弯��
	set<string> inSet;//�˻�����in����
	set<string> outSet;//�˻�����out����
	set<string> arrSet;//���鼯�ϣ��洢�˻������е�������
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
	int dagNodeQueueFront(vector<dagTreeNode *>& que);//���Ҫ��dag�������ĵ�һ����Ԫʽ������
	bool isVarGlobal(vector<quadCode>& globalQuad, string& varname);//�жϱ����Ƿ�Ϊȫ�ֱ���
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
//������a�ͼ���b�Ľ�����������뼯��c
void _union_set(set<string> a, set<string> b, set<string>&c) {
	c.clear();
	for (set<string>::iterator ite = a.begin(); ite != a.end(); ite++)
		c.insert(*ite);
	for (set<string>::iterator ite = b.begin(); ite != b.end(); ite++)
		c.insert(*ite);
}
//�󼯺�a��b�Ĳ���������c��,c = a-b
void _difference_set(set<string> a, set<string> b, set<string>& c) {
	c.clear();
	for (set<string>::iterator ite = a.begin(); ite != a.end(); ite++)
		if (b.find(*ite) == b.end()) {
			c.insert(*ite);//��a�ж�����b�е�Ԫ��
		}
}
//��ȡin��out����
void FuncBaseBlock::genInOutSet() {
	bool changed;
	for (int i = 0; i < innerBlocks.size(); i++) {
		innerBlocks[i]->genUseDefSet();//ÿ������������use��def����
		innerBlocks[i]->inSet.clear();//��ʼ����in��out��������Ϊ�ռ�
		innerBlocks[i]->outSet.clear();
	}
	do {
		changed = false;
		for (int i = innerBlocks.size() - 1; i >= 0; i--) {
			BaseBlock* btemp = innerBlocks[i];
			set<string> oldOutSet = btemp->outSet;
			for (set<int>::iterator ite = btemp->nextBlocks.begin(); ite != btemp->nextBlocks.end(); ite++) {
				//������̻����飬�������ǵ�inset���µ�ǰ��outset
				BaseBlock* ntemp = innerBlocks[*ite];
				_union_set(ntemp->inSet, btemp->outSet, btemp->outSet);//����outSet
			}
			if (oldOutSet != btemp->outSet)
				changed = true;//btemp��outSet��������
			set<string> odd;
			_difference_set(btemp->outSet, btemp->defSet, odd);//out - def
			_union_set(odd, btemp->useSet, btemp->inSet); // in = use U (out - def)
		}
	} while (changed);
}
//�жϸ���Ԫʽ�Ƿ�Ϊ������β
bool BaseBlock::isFuncEnd(quadCode& qc) {
	if (qc.op == "endFunc") return true;
	return false;
}
//Ϊһ������������Ļ����齨����ϵ
void BaseBlock::buildRel(FuncBaseBlock* fbb) {
	if (fbb == NULL) return;
	int len = fbb->innerBlocks.size();
	for (int i = 0; i < len; i++) {
		BaseBlock* temp = fbb->innerBlocks[i];//temp��ʾ���������ĵ�i�������
		if (temp->quadTable.back().op == "GOTO" || temp->quadTable.back().op == "BZ") {
			//����ÿ�����һ����Ԫʽ��һ����תָ��
			string targetLabel = temp->quadTable.back().left;//��ȡ��תĿ�꣬�Ա�������̿�
			for (int j = 0; j < len; j++) {//���������������л����飬�ҵ�Ŀ����תλ��
				if (fbb->innerBlocks[j]->quadTable.size() > 0 &&
					isQuadLabel(fbb->innerBlocks[j]->quadTable[0])) {
					string curLabel = fbb->innerBlocks[j]->quadTable[0].op;
					curLabel = curLabel.substr(0, curLabel.size() - 1);//ȥ��ĩβ��":"
					if (curLabel == targetLabel) {//��Ŀ���ǩ�����˵����������Ҫ�ҵĻ�����
						fbb->innerBlocks[i]->nextBlocks.insert(j);//i�ĺ�̽ڵ�Ϊj
						fbb->innerBlocks[j]->prevBlocks.insert(i);//j��ǰ���ڵ�Ϊi
						break;//һ����תָ��ֻ������һ����ǩ�������ҵ���һ���Ϳ���ֹͣ������
					}
				}
			}
		}
		if (temp->quadTable.back().op == "ret") {//�����return��䣬��ô����Ҫ�ҵ�endFunc���ڵĻ�������Ϊ���
			bool found = false;
			for (int j = 0; j < len; j++) {
				for (int k = 0; k < fbb->innerBlocks[j]->quadTable.size(); k++) {
					if (fbb->innerBlocks[j]->quadTable[k].op == "endFunc") {//�ҵ���endFunc���ڵĻ�����
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
			//�������GOTOҲ����ret��˵������i+1Ҳ��i�ĺ�̿�
			if (i + 1 < len) {
				fbb->innerBlocks[i]->nextBlocks.insert(i + 1);
				fbb->innerBlocks[i + 1]->prevBlocks.insert(i);
			}
		}
	}
}
//�������һ��ֻ�����
void BaseBlock::genBaseBlock() {
	int i;
	vector<int> blockTags(quadCodeTable.size(), 0);//��ʼ���������ǩ
	for (i = 0; i < quadCodeTable.size(); i++) {
		if (i == 0) {//��0���Զ���Ϊ������
			blockTags[i] = 1;
		}
		else if (isQuadLabel(quadCodeTable[i]) || isQuadFunc(quadCodeTable[i])) {//����Ǳ�ǩ���ߺ���
			blockTags[i] = 1;//��ô���о���������
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
		quadCodeTable[i].op == "arrayVar") { //����ȫ�ֳ����Լ���������
		printQuad(quadCodeTable[i]);
		i++;
	}
	int btabIdx = 0;
	while (i < quadCodeTable.size()) {
		btabIdx++;
		FuncBaseBlock* tempFuncBlock = new FuncBaseBlock();
		while (true) {
			BaseBlock* tempBlock = new BaseBlock();
			tempBlock->quadTable.push_back(quadCodeTable[i]);//������һ���ǻ�����
			i++;
			while (i < quadCodeTable.size() && blockTags[i] != 1) {//������Ԫʽ
				tempBlock->quadTable.push_back(quadCodeTable[i]);
				i++;
			}
			tempFuncBlock->innerBlocks.push_back(tempBlock);
			if (isFuncEnd(tempBlock->quadTable.back())) //����Ǻ�����β��Ԫʽ����ô��ǰ�����������Ѿ����꣬�˳�ѭ��
				break;
		}
		buildRel(tempFuncBlock);//�����˺������ڻ�����֮�����ϵ
		int k = 1;//Ϊ��������дȫ�ֱ����Ͳ�������
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
		funcBlocks.push_back(tempFuncBlock);//���˺�������뺯�������顣
	}
}
//���������붨�弯�ϣ�ǰ��������ʹ�ü����в�����
void BaseBlock::enterDefSet(string arg) {
	if (arg == "") return;
	if (useSet.find(arg) == useSet.end())
		defSet.insert(arg);
}
//����������ʹ�ü��ϣ�ǰ�������ڶ��弯���в�����
void BaseBlock::enterUseSet(string arg) {
	if (arg == "") return;
	if (defSet.find(arg) == defSet.end())
		useSet.insert(arg);
}
//����ʹ��-���弯��
void BaseBlock::genUseDefSet() {
	defSet.clear();
	useSet.clear();
	for (int i = 0; i < quadTable.size(); i++) {
		if (isQuadCalc(quadTable[i]) || isQuadComp(quadTable[i]) ||
			quadTable[i].op == "=[]" || quadTable[i].op == "[]=") {
			if (!isConstant(quadTable[i].left) && quadTable[i].left != "@RET")// && !isTemp(quadTable[i].left)) 
				enterUseSet(quadTable[i].left);//����������ʹ�ü���
			if (!isConstant(quadTable[i].right) && quadTable[i].right != "@RET")// && !isTemp(quadTable[i].right)) 
				enterUseSet(quadTable[i].right);
			enterDefSet(quadTable[i].result);//������붨�弯��
			if (quadTable[i].op == "=[]") this->arrSet.insert(quadTable[i].left);
			if (quadTable[i].op == "[]=") this->arrSet.insert(quadTable[i].result);
		}
		else if (quadTable[i].op == "=") {//�򵥸�ֵ
			if (!isConstant(quadTable[i].left) && quadTable[i].left != "@RET")//��������ʹ�ü���
				enterUseSet(quadTable[i].left);
			enterDefSet(quadTable[i].result);//������붨�弯��
		}
		else if (quadTable[i].op == "scanfInt" || quadTable[i].op == "scanfChr") {
			if (!isConstant(quadTable[i].left) && quadTable[i].left != "@RET")
				enterDefSet(quadTable[i].left);
		}
		else if (quadTable[i].op == "ret" || quadTable[i].op == "push" || quadTable[i].op == "printInt" ||
			quadTable[i].op == "printChr") { // || quadTable[i].op == "printStr") {
			if (!isConstant(quadTable[i].left) && quadTable[i].left != "@RET")//���ؽ����Ϊ������Ҳ��Ϊ��
				enterUseSet(quadTable[i].left);
		}
	}
}
//�ڱ�ڵ���в�ѯ��Ӧ�����Ƿ���ڣ����ձ�����
int BaseBlock::searchInNodeTable(string name) {
	for (int i = 0; i < nodeTable.size(); i++) {
		if (nodeTable[i]->value == name) {
			return i;
		}
	}
	return -1;
}
//�ڱ�ڵ���в�ѯ��Ӧ�����Ƿ���ڣ�����dagId
int BaseBlock::searchInNodeTable(int dagId) {
	for (int i = 0; i < nodeTable.size(); i++)
		if (nodeTable[i]->dagId == dagId) return i;
	return -1;
}
//�����ڵ���в�ѯ��Ӧ���м�ڵ��Ƿ����
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
//�����ڵ���в�ѯ��Ӧ��dagId�Ľڵ��Ƿ����
int BaseBlock::searchInTreeTable(int dagId) {
	for (int i = 0; i < treeNodes.size(); i++) {
		if (treeNodes[i]->dagId == dagId) return i;
	}
	return -1;
}
//���ڵ�����Ƿ��Ѿ�װ����������м�ڵ�
bool BaseBlock::checkDagNodeQueue(vector<dagTreeNode*>& dagNodeQueue) {
	int cnt = 0;
	for (int i = 0; i < treeNodes.size(); i++) {
		if (treeNodes[i]->leftson != NULL &&
			treeNodes[i]->rightson != NULL) //����ζ����Ϊһ���м�ڵ�
			cnt++;
	}
	return cnt > dagNodeQueue.size();//���nodeTable���м�ڵ������dagNodeQueue�ж࣬˵�������м�ڵ�δ�������
}
//����м�ڵ��Ƿ������ӣ��������丸�ڵ��Ѿ�ȫ����ӻ�����û�и��ڵ�
bool BaseBlock::checkEnqueue(vector<dagTreeNode*>& dagNodeQueue, dagTreeNode* dtn) {
	if (dtn->leftson == NULL &&
		dtn->rightson == NULL)//ǰ�������һ���м�ڵ�
		return false;
	for (int i = 0; i < dagNodeQueue.size(); i++) {//dtn��Ҫ����dagNodeQueue��
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
		if (!parentIn) return false;//˵���˸��ڵ�δ���
	}
	return true;
}
//�½�һ����ڵ�����ڵ㣬������Ӧ�Ķ�����
void BaseBlock::enterNodeTable(string name) {
	dagTreeNode* treenode = new dagTreeNode(dagNodeCnt++);
	dagTableNode* tablenode = new dagTableNode();
	treenode->value = name;
	tablenode->value = name;
	tablenode->dagId = treenode->dagId;
	nodeTable.push_back(tablenode);
	treeNodes.push_back(treenode);
}
//��ȡ����ڵ���е���Ԫʽ����ǰ���һ����Ԫʽ
int BaseBlock::dagNodeQueueFront(vector<dagTreeNode*>& que) {
	int ret = this->quadTable.size();
	for (int i = 0; i < que.size(); i++) {
		if (que[i]->quadId < ret)
			ret = que[i]->quadId;
	}
	return ret;
}
//�жϱ����ǲ���ȫ�ֵ�
bool BaseBlock::isVarGlobal(vector<quadCode>& globalQuad, string& varname) {
	bool isGlobal = false;
	for (int tot = 0; tot < globalQuad.size(); tot++) {
		if (varname == globalQuad[tot].right) {//�����ȫ�ֱ�����������ô��ֵ����ʹ������������ȥȫ�ֱ�������ô��ֵҲ��Ӱ����ȷ��
			isGlobal = true;
			break;
		}
	}
	return isGlobal;
}
void BaseBlock::checkFuture(int st, string& result, map<int, vector<int>>& curDagId) {
	//�жϱ���result��ԭʼֵ�ڽ����Ƿ���õ������������Ҫ������ԭֵ
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
//���������޻�ͼ�������ֲ������ӱ��ʽ
void BaseBlock::DAG(vector<quadCode>& globalQuad) {
#ifdef NEW_QUAD_OUT
	OptimizedQuad << "----------------------------------DAG--------------------------------------" << endl;
#endif // NEW_QUAD_OUT
	quadCode qc = { "dagBegin","","","" };
	printQuad(qc);
	map<int, vector<int>> curDagId;//��һ����Ԫʽ��ŵ�dagId���ϵ�ӳ�䣬������Ҫ�õ��Ĵ�������Ԫʽ(print���)ӳ�䵽��ʱ������ʹ�õļĴ�����
	//����DAGͼ
	for (int i = 0; i < quadTable.size(); i++) {
		if (isQuadCalc(quadTable[i]) || quadTable[i].op == "=[]") {//������������Ԫʽ��=[]����������Ԫʽ�����Ҳ������ൽ��������Ԫʽ��
			//��Ҫ�õ������Ĵ���
			string src1 = quadTable[i].left; //��ȡ�������Լ����
			string src2 = quadTable[i].right;
			string oprt = quadTable[i].op;
			string result = quadTable[i].result;
			int src1Id, src2Id;
			if (searchInNodeTable(src1) == -1) //�������δ�ҵ�
				enterNodeTable(src1);//�½���ڵ㲢�������ڵ��ͬʱ�½����ڵ�
			src1Id = nodeTable[searchInNodeTable(src1)]->dagId;//��������ڵ�dag���
			treeNodes[src1Id]->ref++;
			if (searchInNodeTable(src2) == -1) //�Ҳ�����δ�ҵ�
				enterNodeTable(src2);//�½���ڵ㲢�������ڵ��ͬʱ�½����ڵ�
			src2Id = nodeTable[searchInNodeTable(src2)]->dagId;//�Ҳ������ڵ�dag���
			treeNodes[src2Id]->ref++;
			if (searchInTreeTable(oprt, src1Id, src2Id) == -1) {//�м�ڵ�δ�ҵ�
				dagTreeNode* treenode = new dagTreeNode(dagNodeCnt++);//�½����ڵ�
				treenode->quadId = i;//������Ԫʽ���
				treenode->op = oprt;//���ò�����
				treenode->value = result;//��ʱ���ýڵ�value
				treenode->leftson = treeNodes[searchInTreeTable(src1Id)];//�����ӽڵ�
				treenode->rightson = treeNodes[searchInTreeTable(src2Id)];
				treeNodes[searchInTreeTable(src1Id)]->parent.push_back(treenode);//���Ӹ��ڵ�
				treeNodes[searchInTreeTable(src2Id)]->parent.push_back(treenode);
				treeNodes.push_back(treenode);//�µ����ڵ�������ڵ��
			}
			if (searchInNodeTable(result) == -1) { //����ڵ��ڱ���δ�ҵ�
				dagTableNode* tablenode = new dagTableNode();
				tablenode->value = result;
				tablenode->dagId = treeNodes[searchInTreeTable(oprt, src1Id, src2Id)]->dagId;
				nodeTable.push_back(tablenode);//�����ڵ��
			}
			int resultIdx = searchInNodeTable(result);
			//�����idд��ڵ����
			nodeTable[resultIdx]->dagId = treeNodes[searchInTreeTable(oprt, src1Id, src2Id)]->dagId;
			curDagId[i] = {};//���뵱ʱ�����ֵ��dag���еı��
			curDagId[i].push_back(src1Id);
			curDagId[i].push_back(src2Id);
		}
		else if (quadTable[i].op == "=") { //�����ĸ�ֵ
			//��Ҫ�õ�һ���Ĵ���
			string src = quadTable[i].left;
			string result = quadTable[i].result;
			int srcId, resultId;
			if (searchInNodeTable(src) == -1)
				enterNodeTable(src);
			srcId = nodeTable[searchInNodeTable(src)]->dagId;
			treeNodes[srcId]->ref++;
			int resultIdx = searchInNodeTable(result);
			if (resultIdx == -1) {//ֻ��Ҫ�½���ڵ㣬�����½����ڵ�
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
			//��Ҫ�õ�һ���Ĵ���
			curDagId[i] = {};
			if (searchInNodeTable(quadTable[i].left) != -1) {
				int lidx = nodeTable[searchInNodeTable(quadTable[i].left)]->dagId;
				treeNodes[lidx]->ref++;
				curDagId[i].push_back(lidx);
			}
			else {
				curDagId[i].push_back(-1);//Ϊ-1��ʾ����ֱ�������Ԫʽ�е��ַ�������
			}
		}
		else if (isQuadComp(quadTable[i]) || quadTable[i].op == "[]=") {
			//��Ҫ�õ������Ĵ���
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

	//��ü���˳��
	vector<dagTreeNode*> dagNodeQueue;
	while (checkDagNodeQueue(dagNodeQueue)) {//������ڵ���Ƿ�װ��
		for (int i = 0; i < treeNodes.size(); i++) {
			if (checkEnqueue(dagNodeQueue, treeNodes[i])) {//���ýڵ��Ƿ��ܲ������ڵ��
				dagNodeQueue.push_back(treeNodes[i]);//�ýڵ����
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
	set<string> tempOutSet = outSet;//��ʱ��out����,����ȫ�ֱ�����������,���ڴ˻������б���ֵ��ȫ�ֱ���Ҳ��������
	for (int tot = 0; tot < quadTable.size(); tot++) {
		if (quadTable[tot].op == "=" && isVarGlobal(globalQuad, quadTable[tot].result))
			tempOutSet.insert(quadTable[tot].result);
	}
	//���������ͷ����Ԫʽ
	while (i < quadTable.size() && (isQuadLabel(quadTable[i]) || isQuadFunc(quadTable[i]) ||
		quadTable[i].op == "var" || quadTable[i].op == "arrayVar" || quadTable[i].op == "const" ||
		quadTable[i].op == "param")) {
		printQuad(quadTable[i]);
		i++;
	}
	set<string> defvar;//�Ѿ�������ı�ʶ��
	if (dagNodeQueue.size() != 0) {//���м�ڵ���Ҫ����
		//*���*����������Ԫʽ֮ǰ����Ԫʽ�����纯����label�������Լ����������ȵȡ�
		while (i < quadTable.size() && i != dagNodeFront) {//ֻ�������������Ԫʽ֮ǰ����Ԫʽ
			if (quadTable[i].op == "=") {//��⸳ֵ����Ƿ�������ı�Ҫ
				int searchIdx = searchInNodeTable(treeNodes[curDagId[i][0]]->value);
				if (searchIdx != -1 && nodeTable[searchIdx]->dagId != curDagId[i][0] &&
					treeNodes[(curDagId[i][0])]->leftson == NULL &&
					treeNodes[(curDagId[i][0])]->rightson == NULL) {
					//�����ֵ���ŵ��Ҳ���Ҷ��㣬�������Ӧ��ֵ�ں��淢���˱仯����ô��Ҫ��������ֵ
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
			//����Ҷ����ֵ
			dagTreeNode* lchild = dagNodeQueue[j]->leftson;
			dagTreeNode* rchild = dagNodeQueue[j]->rightson;
			int lidx = searchInNodeTable(lchild->value);
			int ridx = searchInNodeTable(rchild->value);
			if (lchild->leftson == NULL && lchild->rightson == NULL &&
				lidx != -1 && nodeTable[lidx]->dagId != lchild->dagId && 
				treeNodes[lchild->dagId]->ref > 1) {
				//Ӧ��ֻ�����ô�������1��Ҷ�����б���
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
		//*���*��DAG����Ϊ��Ԫʽ
		while (!dagNodeQueue.empty()) {
			int lopId = curDagId[dagNodeQueue.back()->quadId][0];//���Ҳ�������ֵ
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
			if (searchInNodeTable(result) != -1 && dagNodeQueue.back()->dagId == nodeTable[searchInNodeTable(result)]->dagId)//�����ظ���ֵ
				defvar.insert(result);
			printQuad(qc);
			dagNodeQueue.pop_back();
		}
		//�������������࣬����ȡֵ�࣬�Լ���ֵ����Ԫʽ������û�м���ı�Ҫ
		while (i < quadTable.size() && (isQuadCalc(quadTable[i]) || quadTable[i].op == "=" || quadTable[i].op == "=[]")) {
			i++;
		}
		//*���*����tempOutset�еı����������Ǹ���ȷ��ֵ�������outSet���˱��������outSet���⣬����Ҫ����ȫ�ֱ���
		for (set<string>::iterator ite = tempOutSet.begin(); ite != tempOutSet.end(); ite++) {
			//�˱��������������Ѿ�������ı����г��֣�Ҳ��������������Ҳ�����ǵ�ǰ�ڵ������û�еģ�
			//��ǰ�ڵ������û����˵���ñ�������ǰ�Ļ��������Ѿ�������
			if (defvar.find(*ite) == defvar.end() && arrSet.find(*ite) == arrSet.end() &&
				searchInNodeTable(*ite) != -1) {
				quadCode temp;
				temp.op = "=";
				//temp.left = nodeTable[searchInNodeTable(nodeTable[searchInNodeTable(*ite)]->dagId)]->value;//ȡ����DAG���е�ֵ
				temp.left = treeNodes[nodeTable[searchInNodeTable(*ite)]->dagId]->value;//ȡ����DAG���е�ֵ
				temp.result = (*ite);
				if (temp.result != temp.left) { //�����ֵ�����߷�����ͬ����ôû�и�ֵ�ı�Ҫ
					defvar.insert(temp.result);
					printQuad(temp);
				}
			}
		}
	}

	//���ʣ����Ԫʽ
	while (i < quadTable.size()) {
		if (isQuadComp(quadTable[i]) || quadTable[i].op == "[]=") {//left��right����Ҫ���µ���Ԫʽ
			if (curDagId[i][0] != -1)
				quadTable[i].left = treeNodes[curDagId[i][0]]->value;
			if (curDagId[i][1] != -1)
				quadTable[i].right = treeNodes[curDagId[i][1]]->value;
		}
		else if (quadTable[i].op == "=") {
			int lidx = searchInNodeTable(quadTable[i].left);
			if (curDagId[i][0] != nodeTable[lidx]->dagId) { //��ڵ��ֵ���ú��ı�
				quadTable[i].left = treeNodes[curDagId[i][0]]->value;
				treeNodes[curDagId[i][0]]->value = quadTable[i].result;
				checkFuture(i, quadTable[i].result, curDagId);//��⸳ֵ�����Ҳ���ԭֵ�Ƿ��ڽ����ᱻ�õ�������õ���ô����һ��ԭֵ
				printQuad(quadTable[i]);
			}
			else if (tempOutSet.find(quadTable[i].result) != tempOutSet.end()) { //����Ժ���õ�����ô���
				quadTable[i].left = treeNodes[curDagId[i][0]]->value;
				checkFuture(i, quadTable[i].result, curDagId);
				printQuad(quadTable[i]);
			}
		}
		else if (quadTable[i].op == "printInt" || quadTable[i].op == "printChr" ||
			quadTable[i].op == "push" || quadTable[i].op == "ret") {//��Щ�ǽ���left����Ҫ���µ���Ԫʽ
			if (curDagId[i][0] != -1) 
				quadTable[i].left = treeNodes[curDagId[i][0]]->value;
		}
		if (!(isQuadCalc(quadTable[i]) || quadTable[i].op == "=[]" || quadTable[i].op == "=")) {
			//�������������Ԫʽ
			printQuad(quadTable[i]);
		}
		i++;
	}
}
#endif // !OPTIMIZATION_H
