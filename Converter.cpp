#include "stdafx.h"
#include "Converter.h"
using namespace std;

int inline stringToValue(const CharString& string){//获得一个字符串的数值大小
	int value = 0;
	for(int i=0; i<string.size(); i++){
		value *= 10;
		value += (int)string[i] - 48;
	}
	return value;
}

CharString UnicodeToChinese(const CharString& sentence){//将Unicode编码转化为中文
	typedef std::codecvt_byname<wchar_t, char, mbstate_t> WCHAR_GBK;
	const char* GBK_NAME = ".936";
	std::wstring_convert<WCHAR_GBK> cvtGBK(new WCHAR_GBK(GBK_NAME));//解码转化器
	CharString result;//返回的中文字符串
	int left=-1, right=-1;
	while(true){//在原来的unicode字符串中
		left = sentence.indexOf("&#", left+1);//找到形如"&#25105;"的子串的开头
		if(left >= 0){
			if( (right+1) < left ){//如果[right+1, left)之间存在非unicode字符串
				result.concat( sentence.subString(right+1, left) );//将其直接加入到结果字符串中
			}
			right = sentence.indexOf(";", right+1);//找到形如"&#25105;"的子串的结尾

			CharString word = sentence.subString(left+2, right);//取出其中的数字部分25105
			int value = stringToValue(word);//获得其对应的数值
			wchar_t uchar = value;//将其转化为wchar_t
			wstring ustring; ustring.push_back(uchar);//转化为ustring
			result.concat( cvtGBK.to_bytes(ustring) );//最后将ustring转化为CharString，加入到结果的中文字符串中	
		}else{//如果找不到这样的串了
			if(right < sentence.size()-1){//说明结尾有非unicode字符串
				result.concat(sentence.subString(right+1, sentence.size()));
			}
			break;//则可以退出了
		}
	}

	return result;
}