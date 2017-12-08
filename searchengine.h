#pragma once
#include "BBST.h"
#include "CharString.h"
#include <ostream>
#include "DocList.h"
#include <memory>
#include "Dictionary.h"
#include "Record.h"
class SearchEngine//搜素引擎
{
    std::shared_ptr<BBST> bbst;//平衡二叉树
	const int weight;
    std::shared_ptr<Dictionary> dictionary;//词典指针
    std::vector<Record> records;//存储所有的发帖记录
public:
	SearchEngine(int weight = 10);
	~SearchEngine(void);
	void buildInvertedFile(const CharString& infile, const CharString& outfile);//从本地文件infile读取信息，构建倒排文档,输出到outfile
    std::shared_ptr<DocList> searchWordList(const CharStringLink& wordList, std::ostream& out);//搜索词，返回搜素结果
    std::shared_ptr<DocList> searchSentences(std::vector<CharString> sentences, std::ostream& out);//先对每个句子进行分词，然后搜索多个关键词，返回搜素结果
	void query(const CharString& queryFile, const CharString& resultFile);//进行批量查询，查询文件为queryFile,结果文件为resultFile

    void initDictionary();//初始化词库
    void initDictionary(const CharString& mainDictionary,const CharString& professionalTerm);

    CharStringLink divideWords(const CharString& sentence, bool removeUselessWords);//对句子进行分词
    //如果removeUselessWords为true，则分词结果就会删除无用词；为false，则保留无用词
    Record getNthRecord(int i)const;//返回编号为i的帖子记录assert:0<i<=最大编号
};

