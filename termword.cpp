#include "termword.h"

TermWord::TermWord(const CharString& Term, int TermID):
        Term(Term), TermID(TermID){
                docList = new DocList(TermID);//����һ���µ��ĵ�����
}

TermWord::~TermWord(){
        delete docList;//ɾ�����ʶ�Ӧ���ĵ�����
}
