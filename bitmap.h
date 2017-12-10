#ifndef BITMAP_H
#define BITMAP_H
#include "CharString.h"
class Bitmap
{
private:
    char* M; int N;//比特图存放在M[]中，容量为8*N个比特位
    void init(int n);//初始化
public:
    Bitmap(int n=8){init(n);}//按指定规模生成比特图
    Bitmap(const CharString& file, int n=8);//按指定规模，从本地文件生成比特图
    ~Bitmap();
    void set(int k);//第k位置一
    void clear(int k);//第k位清零
    bool get(int k);//得到第k位
    void expand(int k);//扩容
};

#endif // BITMAP_H
