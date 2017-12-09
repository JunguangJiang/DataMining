#pragma once
#include "CharString.h"
#include "CharStringLink.h"
#include "Dictionary.h"
#include <iostream>
#include <memory>
#include "Converter.h"
#include "Record.h"

enum MatchingType{//����ƥ�������
	SelfMatched, //����ƥ�䣬����<**/>
	Left,//��磬����<**>
	Right//�ҽ磬����</**>
};

struct Node{//html�ı��еĽڵ�
	CharString m_tag;//��ǩ����,����"a","div"
	CharString m_class;//��,���� <a class="z"> ��"z"������
	int left;//���������ı��е�λ��
	int right;//���������ı��е�λ��
	MatchingType matchingType;//����ƥ�������
	Node(){m_tag=""; m_class="";}
};

typedef std::shared_ptr<Node> NodePosi;//�ڵ�λ��

class WebsiteProcessor//��ҳ������
{
private:
	std::shared_ptr<Dictionary> dictionary;//�ʵ�ָ��

	NodePosi readOnePairOfBracket(const CharString& string, int& i);
	//���ַ���string�ĵ�i��λ�ÿ�ʼ��ȡһ��ƥ������ţ����������������ɵĽڵ�

	int getFirstLeftBracket(const CharString& string, int i);
	//����ַ���string�ӵ�i��λ�����һ�������ŵ�λ��
public:
	CharString getText(const CharString& string);//��ȡһ���ַ������ı���Ϣ��ȥ�����еı�ǩ��Ϣ
	
    void downloadWebsite(const CharString& url, const CharString& filename);
	//����url��ҳ�������ļ�filename
    std::shared_ptr<CharString> downloadWebsite(const CharString &url);
    //����url��ҳ��������ҳԴ����ַ���

    void processSourceCode(std::shared_ptr<const CharString> sourceCode, std::ofstream& out, bool removeUselessWords);
	void processHtml(const CharString& htmlText, std::ofstream& out, bool removeUselessWords);
	//�������ļ�htmlText��������������out�� 

	void readURL(std::ifstream& in, CharString& url);
	//����������in�е���ҳ���õ���ҳ��url


public:
	WebsiteProcessor(void);
	~WebsiteProcessor(void);

	void process(std::ifstream& in, std::ofstream& out, bool removeUselessWords);
	//����������in�е�������ҳ�������������������out��
	//���removeUselessWordsΪtrue����ִʽ���ͻ�ɾ�����ôʣ�Ϊfalse���������ô�

    void extractInfo(const CharString& inputFile, const CharString& outputFile, bool removeUselessWords);
	//����inputFile�е�������ҳ��������洢��outputFile�У�ʧ�ܵĻ�����false
	
	void initDictionary();//��ʼ���ʿ�
	void initDictionary(const CharString& mainDictionary,const CharString& professionalTerm);

	CharStringLink divideWords(const CharString& sentence, bool removeUselessWords);//�Ծ��ӽ��зִ�
	//���removeUselessWordsΪtrue����ִʽ���ͻ�ɾ�����ôʣ�Ϊfalse���������ô�

};

