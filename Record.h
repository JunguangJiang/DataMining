#pragma once
#include "CharString.h"
class Record{//为每一个网页生成的一条记录
public:
    CharString title;//发帖标题
    CharString content;//发帖内容
    CharString userName;//发帖人
    CharString date;//发帖日期
    CharString type;//发帖类型
    CharString url;//网页
    CharString category;//发帖大类
    CharString subclass;//发帖小类
    Record(CharString title = CharString(), CharString content = CharString(),
           CharString userName = CharString(), CharString date = CharString(),
           CharString type = CharString(), CharString url = CharString(),
           CharString category = CharString(), CharString subclass = CharString());
    Record(const Record& rhs);
};
