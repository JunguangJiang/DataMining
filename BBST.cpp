#include "BBST.h"

BinNode::BinNode(const CharString& Term, int TermID, BinNodePosi parent, int height):
	TermWord(Term, TermID),
	parent(parent), lc(NULL), rc(NULL), height(height){}

BinNodePosi BinNode::attachAsLChild(const CharString& Term, int TermID){
	BinNodePosi p = new BinNode(Term, TermID, this);//以当前节点为父新建一个节点
	this->lc = p;//让新节点成为当前节点的左孩子
	return p;
}

BinNodePosi BinNode::attachAsRChild(const CharString& Term, int TermID){
	BinNodePosi p = new BinNode(Term, TermID, this);//以当前节点为父新建一个节点
	this->rc = p;//让新节点成为当前节点的右孩子
	return p;
}


BBST::BBST(void):
	m_root(NULL), m_hot(NULL), m_size(0)
{
}


BBST::~BBST(void)
{
	removeSubTree(m_root);//删除全树
}

BinNodePosi& BBST::Search(const CharString& termWord){//搜索节点
	m_hot = NULL;
	return searchIn(m_root, termWord);//返回在全树的搜索结果
}

BinNodePosi& BBST::searchIn(BinNodePosi& r, const CharString& termWord){
	if( r == NULL || r->Term == termWord ){//如果在当前节点(或者假想的空节点)命中
		return r;//则直接返回当前节点
	}else{//否则，
		m_hot = r;//记录热点，然后根据节点r的单词大小，向
		return r->Term > termWord ? searchIn(r->lc, termWord) : searchIn(r->rc, termWord);//左右子树继续寻找
	}
}


inline int max(int a, int b){//返回整数的最大值
	return (a<b)?b:a;
}

void BBST::updateHeight(BinNodePosi v){//更新节点v的树高
	v->height = max( height(v->lc), height(v->rc) ) + 1;//
}

BinNodePosi BBST::Insert(const CharString& termWord){//插入节点
	BinNodePosi& v = Search(termWord); 
	if(v) return v;//先查询原树中是否已有单词termWord,确保原单词不存在
	BinNodePosi x = v = new BinNode(termWord, -1, m_hot);//新建一个节点，单词为termWord,TermID=-1,父节点为m_hot
	m_size++;
	for(BinNodePosi g = m_hot; g; g=g->parent){//从当前节点的父节点m_hot出发,向上追寻
		if(!AVLBalanced(g)){//如果当前节点g AVL失衡，则
			if(IsRoot(g)){//如果g原先是根节点
				m_root = Adjust(g);//将调整后的子树接到树根
			}else{
				BinNodePosi gg = g->parent;
				if(IsLChild(g)){//如果g原先是左孩子
					gg->lc = Adjust(g);//将调整后的子树接到g原先父节点的左孩子
				}else{//如果g原先是右孩子
					gg->rc = Adjust(g);//将调整后的子树接到个原先父节点的右孩子
				}
			}
			//此后局部子树的高度必然复原
			break;
		}else{//如果当前节点g没有失衡
			updateHeight(g);//但是高度也有可能发生改变
		}
	}
	return x;//此时v->Term==termWord必然成立，并且AVL平衡条件也得到了满足
}

void BBST::connect34(BinNodePosi a, BinNodePosi b, BinNodePosi c, 
		BinNodePosi t1, BinNodePosi t2, BinNodePosi t3, BinNodePosi t4){
	//3+4重构，用于节点的旋转调整 assert:a,b,c非空
			a->lc = t1; if(t1) t1->parent = a;
			a->rc = t2; if(t2) t2->parent = a;
			updateHeight(a);
			c->lc = t3; if(t3) t3->parent = c;
			c->rc = t4; if(t4) t4->parent = c;
			updateHeight(c);
			b->lc = a; a->parent = b;
			b->rc = c; c->parent = b;
			updateHeight(b);
			//节点b和父节点的连接交由调用者自行完成
}

