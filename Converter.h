#ifndef _CONVERT_H
#define _CONVERT_H

#include <codecvt>
#include "CharString.h"

int inline stringToValue(const CharString& string);//获得一个字符串的数值大小

CharString UnicodeToChinese(const CharString& sentence);//将Unicode编码转化为中文

#endif