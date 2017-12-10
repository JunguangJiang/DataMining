#ifndef TERMWORD_H
#define TERMWORD_H

#include "CharString.h"
#include "DocList.h"


class TermWord{//单词
public:
        CharString Term;//单词
        int TermID;//单词ID
        DocList *docList;//文档链表指针
        int DF()const{return docList->Size();}//单词出现在多少篇文章中
        int Occur()const{return docList->Times();}//单词的总出现次数
        TermWord(const CharString& Term, int TermID = -1);
        ~TermWord();
};
#endif // TERMWORD_H
