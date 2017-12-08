#include "SearchEngine.h"
#include "DocList.h"
#include <fstream>
#include <vector>
using namespace std;
SearchEngine::SearchEngine(int weight): weight(weight)
{
        bbst = make_shared<BBST>();
}


SearchEngine::~SearchEngine(void)
{
}

void SearchEngine::buildInvertedFile(const CharString& infile, const CharString& outfile){//�ӱ����ļ���ȡ��Ϣ�����������ĵ�
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
	string line; getline(in, line);//����ĵ�һ����������
	int DocID = 0;//�ĵ����
	while(!in.eof()){//һֱ���뵽�ļ���ĩβ
		getline(in, line);CharString string(line); //���ļ��ж���һ��
		DocID++;//�ĵ���ŵ���
		vector<CharString> v = string.split(CharString("||"));//��һ�е��ַ����зֳɶ���Ӵ�
                if(v.size()<9){
                    records.push_back(Record());//����һ���ռ�¼
                    continue;//���������Ϣ������������������
                }
                records.push_back(Record(v[4], v[5]));//��¼Ŀǰֻ��¼��������ͷ�������
		vector<CharString> dividedWordList = v[9].split(CharString(" "));//�õ��ִʽ��
		for(int i=0; i<dividedWordList.size(); i++){//�������еķִʽ��
			CharString word = dividedWordList[i];//��i���ִ�
			if(word.size() == 0) continue;//�������пմ�
			BinNodePosi p = bbst->Insert(word);//���۲����Ƿ�ɹ������صĽڵ�p��Ӧ�ĵ��ʶ���word
			p->docList->Add(DocID);//�򵥴ʵ��ĵ������м��뵱ǰ�ĵ��ı��
		}
	}
	out << DocID << endl;//����ĵ�������
        for(int i = 0; i<records.size(); i++){
                out << records[i].content << endl;//������еķ�������
	}
	bbst->debug(bbst->root());//��ӡ������
}

std::shared_ptr<DocList> SearchEngine::searchWordList(const CharStringLink& wordList, ostream& out){
        std::shared_ptr<DocList> docList = make_shared<DocList>(-1, weight);
	for(auto p = wordList.first(); wordList.isValid(p); p = wordList.next(p)){//��ÿ���ؼ��ʶ�Ӧ���ĵ�����
                BinNodePosi node = bbst->Search(p->data);//��ѯÿ��������bbst�е�λ��
                if(node){//ֻ��bbst�д��ڸõ���ʱ���Ž����Ӧ���ĵ�����
                        docList->Add(*(node->docList));//���ϲ���docList��
        	}
	}
        out << *docList << endl;
        return docList;
}

std::shared_ptr<DocList> SearchEngine::searchSentences(std::vector<CharString> sentences, std::ostream& out){
        CharStringLink wordList;//���еĹؼ��ʼ���
        for(int i=0; i<sentences.size(); i++){//��ÿ������
                wordList.add(divideWords(sentences[i], true));//�ֽ�ɹؼ��ʺ����wordList(���ӷִ���ʱ����ȥ�����ôʣ�
        }
        return searchWordList(wordList,out);//�Եõ��Ĺؼ��ʼ��Ͻ��в�ѯ
}

void SearchEngine::query(const CharString& queryFile, const CharString& resultFile){
	//����������ѯ����ѯ�ļ�ΪqueryFile,����ļ�ΪresultFile
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
		string temp; getline(in, temp);CharString line = temp;//����һ��
		std::vector<CharString> sentences = line.split(CharString(" "));//���ݿո񽫸��зֳɶ������
		searchSentences(sentences, out);//Ȼ���ѯ��Щ����
	}
}

void SearchEngine::initDictionary(){//��ʼ���ʿ�
        dictionary = make_shared<Dictionary>();//�½�һ���ʿ�
        dictionary->init();//�ʿ��ʼ��
}

void SearchEngine::initDictionary(const CharString& mainDictionary,const CharString& professionalTerm){//��ʼ���ʿ�
        dictionary = make_shared<Dictionary>();//�½�һ���ʿ�
        dictionary->init(mainDictionary, professionalTerm);//�ʿ��ʼ��
}

CharStringLink SearchEngine::divideWords(const CharString& sentence, bool removeUselessWords){//�Ծ��ӽ��зִ�
        //���removeUselessWordsΪtrue����ִʽ���ͻ�ɾ�����ôʣ�Ϊfalse���������ô�
        if(!dictionary){
                exit(-1);
        }
        return *dictionary->divideSentence(sentence, removeUselessWords);
}

Record SearchEngine::getNthRecord(int i)const{
    if( 0 < i && i <= records.size() ){
        return records[i-1];
    }else{
        return Record();
    }
}//���ر��Ϊi�����ӵ�����assert:0<i<=�����


