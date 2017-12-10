#pragma once
#include "termword.h"
#include "CharString.h"
#include "DocList.h"

#define BinNodePosi BinNode*//单词的指针

class BinNode: public TermWord{//二叉搜索树节点,继承自单词类
public:
	BinNodePosi lc; BinNodePosi rc; BinNodePosi parent;//左右孩子、父节点
	int height;//节点深度
	BinNode(const CharString& Term, int TermID = -1, BinNodePosi parent = NULL, int height = 1);
	BinNodePosi attachAsLChild(const CharString& Term, int TermID = -1);//作为当前节点的左孩子
	BinNodePosi attachAsRChild(const CharString& Term, int TermID = -1);//作为当前节点的右孩子
};

class BBST//平衡二叉搜索树
{
	BinNodePosi m_root;//根节点
	BinNodePosi m_hot;//热点，记录搜索结果的父节点
	int m_size;//平衡二叉搜索树的规模
	BinNodePosi& searchIn(BinNodePosi& r, const CharString& termWord);//在以r为根的子树中搜索是否有单词termWord
	void updateHeight(BinNodePosi v);//更新节点v的树高,assert:v必然非空
	int inline height(BinNodePosi v) const{return v?v->height:0;}
	//空节点的树高为0，叶节点的树高为1，其余节点的树高为max{左孩子树高，右孩子树高}+1
	int inline AVLFac(BinNodePosi v)const {return height(v->lc)-height(v->rc);}//节点v的AVL平衡系数,assert:v非空
	bool inline AVLBalanced(BinNodePosi v) const{return abs(AVLFac(v)) < 2;}
	//判断某个节点是否满足AVL树的平衡条件,assert:v必然非空
	bool inline IsValid(BinNodePosi v) const {return v?true:false;}//判断节点v是否存在
	bool inline IsRoot(BinNodePosi v)const {return IsValid(v) && v->parent == NULL;}//判断是否为树根
	bool inline IsLeaf(BinNodePosi v) const{return IsValid(v) && !v->lc && !v->rc;}//判断节点v是否为叶子节点
	bool inline IsLChild(BinNodePosi v) const {return IsValid(v) && !IsRoot(v) && ( v->parent->lc == v ); }//判断是否为左孩子
	bool inline IsRChild(BinNodePosi v) const {return IsValid(v) && !IsRoot(v) && ( v->parent->rc == v ); }//判断是否为右孩子 
	bool inline HasLChild(BinNodePosi v) const {return IsValid(v) && v->lc;}//判断是否左孩子
	bool inline HasRChild(BinNodePosi v) const {return IsValid(v) && v->rc;}//判断是否有右孩子
	void connect34(BinNodePosi a, BinNodePosi b, BinNodePosi c, 
		BinNodePosi t1, BinNodePosi t2, BinNodePosi t3, BinNodePosi t4);
	//3+4重构，用于节点的旋转调整 assert:a,b,c非空
	inline BinNodePosi tallerChild(BinNodePosi p)const{return AVLFac(p)<0?p->rc:p->lc;}//找到节点p较高的孩子 assert:p非空
	void removeSubTree(BinNodePosi v);//删除以节点v为树根的子树
	BinNodePosi succ(BinNodePosi v);//寻找节点v的后继
public:
	BBST(void);
	~BBST(void);
	BinNodePosi& Search(const CharString& termWord);//查找节点,返回查找的位置；失败返回空
	BinNodePosi Insert(const CharString& termWord);//插入节点，返回节点的单词必然是termWord
	BinNodePosi Adjust(BinNodePosi p);//对节点g进行旋转调整,返回调整后的子树根节点 Note:上层节点到子树的连接由调用者完成
	bool Remove(const CharString& termWord);//删除单词为termWord的节点，返回false代表搜索树中不存在该单词
	//Edit,编辑某节点的所有功能都可由Search予以完成，因此没有必要实现
	int size()const{return m_size;}//规模
	BinNodePosi root() const{return m_root;}//树根
	bool isEmpty()const{return !m_root;}//判断树是否为空
	void debug(BinNodePosi p);//调试打印输出节点p的信息
};

