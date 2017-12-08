#include "Dictionary.h"
#include <fstream>
#include <iostream>
#include <string>
#include <Windows.h>
#include "Stack.h"
using namespace std;

static size_t hashCode(const char* string){//生成字符串的循环移位散列码
	unsigned int h = 0;//散列码
	for(size_t n = strlen(string), i = 0; i < n; i++){
		h = ( h << 5 ) | ( h >> 27 ); 
		h += (unsigned int)string[i];//散列码循环左移5位，再累加当前字符
	}
	return (size_t) h ;
}

static size_t hashCode(const CharString& string){
	return hashCode(string.data());
}

bool inline isASCII(char c){//判断某个字符是否是ASCII码:目前将字母、数字、英文的标点符号都算作此列，不考虑换行符等
	int d = (int)c;
	return (d<127)&&(d>31);//ASCII表中的一些字符
}

bool inline isEnglishLetter(char c){//判断某个字符是否是英文字母
	int d = (int) c;
	return ( ( ( d>=65 ) && ( d <= 90 ) ) || ( ( d>=97 ) && ( d<=122 ) ) );
}

bool inline isNumber(int c){//判断某个字符是否是数字
	int d = (int) c;
	return ( (c>=48) && (c<=57) );
}

Dictionary::Dictionary(int c)
{
	M = c; N = 0;//初始容量为c，规模为0
	ht = new CharStringLink*[M];//开辟桶数组
	for(int i=0; i<M; i++)
		ht[i] = new CharStringLink();
}


Dictionary::~Dictionary(void)
{
	for(int i=0; i<M; i++){
		delete ht[i]; ht[i] = NULL;
	}
	delete [] ht; ht = NULL;
}

void Dictionary::put(const CharString& word){//插入词条
	put(word.data());
}

bool Dictionary::search(const CharString& word){//查询某个词是否在词典中
	return search(word.data());
}

void Dictionary::put(const char* word){//插入词条
	size_t  h = hashCode(word) % M;//得到该字符串的hash码
	ht[h]->add(word);//在hash码对应的字符串链表末尾加入词条
	N++;//规模加一
}

bool Dictionary::search(const char* word){//查询某个词是否在词典中
	size_t h =hashCode(word) % M;//得到该字符串的hash码
	return (ht[h]->search(word) >= 0);//如果在hash码对应的字符串链表中找到了词条word,则返回true；否则返回false
}

string UTF8ToGB(const char* str)//将utf8转化为GB编码，来源于网上 http://blog.csdn.net/zhancf/article/details/49930969
{
	 string result;
	 WCHAR *strSrc;
	 LPSTR szRes;

	 //获得临时变量的大小
	 int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	 strSrc = new WCHAR[i+1];
	 MultiByteToWideChar(CP_UTF8, 0, str, -1, strSrc, i);

	 //获得临时变量的大小
	 i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
	 szRes = new CHAR[i+1];
	 WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);

	 result = szRes;
	 delete []strSrc; strSrc = NULL;
	 delete []szRes; szRes = NULL;

	 return result;
}

void Dictionary::download(const char* vocabularyFile, bool isUTF8){//下载本地的一个词汇文件
	ifstream in; 
	in.open(vocabularyFile, ios::binary);
	if(!in){
		cout << "error in open dictionary " << vocabularyFile <<endl;
		exit(-1);
	}
	if(isUTF8){//对应utf8文件
		while(!in.eof()){//读入文件中的
			char temp[50];//每个词语
			in >> temp;
			put(UTF8ToGB(temp).c_str());//并将其加入词典中
		}
	}else{//对应gb文件
		while(!in.eof()){//读入文件中的
			string temp;//每个词语
			in >> temp;
			put(temp.c_str());//并将其加入词典中
		}
	}
	in.close();
}

void Dictionary::init(){//词典的初始化
	download("Dictionary/dictionary.dic", true);
	download("Dictionary/professionalTerms.dic", false);
	initStopLetterLink();//初始化停用标点符号链表
	initUselessWordLink();//初始化无用词链表
}

void Dictionary::init(const CharString& dictionary, const CharString& professionalTerm){//词典和停用词的初始化
	download(dictionary.data(), true);
	download(professionalTerm.data(), false);
	initStopLetterLink();//初始化停用标点符号链表
	initUselessWordLink();//初始化无用词链表
}
int Dictionary::getMaxHtSize(){//获得最大桶的规模，用于性能的分析
	int max=0;
	for(int i=0; i<M; i++){
		if(ht[i]->size() > max)
			max = ht[i]->size();
	}
	return max;
}

void Dictionary::print(){//打印所有的桶，用于性能分析
	for(int i=0; i<M;i++){
		cout << "chap "<< i;
		ht[i]->print(cout);
	}
}

