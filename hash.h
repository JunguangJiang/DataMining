#ifndef HASH_H
#define HASH_H
#include "termword.h"
#include "bitmap.h"

#define Entry TermWord//词条就是TermWord
#define EntryPosi Entry*//词条位置
#define Key CharString //关键词是一个字符串
class Hash
{
private:
    EntryPosi* ht;//桶
    int M;//桶数组容量
    int N;//词条数量
    Bitmap* lazyRemoval;//懒惰删除标记

    int probeForFree(const Key& key);//寻找首个空桶
    int probeForHit(const Key& key);//寻找首个命中的桶
    void rehash();//扩容
    bool inline lazyRemoved(int i){return lazyRemoval->get(i);}//第i个桶是否有懒惰删除标记
    void inline markAsLazyRemoval(int i){lazyRemoval->set(i);}//第i个数组标记为懒惰删除
    bool Insert(EntryPosi entry);//直接插入一个词条的位置，用于重hash

public:
    Hash(int c=97);//构建一个初始容量为c的hash表
    ~Hash();
    EntryPosi Search(const Key& key);//查询hash，返回词条
    EntryPosi Insert(const Key& key);
    //返回插入的词条的位置，如果原先已经存在该词条，则插入失败。不论插入是否成功，返回词条对应的单词都是key
    bool Remove(const Key& key);//删除词条，返回是否成功
    void debug();
};

#endif // HASH_H
