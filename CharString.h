#pragma once

#define minStringSize 20//�ַ�������С��ģ

#include <string>
#include <iostream>
#include <memory>
#include <vector>
class CharString
{
private:
	char* _elem;//������
	int _size;//��ģ
	int _capacity;//����

	void expand();//����
public:
	CharString(int capacity = minStringSize);//�ַ����ĳ�ʼ��������ָ����ʼʱ������
	CharString(const std::string& s);//��stl::stringȥ��ʼ����ǰ�ַ���
	~CharString(void);//��������
	CharString(const CharString& rhs);//�������캯��

	//---------------------------Ҫ��ʵ�ֵķ���----------------------------------------------------
	int indexOf(const CharString& subString, const int pos)const;
	//��Ŀ���ַ�����,��pos��λ�ÿ�ʼ�����Ӵ�,
	//�������Ӵ���һ�γ��ֵ�λ�á�����������Ӵ�,��ô����-1��
	int indexOf(const char* subString, const int pos)const;

	bool subString(int left, int right, CharString& sub)const;
	//��ԭ�ַ���[left,right)���Ӵ��Ŀ�����sub��
	CharString subString(int left, int right)const;
	
	void concat(const CharString& string);
	//������һ���ַ���string�ӵ���ǰ�ַ�����ĩβ
	
	CharString& operator=(const CharString& string);
	//��string��ֵ����ǰ�ַ���

	//---------------------------�ṩ�Ķ�̬����----------------------------------------------------
	void insert(char c);//���ַ���ĩβ�����ַ�c
	void remove(int i);//ɾ����i���ַ�
	CharString& operator=(const std::string& s);//��stl�е�string������ǰ�ַ���
	bool operator==(const char* string);//�еȣ��жϵ�ǰ�ַ���������һ���ַ������Ƿ����
	bool operator==(const CharString& string);//�еȣ��жϵ�ǰ�ַ���������һ���ַ����Ƿ����
	char& operator[](int i){return _elem[i];}//���ص�i��Ԫ�ص�����

	friend std::ostream& operator<<(std::ostream& out, const CharString&);//���������
	friend std::ifstream& operator>>(std::ifstream& in, CharString&);//�����ļ�������
	void removeSpace();//ɾ���ַ����е����пո�

	//---------------------------�ṩ�ľ�̬����----------------------------------------------------
	char operator[](int i)const {return _elem[i];}//���ص�i��Ԫ�صĿ���
	int size()const{return _size;}//���ع�ģ
	char* data()const;//����Ԫ�ص�������

	//����ʵ�������ַ����Ĵ�С�Ƚϣ���λ�Ƚ��ַ���ASCII�룬��ǰ׺��ͬ���򳤶Ƚϳ��߽ϴ�
	bool operator<=(const CharString& string);
	bool operator<(const CharString& string);
	bool operator>=(const CharString& string);
	bool operator>(const CharString& string);

	std::vector<CharString> split(const CharString& c) const;//���ַ���c��string���ֳɶ���ַ�����������
};
