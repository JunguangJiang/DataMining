#ifndef TERMWORD_H
#define TERMWORD_H

#include "CharString.h"
#include "DocList.h"


class TermWord{//����
public:
        CharString Term;//����
        int TermID;//����ID
        DocList *docList;//�ĵ�����ָ��
        int DF()const{return docList->Size();}//���ʳ����ڶ���ƪ������
        int Occur()const{return docList->Times();}//���ʵ��ܳ��ִ���
        TermWord(const CharString& Term, int TermID = -1);
        ~TermWord();
};
#endif // TERMWORD_H