BinNodePosi BBST::Adjust(BinNodePosi g){//对失衡节点g进行旋转调整,返回调整后子树的树根
	BinNodePosi p = tallerChild(g); BinNodePosi v = tallerChild(p);//找到失衡节点较高的儿子和孙子
	BinNodePosi gg = g->parent;
	if(IsLChild(p)){//p是左孩子
		if(IsLChild(v)){//v是左孩子
			connect34(v, p, g, v->lc, v->rc, p->rc, g->rc);
			p->parent = gg;//节点p成为新子树的树根
			return p;
		}else{//（p是左孩子）v是右孩子
			connect34(p, v, g, p->lc, v->lc, v->rc, g->rc);
			v->parent = gg;//节点v成为新子树的树根
			return v;
		}
	}else{//p是右孩子
		if(IsLChild(v)){//v是左孩子
			connect34(g, v, p, g->lc, v->lc, v->rc, p->rc);
			v->parent = gg;//节点v成为新子树的树根
			return v;
		}else{//（p是右孩子）v是右孩子
			connect34(g, p, v, g->lc, p->lc, v->lc, v->rc);
			p->parent = gg;//节点p成为新子树的树根
			return p;
		}
	}
	//Note:上层节点到子树的连接由调用者完成
}

void BBST::debug(BinNodePosi p){
	if(!p) return; 
	std::cout <<"Word:"<< p->Term<<" height:"<<p->height << " lc:";
	if(p->lc) 
		std::cout << p->lc->Term;
	else
		std::cout << "NULL";
	std::cout << " rc:";
	if(p->rc)
		std::cout << p->rc->Term;
	else
		std::cout << "NULL";
	std:: cout << " docList: "; p->docList->debug();
	std::cout<<std::endl;
	debug(p->lc);
	debug(p->rc);
}

void BBST::removeSubTree(BinNodePosi v){
	if(!v) return;//确保节点v非空
	removeSubTree(v->lc);v->lc = NULL;//先删除左子树
	removeSubTree(v->rc);v->rc = NULL;//然后删除右子树
	v->parent = NULL; //切断父节点指针
	delete v;
}

BinNodePosi BBST::succ(BinNodePosi v){//寻找节点v的后继
	if(!IsValid(v)) return NULL;//如果节点v不合法，返回空
	BinNodePosi x;
	if(HasRChild(v)){//如果节点v有右子树
		for(x = v->rc; HasLChild(x) ; x = x->lc){}//找到右子树中最左下的节点x
		return x;
	}else{//如果节点v右子树为空
		for(x = v; IsRChild(x); x = x->parent){}//只要x是右孩子,就继续向上寻找
		//假如x为空或者x为根节点或者x为左孩子，则退出循环
		if(!IsLChild(x)) 
			return NULL;//如果节点x为空(即v为根），或者x为根，则返回空
		else //否则，x为左孩子
			return x->parent;//返回x的父节点即可 
	}
}

bool BBST::Remove(const CharString& termWord){//删除单词为termWord的节点，返回false代表搜索树中不存在该单词
	BinNodePosi& v = Search(termWord); if(!v) return false;//确认节点存在
	BinNodePosi w = v;//实际被删除的节点w，初值同v
	BinNodePosi s = NULL;//节点w的接替者
	if(!HasLChild(v)){//如果节点v没有左子树
		s = v->rc;//则v的接替者为右子树
	}else if(!HasRChild(v)){//如果节点v没有右子树
		s = v->lc;//则v的接替者是左子树
	}else{//如果节点v同时有左右子树
		w = succ(v);//则实际被删除节点w为节点v的后继节点（w必定非空且必定没有左子树）
		std::swap(w->Term, v->Term); std::swap(w->TermID, v->TermID);//将二者的数据区交换，此后
		s = w->rc;//转而删除节点w
		m_hot = w->parent;//更改被删除节点的父节点
	}
	if(IsRoot(w)){//如果节点w是根,则s成为新的根
		m_root = s; 
	}else{
		if(IsLChild(w)){//如果节点w是左子树
			w->parent->lc = s;
		}else{//如果节点w是右子树
			w->parent->rc = s;
		}
	}
	if(s) s->parent = w->parent;//至此已经将w隔离
	w->parent = w->lc = w->rc = NULL; delete w; w = NULL;//删除节点w

	//此后BBST可能发生失衡
	for(BinNodePosi g = m_hot; g; g = g->parent){//向上追溯w的祖先节点
		if(!AVLBalanced(g)){//如果祖先中g失衡
			if(IsRoot(g)){//如果g原先是根节点
				g =  m_root = Adjust(g);//将调整后的子树接到树根
			}else{
				BinNodePosi gg = g->parent;
				if(IsLChild(g)){//如果g原先是左孩子
					g = gg->lc = Adjust(g);//将调整后的子树接到g原先父节点的左孩子
				}else{//如果g原先是右孩子
					g = gg->rc = Adjust(g);//将调整后的子树接到g原先父节点的右孩子
				}
			}
		}
		updateHeight(g);//此后g的高度减少，上层节点依然可能失衡
	}
	return true;
}