CharStringLinkPosi Dictionary::divideSentence(const CharString& sentence, bool removeUselessWord){
	//对一个句子进行分词，允许其中出现标点符号、数字、英文字符，从中提取出中文关键词
	//忽略所有的标点符号、数字、英文字符
	CharStringLinkPosi link(new CharStringLink());//新建一个字符串链表,存储句子的分词结果
	Stack<CharString> S;//存储所有的短语
	int left =-1, right = 0;//left表示上一次
	while(right < sentence.size()){//遍历整个句子
		if(isASCII(sentence[right])){//如果当前字符是ASCII字符
			if( (left+1) < right ){//如果[left+1,right)之间存在GB字符(短语）
				CharString string = sentence.subString(left+1, right);//将其取出
				S.push(string);//并加入栈中
			}
			left = right;//left标记上一次找到的停用词位置
			right++;//查看下一个字符
		}else if( (right<sentence.size()-1) && ( isStopLetter(sentence.subString(right, right+2)) ) ){//如果下两个字符构成中文中的一个停用词
			if( (left+1) < right ){//如果[left+1,right)之间存在GB字符(短语）
				CharString string = sentence.subString(left+1, right);//将其取出
				S.push(string);//并加入栈中
			}
			left = right+1;
			right = right+2;
		}else{//其余情况
			right += 2;//说明当前字符是GB码，查看下下一个字符
		}
	}
	if( (left+1) < right){//加入最后一个短语
		CharString string; sentence.subString(left+1, right, string);
		S.push(string);
	}

	while(!S.empty()){//从栈中取出所有的短语
		CharString string = S.top(); S.pop();
		CharStringLinkPosi tempLink = dividePhrase(string, removeUselessWord);//对其分词，将局部分词结果存入tempLink
		if(tempLink->size()>0)
			link->add(tempLink);//将局部分词结果tempLink加入总的分词结果link中
	}
	return link;
}

CharStringLinkPosi Dictionary::dividePhrase(const CharString& phrase, bool removeUselessWord){
//将phrase按照当前加载的词库进行分词，并以字符串链表的形式返回结果
//采用逆序最大分词算法
	CharStringLinkPosi link(new CharStringLink());//新建一个字符串链表
	
	if(removeUselessWord){//如果应该删掉无用词
		int left, right = phrase.size();//left,right是子串的左右界
		while(right > 0){//遍历整个phrase
			left = max(right-MaxWordLength, 0);//子串的左界
			while(left < right){
				CharString subString = phrase.subString(left, right);//子串
				if( search( subString)  && !isUselessWord(subString)){//如果subString是一个词,并且不是无用词
					link->add(subString);//将subString加入字符串链表
					break;
				}else{//如果subString不是一个词
					left+=2;//则左界向右移动一个汉字
				}
			}

			if(left == right){
				//Note:如果此时left==right，则内层循环中始终没有找到一个词,
				//并且[left-2,right)并不是一个词典中的词，此处将其加入链表
			
				CharString subString = phrase.subString(left-2, right);//子串
				if(!isUselessWord(subString))//如果子串不是无用词
					link->add(subString.data());			
				right = left-2;
			}else{
				//Note:如果此时left!=right,则找到了词[left,right)
				right = left;
			}
		}
	}else{//如果不应该删除无用词
		int left, right = phrase.size();//left,right是子串的左右界
		while(right > 0){//遍历整个phrase
			left = max(right-MaxWordLength, 0);//子串的左界
			while(left < right){
				CharString subString = phrase.subString(left, right);//子串
				if( search( subString)){//如果subString是一个词
					link->add(subString);//将subString加入字符串链表
					break;
				}else{//如果subString不是一个词
					left+=2;//则左界向右移动一个汉字
				}
			}

			if(left == right){
				//Note:如果此时left==right，则内层循环中始终没有找到一个词,
				//并且[left-2,right)并不是一个词典中的词，此处将其加入链表
			
				CharString subString = phrase.subString(left-2, right);//子串
				//phrase.subString(left-2, right, subString);//subString = string[left-2, right)
				link->add(subString.data());
				//cout << subString << endl;
				right = left-2;
			}else{
				//Note:如果此时left!=right,则找到了词[left,right)
				right = left;
			}
		}
	}
	
	return link;
}

void Dictionary::initStopLetterLink(){//初始化停用词
	stopLetterLink = make_shared<CharStringLink>();
	const char* stopWordFile = "./Dictionary/stopLetter.dic";
	ifstream in; 
	in.open(stopWordFile, ios::binary);
	if(!in){
		cout << "error in open dictionary " << stopWordFile <<endl;
		exit(-1);
	}
	while(!in.eof()){//读入文件中的
		CharString temp;//每个停用词
		in >> temp;
		stopLetterLink->add(temp);
	}
	in.close();
}

bool Dictionary::isStopLetter(CharString word){
	return (stopLetterLink->search(word)!=-1);
}

void Dictionary::initUselessWordLink(){//初始化ueslessWordLink
	uselessWordLink = new CharStringLinkPosi[uselessWordLinkSize];
	for(int i=0; i<uselessWordLinkSize; i++){
		uselessWordLink[i] = make_shared<CharStringLink>();
	}

	const char* uselessWordFile = "./Dictionary/uselessWord.dic";
	ifstream in; 
	in.open(uselessWordFile, ios::binary);
	if(!in){
		cout << "error in open dictionary " << uselessWordFile <<endl;
		exit(-1);
	}
	while(!in.eof()){//读入文件中的
		CharString word;//每个停用词
		in >> word;
		size_t  h = hashCode(word) % uselessWordLinkSize;//得到该字符串的hash码
		uselessWordLink[h]->add(word);//在hash码对应的字符串链表末尾加入无用词
	}
	in.close();
}
bool Dictionary::isUselessWord(CharString word){//判断某个词是否有用
	size_t h = hashCode(word) % uselessWordLinkSize;//得到该次的hash码
	return (uselessWordLink[h]->search(word) >= 0);//如果在hash码对应的无用词链表中找到了词条word,则返回true；否则返回false
}
