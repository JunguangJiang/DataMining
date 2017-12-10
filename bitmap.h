#ifndef BITMAP_H
#define BITMAP_H
#include "CharString.h"
class Bitmap
{
private:
    char* M; int N;//����ͼ�����M[]�У�����Ϊ8*N������λ
    void init(int n);//��ʼ��
public:
    Bitmap(int n=8){init(n);}//��ָ����ģ���ɱ���ͼ
    Bitmap(const CharString& file, int n=8);//��ָ����ģ���ӱ����ļ����ɱ���ͼ
    ~Bitmap();
    void set(int k);//��kλ��һ
    void clear(int k);//��kλ����
    bool get(int k);//�õ���kλ
    void expand(int k);//����
};

#endif // BITMAP_H
