#include "Converter.h"
using namespace std;
#include <QDebug>
#include <iostream>
#include "convert_qstr_str.h"
int inline stringToValue(const CharString& string){//获得一个字符串的数值大小
	int value = 0;
	for(int i=0; i<string.size(); i++){
		value *= 10;
		value += (int)string[i] - 48;
	}
	return value;
}

CharString UnicodeToChinese(const CharString& sentence){//将Unicode编码转化为中文
    CharString result;//返回的中文字符串
    int left=-1, right=-1;
    while(true){//在原来的unicode字符串中
        left = sentence.indexOf("&", left+1);//找到形如"&#25105;"的子串的开头
        if(left >= 0){
            if( (right+1) < left ){//如果[right+1, left)之间存在非unicode字符串
                result.concat( sentence.subString(right+1, left) );//将其直接加入到结果字符串中
            }
            right = sentence.indexOf(";", right+1);//找到形如"&#25105;"的子串的结尾
            if(sentence[left+1] == '#'){//说明是&#25105;之类的字符
                CharString word = sentence.subString(left+2, right);//取出其中的数字部分25105
                uint value[2] = {stringToValue(word),'\0'};//获得其对应的数值
                result.concat( qstr2charStr( QString::fromUcs4(value) ) );//将其转化为CharString后加入到result末尾
            }else if(sentence[left+1] == 'n'){//说明是&nbsp;之类的字符
                result.concat(CharString(" "));//加入空格
            }
        }else{//如果找不到这样的串了
            if(right < sentence.size()-1){//说明结尾有非unicode字符串
                result.concat(sentence.subString(right+1, sentence.size()));
            }
            break;//则可以退出了
        }
    }
    return result;
}



/*
CharString UnicodeToChinese(const CharString& sentence){//将Unicode编码转化为中文
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
            uint value[2] = {stringToValue(word),'\0'};//获得其对应的数值
            result.concat( qstr2charStr( QString::fromUcs4(value) ) );//将其转化为CharString后加入到result末尾
		}else{//如果找不到这样的串了
			if(right < sentence.size()-1){//说明结尾有非unicode字符串
				result.concat(sentence.subString(right+1, sentence.size()));
			}
			break;//则可以退出了
		}
	}    
    return result;
}
*/
