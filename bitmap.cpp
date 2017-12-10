#include "bitmap.h"
#include <memory.h>
#include <fstream>
#include <iostream>
void Bitmap::init(int n){
    N = (n-1) / 8 + 1;
    M = new char[N];
    memset(M, 0, N);
}

Bitmap::Bitmap(const CharString &file, int n){
    init(n);//首先初始化位图
    FILE* fp = fopen(file.data(), "r");//打开本地文件
    if(fp){
        fread(M, sizeof(char), N, fp);//读入数据
        fclose(fp);//关闭文件
    }else{
        std::cout << "Fail to open "<<file << std::endl; std::cout.flush();
    }
}

Bitmap::~Bitmap(){
    delete [] M; M = NULL;
}

void Bitmap::set(int k){
    expand(k);
    M[k>>3] |= 0x80 >> (k & 0x07);//k>>3=k/8, k&07 = k%8
}

void Bitmap::clear(int k){
    expand(k);
    M[k>>3] &= ~(0x80 >> (k & 0x07));
}

bool Bitmap::get(int k){
    expand(k);
    return M[k>>3] & ( 0x80 >> (k & 0x07) );
}

void Bitmap::expand(int k){
    if( (k>>3) > N){//假如访问越界，则需要
        int oldN = N; char* oldM = M;
        init(2 * k);//扩容
        memcpy_s(M, N, oldM, oldN);//复制原先的数据
        delete [] oldM;//删除原先的数据
    }
}
