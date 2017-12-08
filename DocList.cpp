#include "DocList.h"
#include <limits>
#include <iostream>
#include <ostream>
#include <QDebug>
DocNode::DocNode(int DocID, int Times, DocNodePosi prev, DocNodePosi succ):
	DocID(DocID), Times(Times), prev(prev), succ(succ), count(1){}

DocNodePosi DocNode::insertAsNext(int DocID, int Times){
	DocNodePosi p = new DocNode(DocID, Times, this, this->succ);//创建一个新节点，插入到当前节点的后继
	if(succ) succ->prev = p; succ = p;
	return p;//返回新创建的节点
}

DocNodePosi DocNode::insertAsPred(int DocID, int Times){
	DocNodePosi p = new DocNode(DocID, Times, this->prev, this);//创建一个新节点，插入到当前节点的前驱
	if(prev) prev->succ = p; prev = p;
	return p;//返回新创建的节点
}


DocList::DocList(int TermID, int weight):TermID(TermID), weight(weight), m_times(0), m_size(0){
	header = new DocNode(-1, INT_MAX);//头结点的DocID为-1,单词出现次数为无穷大，则节点都只能在头结点之后插入
	trailer = new DocNode(-1, INT_MIN);//尾节点的DocID为-1，单词出现的次数为无穷小
	header->succ = trailer; trailer->prev = header;
}


DocList::~DocList(void){
	DocNodePosi p = header->succ;//反复将头结点的后继
	while(p != trailer){
		p->prev->succ = p->succ; p->succ->prev = p->prev;//从链表中摘除
		p->succ = p->prev = NULL;
		delete p;//然后删除
		p = header->succ;
	}//直至，只剩下头尾节点
	header->prev = trailer->succ = NULL;
	delete header; delete trailer;//删除头尾节点
}

int DocList::totalWeight(DocNodePosi p)const{
	if(p == header) return INT_MAX;
	else if( p == trailer ) return INT_MIN;
	else return weight * p->count + p->Times; 
}//返回总权重，文档链表根据总权重进行排序


void DocList::swap(DocNodePosi p1, DocNodePosi p2){
	std::swap(p1->DocID, p2->DocID);
	std::swap(p1->Times, p2->Times);
	std::swap(p1->count, p2->count);
}

DocNodePosi DocList::Add(int DocID, int Times){//添加文档
	DocNodePosi p = NULL;
	for(p = header->succ; p != trailer && p->DocID != DocID; p = p->succ){//遍历整个链表，
	}
	this->m_times+=Times;//修改文档链表的单词总次数
	if(p == trailer){//如果原先不存在文档DocID
		this->m_size++;//文档总数增加
		p = trailer->insertAsPred(DocID, Times);//则将DocID作为最后一个节点插入链表
	}else{//如果原先存在文档
		p->Times+=Times;//则次数增加		
		p->count++;//count记录了这个DocID对应的关键词个数（用于链表合并）
	}
	return adjust(p);//返回调整次序后的位置
}

DocNodePosi DocList::Search(int DocID){//查询文档
	DocNodePosi p = NULL;
	for(p = header; p != trailer; p = p->succ){//遍历所有文档（头尾节点必不匹配）
		if(p->DocID == DocID) break; //如果在当前文档命中，则退出循环
	}
	return (p->DocID == DocID) ? p : NULL;//如果没有找到文档，则返回空
}

DocNodePosi DocList::Edit(int DocID, int Times){//修改文档
	DocNodePosi p = Search(DocID); 
	if(!p){//如果没有搜索到文档
		return Add(DocID, Times);//转而直接加入新文档
	}else{//否则对现有文档
		p->Times = Times;//进行修改
		return adjust(p);//返回调整后的位置
	}
}

int DocList::Remove(int DocID){//删除文档
	DocNodePosi p = Search(DocID);//首先查询文档
	int oldTimes = 0;//删除前文档中单词出现次数
	if(p){//如果文档存在
		oldTimes = p->Times;
		p->prev->succ = p->succ; p->succ->prev = p->prev;//将其从链表中摘除
		p->succ = p->prev = NULL;
		this->m_times -= oldTimes;//修改文档链表的单词总次数
		this->m_size--;
		delete p;
	}
	return oldTimes;//调用者通过判断被删除文档的次数是否大于0，判断删除是否成功
}

void DocList::debug() const{
	DocNodePosi p = NULL;
	for(p = header->succ; p != trailer; p = p->succ){//遍历所有文档（头尾节点必不匹配）
		std::cout << "(" << p->DocID << ", " << p->Times; 
		if(weight>0) std::cout <<","<< p->count << "," << totalWeight(p); 
		std::cout <<  ")  " ; 
	}
}

DocNodePosi DocList::adjust(DocNodePosi p){
	//调整节点p使其有序 assert:无法处理p->prev<p 且p>p->succ的情况，二者至多有一个失序
	//assert：p为非头尾节点
	if(totalWeight(p->prev) < totalWeight(p)){
		DocNodePosi prev = p->prev;
		while(totalWeight(prev) < totalWeight(p)){//只有当前驱小于p时，才做交换
			swap(p, prev);//交换二者的数据区
			p = prev;//此时需要检查prev是否有序
			prev = p->prev;
		}//当prev=header时，必然终止
	}else if(totalWeight(p->succ) > totalWeight(p)){
		DocNodePosi succ = p->succ;
		while(totalWeight(succ) > totalWeight(p)){//只有当后继大于p时，才做交换
			swap(p, succ);//交换二者的数据区
			p = succ;//此时需要检查prev是否有序
			succ = p->succ;
		}//当succ = trailer时，必然终止
	}
	//其余情况满足p->prev >= p >= p->succ,已经有序
	return p;//返回调整后的位置
}

void DocList::Add(const DocList& rhs){
	for(DocNodePosi p = rhs.header->succ; p != rhs.trailer; p = p->succ){
		Add(p->DocID, p->Times);//将rhs中的所有元素都加入都当前链表中
	}
}

std::ostream& operator<<(std::ostream& out, const DocList& list){//将整个链表输出到out中
	for(DocNodePosi p = list.header->succ; p != list.trailer; p = p->succ){
		out << "(" << p->DocID << "," << p->Times << ") "; 
	}
	return out;
}
