#include "termword.h"

TermWord::TermWord(const CharString& Term, int TermID):
        Term(Term), TermID(TermID){
                docList = new DocList(TermID);//创建一个新的文档链表
}

TermWord::~TermWord(){
        delete docList;//删除单词对应的文档链表
}
