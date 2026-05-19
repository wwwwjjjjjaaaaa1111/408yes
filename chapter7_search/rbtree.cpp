#include "rbtree.h"

RBTree::RBTree(QObject *parent) : AlgorithmBase(parent) {}

void RBTree::rotateLeft(Node *x) {
    Node *y=x->r; x->r=y->l;
    if(y->l)y->l->p=x; y->p=x->p;
    if(!x->p)m_root=y;
    else if(x==x->p->l)x->p->l=y;
    else x->p->r=y;
    y->l=x;x->p=y;
    addStep(QString("左旋: %1 ← %2").arg(x->val).arg(y->val));
}

void RBTree::rotateRight(Node *y) {
    Node *x=y->l;y->l=x->r;
    if(x->r)x->r->p=y;x->p=y->p;
    if(!y->p)m_root=x;
    else if(y==y->p->l)y->p->l=x;
    else y->p->r=x;
    x->r=y;y->p=x;
    addStep(QString("右旋: %1 → %2").arg(x->val).arg(y->val));
}

void RBTree::fixInsert(Node *z) {
    while(z->p&&z->p->c==RED){
        if(z->p==z->p->p->l){
            Node *y=z->p->p->r;
            if(y&&y->c==RED){
                addStep(QString("情况1(叔节点红色): %1、p=%2、u=%3 变色").arg(z->val).arg(z->p->val).arg(y->val));
                z->p->c=BLACK;y->c=BLACK;z->p->p->c=RED;z=z->p->p;
            } else {
                if(z==z->p->r){addStep(QString("情况2(LR): 对%1左旋").arg(z->p->val));z=z->p;rotateLeft(z);}
                addStep(QString("情况3(LL): 对%1右旋, 变色").arg(z->p->p->val));
                z->p->c=BLACK;z->p->p->c=RED;rotateRight(z->p->p);
            }
        } else {
            Node *y=z->p->p->l;
            if(y&&y->c==RED){
                addStep(QString("情况1(叔节点红色): 变色"));
                z->p->c=BLACK;y->c=BLACK;z->p->p->c=RED;z=z->p->p;
            } else {
                if(z==z->p->l){addStep(QString("情况2(RL): 对%1右旋").arg(z->p->val));z=z->p;rotateRight(z);}
                addStep(QString("情况3(RR): 对%1左旋, 变色").arg(z->p->p->val));
                z->p->c=BLACK;z->p->p->c=RED;rotateLeft(z->p->p);
            }
        }
    }
    m_root->c=BLACK;
}

void RBTree::insert(int v) {
    Node *z=new Node{v,RED},*y=nullptr,*x=m_root;
    while(x){y=x;x=v<x->val?x->l:x->r;}
    z->p=y;
    if(!y){m_root=z;addStep(QString("插入根节点: %1(黑色)").arg(v));}
    else if(v<y->val){y->l=z;addStep(QString("插入红色节点 %1 作为 %2 的左孩子").arg(v).arg(y->val));}
    else{y->r=z;addStep(QString("插入红色节点 %1 作为 %2 的右孩子").arg(v).arg(y->val));}
    if(z->p&&z->p->c==RED)fixInsert(z);
    m_root->c=BLACK;
}

void RBTree::inorder(Node *n, QString &s) { if(!n)return;inorder(n->l,s);s+=QString::number(n->val)+(n->c==RED?"R":"B")+" ";inorder(n->r,s); }
void RBTree::deleteTree(Node *n) { if(!n)return;deleteTree(n->l);deleteTree(n->r);delete n; }

void RBTree::buildViz(Node *n, int &id, int pid, QVariantList &nodes) {
    if(!n)return;
    QVariantMap nd;nd["id"]=id;nd["label"]=QString::number(n->val);nd["parentId"]=pid;
    nd["isRed"]=(n->c==RED);nodes.append(nd);
    int cid=id;
    if(n->l){id++;buildViz(n->l,id,cid,nodes);}
    if(n->r){id++;buildViz(n->r,id,cid,nodes);}
}

void RBTree::run() {
    reset();
    addStep("===== 红黑树插入演示 =====");
    addStep("依次插入: 10,20,30,15,25,5,35");
    for(int v:{10,20,30,15,25,5,35})insert(v);

    QString s;inorder(m_root,s);
    addStep(QString("中序(R=红色,B=黑色): %1").arg(s.trimmed()));
    m_resultText=QString("红黑树构建完成，中序: %1").arg(s.trimmed());

    QVariantMap viz;viz["type"]="tree";viz["title"]="红黑树";
    QVariantList nodes;int id=0;buildViz(m_root,id,-1,nodes);viz["nodes"]=nodes;m_vizData=viz;

    deleteTree(m_root);m_root=nullptr;
    m_running=false;
}

void RBTree::step() { if(m_stepIndex==0)run(); }
void RBTree::reset() { deleteTree(m_root);m_root=nullptr;m_steps.clear();m_stepIndex=0;m_resultText.clear();m_vizData.clear(); }
