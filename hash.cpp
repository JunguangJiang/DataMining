#include "hash.h"
#include "parameter.h"
#include <iostream>
#include <QDebug>
#include "convert_qstr_str.h"
int primeNLT(int c, int n, const CharString& file){//根据file文件中的记录，在[c,n)内取最小的素数
    Bitmap B(file, n);//file已经按位图格式，记录了n以内的所有素数，因此只要
    while(c < n){//从c开始，逐位地
        if(B.get(c)) c++;//测试，即可
        else return c;//返回首个发现的素数
    }
    return c;//若没有这样的素数，返回n
}

Hash::Hash(int c)
{
    M = primeNLT(c, 1048576, CharString(PrimNumberList));//得到大于c的最小素数
    N = 0;//初始词条数量为0
    ht = new EntryPosi[M]; memset(ht, 0, sizeof(EntryPosi) * M);//初始化桶数组
    lazyRemoval = new Bitmap(M);//初始化懒惰删除位图
}

Hash::~Hash(){
    delete lazyRemoval; lazyRemoval = NULL;
    for(int i=0; i<M; i++){
        if(ht[i]){
            delete ht[i];
            ht[i] = NULL;
        }
    }
    delete [] ht; ht = NULL;
}

int Hash::probeForHit(const Key &key){//查找首个命中桶
    int p = hashCode(key) % M;//找到key对应的散列码
    while( ( ht[p] && ht[p]->Term != key ) || //如果当前桶非空并且存储的单词不是key
           ( !ht[p] && lazyRemoved(p) ) ){//或者当前桶为空，但是有懒惰删除标记
        p = (p+1) % M;//都沿着查找链继续寻找
    }
    return p;//调用者根据ht[p]是否为空判断查找是否成功
}

int Hash::probeForFree(const Key &key){//查找首个空桶
    int p = hashCode(key) % M;//找到key对应的散列码
    while( ht[p] ){//如果当前桶为空
        p = (p+1) % M;//就沿着查找链继续寻找
    }
    return p;
}

EntryPosi Hash::Search(const Key &key){
    int p = probeForHit(key);
    return ht[p];//调用者根据查找到的词条是否为空，判断查找是否成功
}

EntryPosi Hash::Insert(const Key &key){
    rehash();//如果词条数量过多，则需要进行重hash
    EntryPosi entry = Search(key);//先进行查找
    if(entry) return entry;//如果原先存在词条key，则直接返回
    int p = probeForFree(key);//找到可以插入词条的首个空桶
    ht[p] = new Entry(key); //插入词条
    N++;
    return ht[p];
}

bool Hash::Insert(EntryPosi entry){
    rehash();//如果词条数量过多，则需要重hash
    if(!entry) return false;
    EntryPosi searchEntry = Search(entry->Term);//先进行查找
    if(searchEntry) return false;//如果原先存在词条key，则插入失败
    int p = probeForFree(entry->Term);//找到可以插入词条的首个空桶
    ht[p] = entry;//插入词条
    N++;
    return true;//插入成功
}

bool Hash::Remove(const Key &key){
    int p = probeForHit(key);//首先进行查找
    if(!ht[p]) return false;//如果原先不存在单词key，则删除失败
    delete ht[p]; ht[p] = NULL;//否则删除对应的词条
    markAsLazyRemoval(p);//并且设置懒惰删除标记
    return true;//删除成功
}

void Hash::rehash(){
    if(M > 2 * N)return;//如果空桶数量大于50%，则不需要重hash
    EntryPosi* oldHt=ht; int oldM = M;//先记录原先的桶和容量
    M = primeNLT(2*M, 1048576, CharString(PrimNumberList));//桶数组容量至少加倍
    delete lazyRemoval; lazyRemoval = new Bitmap(M);//删除原先的懒惰删除标记，重新生成一张空白的懒惰删除标记
    ht = new EntryPosi[M]; memset(ht, 0, sizeof(EntryPosi) * M); N=0;//生成新的桶数组
    for(int i=0; i<oldM; i++){//将原先的词条都
        if(oldHt[i]){
            Insert(oldHt[i]);//拷贝到新的桶数组中
            oldHt[i] = NULL;
        }
    }
    delete[] oldHt; oldHt = NULL;//删除原先的桶数组（这个过程中词条并没有析构）
}

void Hash::debug(){
    for(int i=0; i<M; i++){
        if(ht[i]){
            std::cout << i  << " "<< ht[i]->Term << std::endl;
        }
    }
}




