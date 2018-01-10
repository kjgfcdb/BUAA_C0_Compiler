#ifndef GRAPH_COLORING
#define GGRAPH_COLORIN
#include "optimization.h"
#define GLOBAL_REG_NUM 8 //ȫ�ּĴ�������
/************************************************************************/
/* ͼȾɫ�㷨������Ҫ������ͻͼ�������������£�
	���ڻ�����B��������live = out[B]
	Ȼ�����B�е�ָ��I���Ӻ���ǰ����ʼ����
		�����������x����def(I)�������������y����live
			��ͻͼ��ӱ�(x,y)
		Ȼ�����live = use(I) U (live-def(I))
*/
/************************************************************************/
//ͼȾɫ��
class graphColorer {
public:
	map<string, int> id2int;//��ʶ�������ڵ��ŵ�ӳ��
	map<int, int> int2color;//�ڵ������Ӧ�ļĴ�����ӳ��
	set<string> crossBBVar;//���������Ȼ��Ծ�ı���
	graphColorer();
	void colorGraph(FuncBaseBlock* fbb, vector<vector<int>>& iG, set<string>& cbv);
	void buildInterferenceGraph(FuncBaseBlock* fbb);
};
//��ʼ��ͼȾɫ��
graphColorer::graphColorer() {
}
//��ʼͼȾɫ,���ݳ�ͻͼ�Ϳ��������Ȼ��Ծ�ı���
void graphColorer::colorGraph(FuncBaseBlock* fbb, vector<vector<int>>& iG, set<string>& cbv) {
	//��ʼ���Ĵ�������
	int2color.clear();
	for (int i = 0; i < cbv.size(); i++) {
		int2color[i] = -1;
	}
	vector<int> nodeQue;//��¼�ڵ�����˳��Ķ���
	set<int> noGlobalReg;//������ȫ�ּĴ����ı�������
	vector<vector<int>> saveIG = iG;//�����ͻͼ
	while (nodeQue.size() < cbv.size()) {//������нڵ�û����ӵ������У���ô����
		bool flag = false;
		int lastNode;
		for (set<string>::iterator ite = cbv.begin(); ite != cbv.end(); ite++) {
			int nodeId = id2int[*ite];//�˽ڵ�ı��
			if (find(nodeQue.begin(), nodeQue.end(), nodeId) != nodeQue.end()) //����˽ڵ��Ѿ���ӵ��������ˣ���ô����
				continue;
			lastNode = nodeId;
			int degree = 0;//�˽ڵ�Ķȣ�Ҳ���ǽڵ������ӵıߵ�����
			for (int i = 0; i < iG[nodeId].size(); i++) {//����˽ڵ��Ӧ�ıߵ�����
				if (iG[nodeId][i] == 1) {
					degree++;
				}
			}
			if (degree < GLOBAL_REG_NUM) {//�˽ڵ�Ķ�С��GLOBAL_REG_NUM�����Ա����������
				nodeQue.push_back(nodeId);
				for (int i = 0; i < iG[nodeId].size(); i++) {//�Ƴ���ýڵ����������б�
					if (iG[nodeId][i] == 1) {
						iG[nodeId][i] = 0;
						iG[i][nodeId] = 0;
					}
				}
				flag = true;
				break;
			}
		}
		if (!flag) {//û��һ���ڵ�Ķ�С��GLOBAL_REG_NUM������һ���㵱��������ȫ�ּĴ����ĵ�
			nodeQue.push_back(lastNode);//���������
			noGlobalReg.insert(lastNode);//������ȫ�ֱ���
			for (int i = 0; i < iG[lastNode].size(); i++) {//��ͼ���Ƴ��ýڵ�
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
		if (noGlobalReg.find(nodeId) == noGlobalReg.end()) {//ֻ��δ��ǡ�������ȫ�ֱ������Ľڵ���ܷ��䵽ȫ�ֱ���
			if (color < GLOBAL_REG_NUM) {
				int2color[nodeId] = color;
				color++;
			}
			//for (int color = 0; color < GLOBAL_REG_NUM; color++) {
			//	bool dupColor = false;//��ɫ�Ƿ��ظ�
			//	for (int i = 0; i < saveIG[nodeId].size(); i++) {//�ж�nodeId��Χ�Ľڵ��Ƿ���nodeId����ɫ��ͬ
			//		if (saveIG[nodeId][i] == 1 && color == int2color[i]) {
			//			dupColor = true;
			//			break;
			//		}
			//	}
			//	if (!dupColor) {//���nodeId����Χ�ڵ���ɫ���ظ�����ô����Ⱦɫ
			//		int2color[nodeId] = color;
			//		break;
			//	}
			//}
		}
		nodeQue.pop_back();
	}
	//���յõ�int2color������int2color[id2int[id]]���ܸ��ݱ�ʶ���õ����Ӧ��ȫ�ּĴ������
	fbb->id2Greg.clear();
	for (map<string, int>::iterator ite = id2int.begin(); ite != id2int.end(); ite++) {//��亯�����ȫ�ֱ�ʶ�����������ȫ�ּĴ�����ӳ��
		if (int2color[id2int[ite->first]] != -1) {
			fbb->id2Greg[ite->first] = int2color[id2int[ite->first]];
		}
	}
}
//������ͻͼ
void graphColorer::buildInterferenceGraph(FuncBaseBlock* fbb) {
	crossBBVar.clear();
	id2int.clear();
	fbb->genInOutSet();//��������in,out,use,def����
	for (int i = 0; i < fbb->innerBlocks.size(); i++) {//���˺������е����л������out���еı�������crossBBVar����Ϊ��Щ�����ǿ��������Ȼ��Ծ��
		BaseBlock* temp = fbb->innerBlocks[i];
		set<string>::iterator ite = temp->outSet.begin();
		while (ite != temp->outSet.end()){//�����ȫ�ֱ���������ȫ�ּĴ���
			crossBBVar.insert(*ite);
			ite++;
		}
	}
	int nodeIdx = 0;//�ڵ���
	for (set<string>::iterator ite = crossBBVar.begin(); ite != crossBBVar.end(); ite++) 
		id2int[*ite] = nodeIdx++;
	vector<vector<int>> interferenceGraph(nodeIdx, vector<int>(nodeIdx, 0));//��ͻͼ
	for (int i = 0; i < fbb->innerBlocks.size(); i++) {
		BaseBlock* temp = fbb->innerBlocks[i];
		set<string> live = temp->outSet;
		for (int j = temp->newQuads.size() - 1; j >= 0; j--) {//������ʴ˻��������Ԫʽ
			quadCode qc = temp->newQuads[j];
			string x = "";
			set<string> useI;//ָ��I��ʹ�ü����붨�弯
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
			else if (qc.op == "scanfInt" || qc.op == "scanfChr") {//scanf�ı�������Ϊ�Ǹ�ֵ������Ҳ����def
				x = qc.left;
			}
			if (x != "" && crossBBVar.find(x) != crossBBVar.end()) {//���������ı�����Ϊ�ղ����ǿ������ı�������ô��ʼ��ӳ�ͻͼ�ı�
				for (set<string>::iterator ite = live.begin(); ite != live.end(); ite++) {
					if (crossBBVar.find(*ite) != crossBBVar.end() && x != *ite) {
						interferenceGraph[id2int[x]][id2int[*ite]] = 1;
						interferenceGraph[id2int[*ite]][id2int[x]] = 1;
					}
				}
			}
			//live = useI U (live - defI) ����defI���Ǿ�ֻ��һ������
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
