#pragma once

#define minStringSize 20//字符串的最小规模

#include <string>
#include <iostream>
#include <memory>
#include <vector>
class CharString
{
private:
	char* _elem;//数据区
	int _size;//规模
	int _capacity;//容量

	void expand();//扩容
public:
	CharString(int capacity = minStringSize);//字符串的初始化，可以指定初始时的容量
	CharString(const std::string& s);//用stl::string去初始化当前字符串
	~CharString(void);//析构函数
	CharString(const CharString& rhs);//拷贝构造函数
	//---------------------------要求实现的方法----------------------------------------------------
	int indexOf(const CharString& subString, const int pos)const;
	//在目标字符串中,从pos的位置开始查找子串,
	//并返回子串第一次出现的位置。如果不存在子串,那么返回-1。
	int indexOf(const char* subString, const int pos)const;

	bool subString(int left, int right, CharString& sub)const;
	//将原字符串[left,right)的子串的拷贝到sub中
	CharString subString(int left, int right)const;
	
	void concat(const CharString& string);
	//将另外一个字符串string接到当前字符串的末尾
	
	CharString& operator=(const CharString& string);
	//将string赋值给当前字符串

	//---------------------------提供的动态操作----------------------------------------------------
	void insert(char c);//在字符串末尾插入字符c
	void remove(int i);//删除第i个字符
	CharString& operator=(const std::string& s);//用stl中的string赋给当前字符串
	bool operator==(const char* string);//判等，判断当前字符串和另外一个字符数组是否相等
	bool operator==(const CharString& string);//判等，判断当前字符串和另外一个字符串是否相等
	char& operator[](int i){return _elem[i];}//返回第i个元素的引用

	friend std::ostream& operator<<(std::ostream& out, const CharString&);//重载输出流
	friend std::ifstream& operator>>(std::ifstream& in, CharString&);//重载文件输入流
	void removeSpace();//删除字符串中的所有空格

	//---------------------------提供的静态操作----------------------------------------------------
	char operator[](int i)const {return _elem[i];}//返回第i个元素的拷贝
	int size()const{return _size;}//返回规模
	char* data()const;//返回元素的数据区

	//以下实现两个字符串的大小比较（逐位比较字符的ASCII码，若前缀相同，则长度较长者较大）
	bool operator<=(const CharString& string);
	bool operator<(const CharString& string);
	bool operator>=(const CharString& string);
	bool operator>(const CharString& string);

	std::vector<CharString> split(const CharString& c) const;//用字符串c将string划分成多个字符串，并返回
};

