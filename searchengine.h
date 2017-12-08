#pragma once
#include "BBST.h"
#include "CharString.h"
#include <ostream>
#include "DocList.h"
#include <memory>
#include "Dictionary.h"
#include "Record.h"
class SearchEngine//��������
{
    std::shared_ptr<BBST> bbst;//ƽ�������
	const int weight;
    std::shared_ptr<Dictionary> dictionary;//�ʵ�ָ��
    std::vector<Record> records;//�洢���еķ�����¼
public:
	SearchEngine(int weight = 10);
	~SearchEngine(void);
	void buildInvertedFile(const CharString& infile, const CharString& outfile);//�ӱ����ļ�infile��ȡ��Ϣ�����������ĵ�,�����outfile
    std::shared_ptr<DocList> searchWordList(const CharStringLink& wordList, std::ostream& out);//�����ʣ��������ؽ��
    std::shared_ptr<DocList> searchSentences(std::vector<CharString> sentences, std::ostream& out);//�ȶ�ÿ�����ӽ��зִʣ�Ȼ����������ؼ��ʣ��������ؽ��
	void query(const CharString& queryFile, const CharString& resultFile);//����������ѯ����ѯ�ļ�ΪqueryFile,����ļ�ΪresultFile

    void initDictionary();//��ʼ���ʿ�
    void initDictionary(const CharString& mainDictionary,const CharString& professionalTerm);

    CharStringLink divideWords(const CharString& sentence, bool removeUselessWords);//�Ծ��ӽ��зִ�
    //���removeUselessWordsΪtrue����ִʽ���ͻ�ɾ�����ôʣ�Ϊfalse���������ô�
    Record getNthRecord(int i)const;//���ر��Ϊi�����Ӽ�¼assert:0<i<=�����
};

