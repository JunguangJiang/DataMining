#ifndef HASH_H
#define HASH_H
#include "termword.h"
#include "bitmap.h"

#define Entry TermWord//��������TermWord
#define EntryPosi Entry*//����λ��
#define Key CharString //�ؼ�����һ���ַ���
class Hash
{
private:
    EntryPosi* ht;//Ͱ
    int M;//Ͱ��������
    int N;//��������
    Bitmap* lazyRemoval;//����ɾ�����

    int probeForFree(const Key& key);//Ѱ���׸���Ͱ
    int probeForHit(const Key& key);//Ѱ���׸����е�Ͱ
    void rehash();//����
    bool inline lazyRemoved(int i){return lazyRemoval->get(i);}//��i��Ͱ�Ƿ�������ɾ�����
    void inline markAsLazyRemoval(int i){lazyRemoval->set(i);}//��i��������Ϊ����ɾ��
    bool Insert(EntryPosi entry);//ֱ�Ӳ���һ��������λ�ã�������hash

public:
    Hash(int c=97);//����һ����ʼ����Ϊc��hash��
    ~Hash();
    EntryPosi Search(const Key& key);//��ѯhash�����ش���
    EntryPosi Insert(const Key& key);
    //���ز���Ĵ�����λ�ã����ԭ���Ѿ����ڸô����������ʧ�ܡ����۲����Ƿ�ɹ������ش�����Ӧ�ĵ��ʶ���key
    bool Remove(const Key& key);//ɾ�������������Ƿ�ɹ�
    void debug();
};

#endif // HASH_H
