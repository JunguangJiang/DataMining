#include "hash.h"
#include "parameter.h"
#include <iostream>
#include <QDebug>
#include "convert_qstr_str.h"
int primeNLT(int c, int n, const CharString& file){//����file�ļ��еļ�¼����[c,n)��ȡ��С������
    Bitmap B(file, n);//file�Ѿ���λͼ��ʽ����¼��n���ڵ��������������ֻҪ
    while(c < n){//��c��ʼ����λ��
        if(B.get(c)) c++;//���ԣ�����
        else return c;//�����׸����ֵ�����
    }
    return c;//��û������������������n
}

Hash::Hash(int c)
{
    M = primeNLT(c, 1048576, CharString(PrimNumberList));//�õ�����c����С����
    N = 0;//��ʼ��������Ϊ0
    ht = new EntryPosi[M]; memset(ht, 0, sizeof(EntryPosi) * M);//��ʼ��Ͱ����
    lazyRemoval = new Bitmap(M);//��ʼ������ɾ��λͼ
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

int Hash::probeForHit(const Key &key){//�����׸�����Ͱ
    int p = hashCode(key) % M;//�ҵ�key��Ӧ��ɢ����
    while( ( ht[p] && ht[p]->Term != key ) || //�����ǰͰ�ǿղ��Ҵ洢�ĵ��ʲ���key
           ( !ht[p] && lazyRemoved(p) ) ){//���ߵ�ǰͰΪ�գ�����������ɾ�����
        p = (p+1) % M;//�����Ų���������Ѱ��
    }
    return p;//�����߸���ht[p]�Ƿ�Ϊ���жϲ����Ƿ�ɹ�
}

int Hash::probeForFree(const Key &key){//�����׸���Ͱ
    int p = hashCode(key) % M;//�ҵ�key��Ӧ��ɢ����
    while( ht[p] ){//�����ǰͰΪ��
        p = (p+1) % M;//�����Ų���������Ѱ��
    }
    return p;
}

EntryPosi Hash::Search(const Key &key){
    int p = probeForHit(key);
    return ht[p];//�����߸��ݲ��ҵ��Ĵ����Ƿ�Ϊ�գ��жϲ����Ƿ�ɹ�
}

EntryPosi Hash::Insert(const Key &key){
    rehash();//��������������࣬����Ҫ������hash
    EntryPosi entry = Search(key);//�Ƚ��в���
    if(entry) return entry;//���ԭ�ȴ��ڴ���key����ֱ�ӷ���
    int p = probeForFree(key);//�ҵ����Բ���������׸���Ͱ
    ht[p] = new Entry(key); //�������
    N++;
    return ht[p];
}

bool Hash::Insert(EntryPosi entry){
    rehash();//��������������࣬����Ҫ��hash
    if(!entry) return false;
    EntryPosi searchEntry = Search(entry->Term);//�Ƚ��в���
    if(searchEntry) return false;//���ԭ�ȴ��ڴ���key�������ʧ��
    int p = probeForFree(entry->Term);//�ҵ����Բ���������׸���Ͱ
    ht[p] = entry;//�������
    N++;
    return true;//����ɹ�
}

bool Hash::Remove(const Key &key){
    int p = probeForHit(key);//���Ƚ��в���
    if(!ht[p]) return false;//���ԭ�Ȳ����ڵ���key����ɾ��ʧ��
    delete ht[p]; ht[p] = NULL;//����ɾ����Ӧ�Ĵ���
    markAsLazyRemoval(p);//������������ɾ�����
    return true;//ɾ���ɹ�
}

void Hash::rehash(){
    if(M > 2 * N)return;//�����Ͱ��������50%������Ҫ��hash
    EntryPosi* oldHt=ht; int oldM = M;//�ȼ�¼ԭ�ȵ�Ͱ������
    M = primeNLT(2*M, 1048576, CharString(PrimNumberList));//Ͱ�����������ټӱ�
    delete lazyRemoval; lazyRemoval = new Bitmap(M);//ɾ��ԭ�ȵ�����ɾ����ǣ���������һ�ſհ׵�����ɾ�����
    ht = new EntryPosi[M]; memset(ht, 0, sizeof(EntryPosi) * M); N=0;//�����µ�Ͱ����
    for(int i=0; i<oldM; i++){//��ԭ�ȵĴ�����
        if(oldHt[i]){
            Insert(oldHt[i]);//�������µ�Ͱ������
            oldHt[i] = NULL;
        }
    }
    delete[] oldHt; oldHt = NULL;//ɾ��ԭ�ȵ�Ͱ���飨��������д�����û��������
}

void Hash::debug(){
    for(int i=0; i<M; i++){
        if(ht[i]){
            std::cout << i  << " "<< ht[i]->Term << std::endl;
        }
    }
}




