#pragma once
#include "CharString.h"
class Record{//Ϊÿһ����ҳ���ɵ�һ����¼
public:
    CharString title;//��������
    CharString content;//��������
    CharString userName;//������
    CharString date;//��������
    CharString type;//��������
    CharString url;//��ҳ
    CharString category;//��������
    CharString subclass;//����С��
    Record(CharString title = CharString(), CharString content = CharString(),
           CharString userName = CharString(), CharString date = CharString(),
           CharString type = CharString(), CharString url = CharString(),
           CharString category = CharString(), CharString subclass = CharString());
    Record(const Record& rhs);
};
