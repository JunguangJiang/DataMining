#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H
#include <QObject>
#include "BBST.h"
#include "CharString.h"
#include "CharStringLink.h"
#include "Dictionary.h"
#include <ostream>
#include "DocList.h"
#include <memory>
#include <vector>
class SearchEngine
{
    std::shared_ptr<Dictionary> dictionary;//词典指针
    BBST* bbst;//平衡二叉树
    const int weight;
    std::vector<CharString> content;//存储所有的发帖内容
    void searchWordList(const CharStringLink& wordList, std::ostream& out);//搜索词，返回搜素结果

public:
    SearchEngine(int weight = 10);
    ~SearchEngine(void);
    void extractInfo();//首先从下载的网页中提取信息
    void initDictionary();//初始化词库
    void initDictionary(const CharString& mainDictionary,const CharString& professionalTerm);

    CharStringLink divideWords(const CharString& sentence, bool removeUselessWords);//对句子进行分词
    //如果removeUselessWords为true，则分词结果就会删除无用词；为false，则保留无用词

    void buildInvertedFile(const CharString& infile, const CharString& outfile);//从本地文件infile读取信息，构建倒排文档,输出到outfile
    void searchSentences(std::vector<CharString> sentences, std::ostream& out);//先对每个句子进行分词，然后搜索多个关键词，返回搜素结果
    void query(const CharString& queryFile, const CharString& resultFile);//进行批量查询，查询文件为queryFile,结果文件为resultFile
};

#endif // SEARCHENGINE_H
