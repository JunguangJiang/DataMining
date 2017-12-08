#include "searchengine.h"
#include <QProcess>
#include <QDebug>
#include <fstream>
using namespace std;
SearchEngine::SearchEngine(int weight):weight(weight)
{
    bbst = new BBST();
}


SearchEngine::~SearchEngine(void)
{
}

void SearchEngine::buildInvertedFile(const CharString& infile, const CharString& outfile){//从本地文件读取信息，构建倒排文档
    ifstream in;
    in.open(infile.data());
    if(!in){
        cout << "error in open inverted file "<< infile << endl;
        exit(-1);
    }
    ofstream out;
    out.open(outfile.data());
    if(!out){
        cout << "error in open inverted file "<< outfile << endl;
        exit(-1);
    }
    string line; getline(in, line);//读入的第一行内容无用
    int DocID = 0;//文档编号
    while(!in.eof()){//一直输入到文件的末尾
        getline(in, line);CharString string(line); //从文件中读入一行
        DocID++;//文档编号递增
        vector<CharString> v = string.split(CharString("||"));//将一行的字符串切分成多个子串
        if(v.size()<9){//如果该行信息不完整，则跳过该行
            content.push_back(CharString(""));
            continue;
        }
        content.push_back(v[5]);//得到发帖内容
        vector<CharString> dividedWordList = v[9].split(CharString(" "));//得到分词结果
        for(int i=0; i<dividedWordList.size(); i++){//遍历所有的分词结果
            CharString word = dividedWordList[i];//第i个分词
            if(word.size() == 0) continue;//跳过所有空词
            BinNodePosi p = bbst->Insert(word);//无论插入是否成功，返回的节点p对应的单词都是word
            p->docList->Add(DocID);//向单词的文档链表中加入当前文档的编号
        }
    }
    out << DocID << endl;//输出文档的总数
    for(int i = 0; i<content.size(); i++){
        out << content[i] << endl;//输出所有的发帖内容
    }
    bbst->debug(bbst->root());//打印整颗树
}

void SearchEngine::searchWordList(const CharStringLink& wordList, ostream& out){
    DocList docList(-1, weight);
    for(auto p = wordList.first(); wordList.isValid(p); p = wordList.next(p)){//将每个关键词对应的文档链表
        docList.Add(*(bbst->Search(p->data)->docList));//都加入到docList中
    }
    out << docList << endl;
}

void SearchEngine::searchSentences(std::vector<CharString> sentences, std::ostream& out){
        CharStringLink wordList;//所有的关键词集合
        for(int i=0; i<sentences.size(); i++){//将每个句子
            wordList.add(divideWords(sentences[i], true));//分解成关键词后加入wordList(句子分词暂时采用去除无用词）
        }
        searchWordList(wordList,out);//对得到的关键词集合进行查询
}

void SearchEngine::query(const CharString& queryFile, const CharString& resultFile){
    //进行批量查询，查询文件为queryFile,结果文件为resultFile
    ifstream in;
    in.open(queryFile.data());
    if(!in){
        cout << "error in open inverted file "<< queryFile << endl;
        exit(-1);
    }
    ofstream out;
    out.open(resultFile.data());
    if(!out){
        cout << "error in open inverted file "<< resultFile << endl;
        exit(-1);
    }
    while(!in.eof()){
        string temp; getline(in, temp);CharString line = temp;//读入一行
        std::vector<CharString> sentences = line.split(CharString(" "));//根据空格将该行分成多个句子
        searchSentences(sentences, out);//然后查询这些句子
    }
}

void SearchEngine::extractInfo(){
    qDebug() << "-------------主程序运行中-----------";

    QProcess* process=new QProcess();
     if(process->execute("Project.exe",QStringList()))
     {
         qDebug() << "外部程序helloworld.exe运行成功";
     }

    qDebug() << "------------主程序继续运行-----------";    //不会被执行
}

void SearchEngine::initDictionary(){//初始化词库
    dictionary = make_shared<Dictionary>();//新建一个词库
    dictionary->init();//词库初始化
}

void SearchEngine::initDictionary(const CharString& mainDictionary,const CharString& professionalTerm){//初始化词库
    dictionary = make_shared<Dictionary>();//新建一个词库
    dictionary->init(mainDictionary, professionalTerm);//词库初始化
}

CharStringLink SearchEngine::divideWords(const CharString& sentence, bool removeUselessWords){//对句子进行分词
    //如果removeUselessWords为true，则分词结果就会删除无用词；为false，则保留无用词
    if(!dictionary){
        exit(-1);
    }
    return *dictionary->divideSentence(sentence, removeUselessWords);
}
