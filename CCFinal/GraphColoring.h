#ifndef GRAPH_COLORING
#define GGRAPH_COLORIN
#include "optimization.h"
#define GLOBAL_REG_NUM 8 //全局寄存器数量
/************************************************************************/
/* 图染色算法首先需要构建冲突图，构建方法如下：
	对于基本块B，首先令live = out[B]
	然后对于B中的指令I，从后往前，开始遍历
		对于任意变量x属于def(I)，对于任意变量y属于live
			冲突图添加边(x,y)
		然后更新live = use(I) U (live-def(I))
*/
/************************************************************************/
//图染色器
class graphColorer {
public:
	map<string, int> id2int;//标识符名到节点编号的映射
	map<int, int> int2color;//节点编号与对应的寄存器的映射
	set<string> crossBBVar;//跨基本块仍然活跃的变量
	graphColorer();
	void colorGraph(FuncBaseBlock* fbb, vector<vector<int>>& iG, set<string>& cbv);
	void buildInterferenceGraph(FuncBaseBlock* fbb);
};
//初始化图染色器
graphColorer::graphColorer() {
}
//开始图染色,根据冲突图和跨基本块仍然活跃的变量
void graphColorer::colorGraph(FuncBaseBlock* fbb, vector<vector<int>>& iG, set<string>& cbv) {
	//初始化寄存器分配
	int2color.clear();
	for (int i = 0; i < cbv.size(); i++) {
		int2color[i] = -1;
	}
	vector<int> nodeQue;//记录节点移走顺序的队列
	set<int> noGlobalReg;//不分配全局寄存器的变量集合
	vector<vector<int>> saveIG = iG;//保存冲突图
	while (nodeQue.size() < cbv.size()) {//如果还有节点没有添加到队列中，那么继续
		bool flag = false;
		int lastNode;
		for (set<string>::iterator ite = cbv.begin(); ite != cbv.end(); ite++) {
			int nodeId = id2int[*ite];//此节点的编号
			if (find(nodeQue.begin(), nodeQue.end(), nodeId) != nodeQue.end()) //如果此节点已经添加到队列中了，那么跳过
				continue;
			lastNode = nodeId;
			int degree = 0;//此节点的度，也就是节点所链接的边的数量
			for (int i = 0; i < iG[nodeId].size(); i++) {//求出此节点对应的边的数量
				if (iG[nodeId][i] == 1) {
					degree++;
				}
			}
			if (degree < GLOBAL_REG_NUM) {//此节点的度小于GLOBAL_REG_NUM，可以被加入队列中
				nodeQue.push_back(nodeId);
				for (int i = 0; i < iG[nodeId].size(); i++) {//移除与该节点相连的所有边
					if (iG[nodeId][i] == 1) {
						iG[nodeId][i] = 0;
						iG[i][nodeId] = 0;
					}
				}
				flag = true;
				break;
			}
		}
		if (!flag) {//没有一个节点的度小于GLOBAL_REG_NUM，则标记一个点当做不分配全局寄存器的点
			nodeQue.push_back(lastNode);//插入进队列
			noGlobalReg.insert(lastNode);//不分配全局变量
			for (int i = 0; i < iG[lastNode].size(); i++) {//从图中移除该节点
				if (iG[lastNode][i] == 1) {
					iG[lastNode][i] = 0;
					iG[i][lastNode] = 0;
				}
			}
		} 
	}
	int color = 0;
	while (!nodeQue.empty()) {
		int nodeId = nodeQue.back();
		if (noGlobalReg.find(nodeId) == noGlobalReg.end()) {//只有未标记“不分配全局变量”的节点才能分配到全局变量
			if (color < GLOBAL_REG_NUM) {
				int2color[nodeId] = color;
				color++;
			}
			//for (int color = 0; color < GLOBAL_REG_NUM; color++) {
			//	bool dupColor = false;//颜色是否重复
			//	for (int i = 0; i < saveIG[nodeId].size(); i++) {//判断nodeId周围的节点是否与nodeId的颜色相同
			//		if (saveIG[nodeId][i] == 1 && color == int2color[i]) {
			//			dupColor = true;
			//			break;
			//		}
			//	}
			//	if (!dupColor) {//如果nodeId与周围节点颜色不重复，那么将其染色
			//		int2color[nodeId] = color;
			//		break;
			//	}
			//}
		}
		nodeQue.pop_back();
	}
	//最终得到int2color，这样int2color[id2int[id]]就能根据标识符得到其对应的全局寄存器编号
	fbb->id2Greg.clear();
	for (map<string, int>::iterator ite = id2int.begin(); ite != id2int.end(); ite++) {//填充函数块的全局标识符到被分配的全局寄存器的映射
		if (int2color[id2int[ite->first]] != -1) {
			fbb->id2Greg[ite->first] = int2color[id2int[ite->first]];
		}
	}
}
//构建冲突图
void graphColorer::buildInterferenceGraph(FuncBaseBlock* fbb) {
	crossBBVar.clear();
	id2int.clear();
	fbb->genInOutSet();//函数生成in,out,use,def集合
	for (int i = 0; i < fbb->innerBlocks.size(); i++) {//将此函数块中的所有基本块的out集中的变量放入crossBBVar，因为那些变量是跨基本块仍然活跃的
		BaseBlock* temp = fbb->innerBlocks[i];
		set<string>::iterator ite = temp->outSet.begin();
		while (ite != temp->outSet.end()){//程序的全局变量不分配全局寄存器
			crossBBVar.insert(*ite);
			ite++;
		}
	}
	int nodeIdx = 0;//节点编号
	for (set<string>::iterator ite = crossBBVar.begin(); ite != crossBBVar.end(); ite++) 
		id2int[*ite] = nodeIdx++;
	vector<vector<int>> interferenceGraph(nodeIdx, vector<int>(nodeIdx, 0));//冲突图
	for (int i = 0; i < fbb->innerBlocks.size(); i++) {
		BaseBlock* temp = fbb->innerBlocks[i];
		set<string> live = temp->outSet;
		for (int j = temp->newQuads.size() - 1; j >= 0; j--) {//逆序访问此基本块的四元式
			quadCode qc = temp->newQuads[j];
			string x = "";
			set<string> useI;//指令I的使用集合与定义集
			if (isQuadCalc(qc)) {
				x = qc.result;
				if (!isConstant(qc.left) && qc.left != "@RET")
					useI.insert(qc.left);
				if (!isConstant(qc.right) && qc.right != "@RET") 
					useI.insert(qc.right);
			}
			else if (isQuadComp(qc) || qc.op == "[]=") {
				if (!isConstant(qc.left) && qc.left != "@RET")
					useI.insert(qc.left);
				if (!isConstant(qc.right) && qc.right != "@RET") 
					useI.insert(qc.right);
			}
			else if (qc.op == "=[]") {
				x = qc.result;
				if (!isConstant(qc.right) && qc.right != "@RET") 
					useI.insert(qc.right);
			}
			else if (qc.op == "=") {
				x = qc.result;
				if (!isConstant(qc.left) && qc.left != "@RET")
					useI.insert(qc.left);
			}
			else if (qc.op == "printInt" || qc.op == "printChr" || qc.op == "ret" || qc.op == "push") {
				if (!isConstant(qc.left) && qc.left != "@RET")
					useI.insert(qc.left);
			}
			else if (qc.op == "scanfInt" || qc.op == "scanfChr") {//scanf的变量被认为是赋值，所以也算是def
				x = qc.left;
			}
			if (x != "" && crossBBVar.find(x) != crossBBVar.end()) {//如果被定义的变量不为空并且是跨基本块的变量，那么开始添加冲突图的边
				for (set<string>::iterator ite = live.begin(); ite != live.end(); ite++) {
					if (crossBBVar.find(*ite) != crossBBVar.end() && x != *ite) {
						interferenceGraph[id2int[x]][id2int[*ite]] = 1;
						interferenceGraph[id2int[*ite]][id2int[x]] = 1;
					}
				}
			}
			//live = useI U (live - defI) 其中defI就是就只有一个变量
			set<string> tempset = useI;
			for (set<string>::iterator ite = live.begin(); ite != live.end(); ite++) {
				if (*ite != x)
					tempset.insert(*ite);
			}
			live = tempset;
		}
	}
	colorGraph(fbb, interferenceGraph, crossBBVar);
}
#endif // GRAPH_COLORING
