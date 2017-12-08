#pragma once

#define DocNodePosi DocNode*//文档节点指针
#include <ostream>
class DocNode{//文档链表节点
public:
	int DocID;//出现该单词的文档ID
	int Times;//单词的出现次数
	int count;//该文档出现了多少个关键词,在后续文档链表的合并时使用
	//int totalWeight();//返回总权重，文档链表根据总权重进行排序
	DocNodePosi prev; DocNodePosi succ;//前驱和后继
	DocNode(int DocID=-1, int Times=0, DocNodePosi prev = NULL, DocNodePosi succ = NULL);//创建一个节点
	DocNodePosi insertAsNext(int DocID, int Times);//作为当前节点的后继节点插入一个新的节点
	DocNodePosi insertAsPred(int DocID, int Times);//作为当前节点的前驱节点插入一个新的节点
};

class DocList//文档链表
{
protected:
	int TermID;//单词ID
	DocNodePosi header; DocNodePosi trailer;//链表头结点、尾节点
	int m_size;//链表的大小
	int m_times;//单词的总次数
	void swap(DocNodePosi p1, DocNodePosi p2);//交换两个文档节点数据区的内容
	DocNodePosi adjust(DocNodePosi p);
	//调整节点p使其有序 assert:无法处理p->prev<p 且p>p->succ的情况，二者至多有一个失序
	//assert：p为非头尾节点
	const int weight;//计算总权重时的参数，只有在链表合并时才使用weight>0!!!
	int totalWeight(DocNodePosi p)const;//返回总权重，文档链表根据总权重进行排序
public:
	DocList(int TermID = -1, const int  weight = 0);
	~DocList(void);
	DocNodePosi Add(int DocID, int Times);
	//添加文档,返回插入的文档节点位置.如果原先存在文档，则对应的次数增加Times(只要用于链表合并中，并且count值会发生改变）
	DocNodePosi Add(int DocID){return Add(DocID, 1);}
	//添加文档(次数为1），返回插入的文档节点的位置.如果原先存在文档，则对应的次数增加1(用于遍历数据库，count值不变）
	DocNodePosi Search(int DocID);//查询文档，返回文档节点位置
	DocNodePosi Edit(int DocID, int Times);//修改文档，如果原先存在文档，则返回修改后的文档节点位置；如果原先不存在文档，则添加文档
	int Remove(int DocID);//删除文档,返回文档中单词出现次数
	int Times()const{return m_times;}//返回文档链表的单词总次数
	int Size() const{return m_size;}//返回文档链表的规模
	void debug()const;//打印文档链表的信息
	void Add(const DocList& rhs);//增加另外一个文档链表中的所有元素（用于链表合并）
	friend std::ostream& operator<<(std::ostream& out,const DocList& list);//将整个链表输出到out中
};



