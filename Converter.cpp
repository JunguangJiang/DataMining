#include "Converter.h"
using namespace std;
#include <QDebug>
#include <iostream>
#include "convert_qstr_str.h"
int inline stringToValue(const CharString& string){//���һ���ַ�������ֵ��С
	int value = 0;
	for(int i=0; i<string.size(); i++){
		value *= 10;
		value += (int)string[i] - 48;
	}
	return value;
}

CharString UnicodeToChinese(const CharString& sentence){//��Unicode����ת��Ϊ����
    CharString result;//���ص������ַ���
    int left=-1, right=-1;
    while(true){//��ԭ����unicode�ַ�����
        left = sentence.indexOf("&", left+1);//�ҵ�����"&#25105;"���Ӵ��Ŀ�ͷ
        if(left >= 0){
            if( (right+1) < left ){//���[right+1, left)֮����ڷ�unicode�ַ���
                result.concat( sentence.subString(right+1, left) );//����ֱ�Ӽ��뵽����ַ�����
            }
            right = sentence.indexOf(";", right+1);//�ҵ�����"&#25105;"���Ӵ��Ľ�β
            if(sentence[left+1] == '#'){//˵����&#25105;֮����ַ�
                CharString word = sentence.subString(left+2, right);//ȡ�����е����ֲ���25105
                uint value[2] = {stringToValue(word),'\0'};//������Ӧ����ֵ
                result.concat( qstr2charStr( QString::fromUcs4(value) ) );//����ת��ΪCharString����뵽resultĩβ
            }else if(sentence[left+1] == 'n'){//˵����&nbsp;֮����ַ�
                result.concat(CharString(" "));//����ո�
            }
        }else{//����Ҳ��������Ĵ���
            if(right < sentence.size()-1){//˵����β�з�unicode�ַ���
                result.concat(sentence.subString(right+1, sentence.size()));
            }
            break;//������˳���
        }
    }
    return result;
}



/*
CharString UnicodeToChinese(const CharString& sentence){//��Unicode����ת��Ϊ����
    CharString result;//���ص������ַ���
	int left=-1, right=-1;
	while(true){//��ԭ����unicode�ַ�����
		left = sentence.indexOf("&#", left+1);//�ҵ�����"&#25105;"���Ӵ��Ŀ�ͷ
		if(left >= 0){
			if( (right+1) < left ){//���[right+1, left)֮����ڷ�unicode�ַ���
				result.concat( sentence.subString(right+1, left) );//����ֱ�Ӽ��뵽����ַ�����
			}
			right = sentence.indexOf(";", right+1);//�ҵ�����"&#25105;"���Ӵ��Ľ�β

			CharString word = sentence.subString(left+2, right);//ȡ�����е����ֲ���25105
            uint value[2] = {stringToValue(word),'\0'};//������Ӧ����ֵ
            result.concat( qstr2charStr( QString::fromUcs4(value) ) );//����ת��ΪCharString����뵽resultĩβ
		}else{//����Ҳ��������Ĵ���
			if(right < sentence.size()-1){//˵����β�з�unicode�ַ���
				result.concat(sentence.subString(right+1, sentence.size()));
			}
			break;//������˳���
		}
	}    
    return result;
}
*/
