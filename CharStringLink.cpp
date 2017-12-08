#include "CharStringLink.h"
#include <iostream>
using namespace std;

CharStringLink::CharStringLink(void)
{
	header = new StringNode();
	tailer = new StringNode();
	header->succ = tailer;//初始化时头、
	tailer->prev = header;//尾节点相连
	_size = 0;//初始规模为0
}


CharStringLink::~CharStringLink(void)
{
	while(_size>0){//删除所有的节点
		remove(0);//直到规模为0
	}
	if(header){
		header->succ = NULL; 
		delete header; 
		header = NULL;//然后删除头节点
	}
	if(tailer){
		tailer->prev = NULL; 
		delete tailer;
		tailer = NULL;//和尾节点
	}
}

CharStringLink::CharStringLink(const CharStringLink& rhs){//拷贝构造函数
	header = new StringNode();
	tailer = new StringNode();
	header->succ = tailer;//初始化时头、
	tailer->prev = header;//尾节点相连
	_size = 0;//初始规模为0
	
	StringNodePosi node = rhs.header->succ;
	while(node != rhs.tailer){
		add(node->data);
		node = node->succ;
	}
}

void CharStringLink::add(const CharString& data){//添加元素
	add(data.data());
}
void CharStringLink::remove(const CharString& data){//删除值为data的元素，若删除成功，则返回true
	remove(data.data());	
}

int CharStringLink::search(const CharString& data){//查找某个元素的位置，若失败则返回-1
	return search(data.data());
}

void CharStringLink::add(const char* data){//加入data元素
	StringNodePosi node = new StringNode(data);//创建一个新的节点
	node->prev = tailer->prev; node->succ = tailer;//将其放置在末节点之后
	tailer->prev->succ = node; tailer->prev = node;//尾节点之前
	_size++;//规模加1
}

bool CharStringLink::remove(int i){//删除第i个元素，assert： 0 <= i < size
	if(i<0 || i>=_size)//排除非法的情况
		return false;

	StringNodePosi node = header->succ;
	while(i--){
		node = node->succ;//找到第i个元素
	}
	node->prev->succ = node->succ;
	node->succ->prev = node->prev;//将其前驱和后继相连
	node->prev = NULL; node->succ = NULL;//将该元素
	delete node; node = NULL;//删除
	_size--;//规模减一
	return true;
}

int CharStringLink::search(const char* data){//查找某个元素的位置，若失败则返回-1
	StringNodePosi node = header->succ; int i = 0;//从首节点开始查找
	while(node!=tailer){
		if(node->data == data){//如果某个节点的值即为data
			return i;//返回该元素的位置
		}else{
			node = node->succ; i++;//否则继续查找
		}
	}
	return -1;//加入没有找到，返回-1
}

bool CharStringLink::remove(const char* data){//删除值为data的元素，若删除成功，则返回true
	StringNodePosi node = header->succ;//从首节点开始查找
	while(node!=tailer){
		if(node->data == data){//如果某个节点的值即为data
			node->prev->succ = node->succ;
			node->succ->prev = node->prev;
			node->prev = NULL;
			node->succ = NULL;
			delete node; node = NULL;//删除该节点
			_size--;//规模减一
			return true;
		}else{
			node = node->succ; //否则继续查找
		}
	}
	return false;//如果没有找到相应节点，返回false
}


void CharStringLink::print(ostream& out){//打印所有的节点用作调试
	StringNodePosi node = header->succ; 
	while(node!=tailer){
		out << node->data << " | ";
		node = node->succ;
	}
	out << endl;
}

void CharStringLink::printReverse(ostream& out){
	StringNodePosi node = tailer->prev; 
	while(node!=header){
		out << node->data << " | ";
		node = node->prev;
	}
	out << endl;
}

void CharStringLink::add(CharStringLinkPosi link){//加入另外一个字符串链表的所有元素
	StringNodePosi node = link->header->succ;
	while(node != link->tailer){
		add(node->data);
		node = node->succ;
	}
}

void CharStringLink::add(const CharStringLink & link){//加入另外一个字符串链表的所有元素
	StringNodePosi node = link.header->succ;
	while(node != link.tailer){
		add(node->data);
		node = node->succ;
	}
}

ostream& operator<<(ostream& out, const CharStringLink& link){
	StringNodePosi node = link.tailer->prev;
	while(node != link.header ){
		out << node->data << " ";
		node = node->prev;//逆序输出
	}
	return out;
}
