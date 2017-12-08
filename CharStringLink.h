#pragma once
#include "CharString.h"
#include <memory>

#define StringNodePosi StringNode*
#define CharStringLinkPosi  std::shared_ptr<CharStringLink>

class StringNode{//字符串链表节点
public:
	CharString data;//数据区
	StringNodePosi prev;//前驱
	StringNodePosi succ;//后继
	StringNode(const char* d = "", StringNodePosi p = NULL, 
		StringNodePosi s = NULL):prev(p), succ(s){data = d;}//字符串的初始化
	~StringNode(){prev = succ = NULL;}
};


class CharStringLink//字符串链表，用于存储分词结果、作为hash表中的桶元素
	//（此处可以在空间上进一步优化，改成只有头结点的单向链表）
{
private:
	StringNodePosi header;//头节点
	StringNodePosi tailer;//尾节点
	int _size;//规模
public:
	CharStringLink(void);
	~CharStringLink(void);
	CharStringLink(const CharStringLink& rhs);//拷贝构造函数

	int size()const {return _size;}
	void add(const CharString& data);//添加元素
	void remove(const CharString& data);//删除值为data的元素，若删除成功，则返回true
	int search(const CharString& data);//查找某个元素的位置，若失败则返回-1

	void add(const char* data);//添加元素
	bool remove(const char* data);//删除值为data的元素，若删除成功，则返回true
	bool remove(int i);//删除第i个元素，assert： 0 <= i < size
	int search(const char* data);//查找某个元素的位置，若失败则返回-1

	void add(CharStringLinkPosi link);//加入另外一个字符串链表的所有元素
	void add(const CharStringLink& link);

	void print(std::ostream&);//打印所有的元素，用作调试
	void printReverse(std::ostream&);//逆向打印

	friend std::ostream& operator<<(std::ostream& out, const CharStringLink& link);

	//以下函数用于对字符串链表的遍历
	StringNodePosi first()const{return header->succ;}//返回首元素
	bool isValid(StringNodePosi p)const{return p && p != header && p!= tailer;}//判断节点p是否为合法节点
	StringNodePosi next(StringNodePosi p)const{return p->succ;}//返回节点p的后继
};


