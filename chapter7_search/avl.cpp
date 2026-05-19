#include "avl.h"
#include <algorithm>

AVLTree::AVLTree(QObject *parent) : AlgorithmBase(parent) {}

AVLTree::Node* AVLTree::rotateRight(Node *y) {
    Node *x=y->l, *T2=x->r;
    x->r=y; y->l=T2;
    y->h=std::max(height(y->l),height(y->r))+1;
    x->h=std::max(height(x->l),height(x->r))+1;
    addStep(QString("LL旋转(右旋): %1 绕 %2 右旋").arg(y->val).arg(x->val));
    return x;
}

AVLTree::Node* AVLTree::rotateLeft(Node *x) {
    Node *y=x->r, *T2=y->l;
    y->l=x; x->r=T2;
    x->h=std::max(height(x->l),height(x->r))+1;
    y->h=std::max(height(y->l),height(y->r))+1;
    addStep(QString("RR旋转(左旋): %1 绕 %2 左旋").arg(x->val).arg(y->val));
    return y;
}

AVLTree::Node* AVLTree::insert(Node *n, int v) {
    if(!n){addStep(QString("插入节点: %1").arg(v));return new Node{v};}
    if(v<n->val)n->l=insert(n->l,v);
    else if(v>n->val)n->r=insert(n->r,v);
    else return n;
    n->h=1+std::max(height(n->l),height(n->r));
    int bal=balance(n);
    addStep(QString("节点%1: 高度=%2, 平衡因子=%3").arg(n->val).arg(n->h).arg(bal));

    // LL
    if(bal>1&&v<n->l->val){addStep(QString("LL不平衡: 在%1左子%2的左侧, 右旋").arg(n->val).arg(n->l->val));return rotateRight(n);}
    // RR
    if(bal<-1&&v>n->r->val){addStep(QString("RR不平衡: 在%1右子%2的右侧, 左旋").arg(n->val).arg(n->r->val));return rotateLeft(n);}
    // LR
    if(bal>1&&v>n->l->val){addStep(QString("LR不平衡: 先左旋%1, 再右旋%2").arg(n->l->val).arg(n->val));n->l=rotateLeft(n->l);return rotateRight(n);}
    // RL
    if(bal<-1&&v<n->r->val){addStep(QString("RL不平衡: 先右旋%1, 再左旋%2").arg(n->r->val).arg(n->val));n->r=rotateRight(n->r);return rotateLeft(n);}
    return n;
}

void AVLTree::inorder(Node *n, QString &s) { if(!n)return;inorder(n->l,s);s+=QString::number(n->val)+"("+QString::number(balance(n))+") ";inorder(n->r,s); }
void AVLTree::deleteTree(Node *n) { if(!n)return;deleteTree(n->l);deleteTree(n->r);delete n; }

void AVLTree::buildViz(Node *n, int &id, int pid, QVariantList &nodes) {
    if(!n)return;
    QVariantMap nd;nd["id"]=id;nd["label"]=QString("%1(b:%2)").arg(n->val).arg(balance(n));nd["parentId"]=pid;
    if(balance(n)>1||balance(n)<-1)nd["color"]="#FF6B6B";
    nodes.append(nd);
    int cid=id;
    if(n->l){id++;buildViz(n->l,id,cid,nodes);}
    if(n->r){id++;buildViz(n->r,id,cid,nodes);}
}

void AVLTree::run() {
    reset();
    m_rot = m_params.value("rotDemo", "LL旋转").toString();

    if(m_rot=="LL旋转"){
        addStep("LL旋转演示: 依次插入 30,20,10");
        m_root=insert(m_root,30);m_root=insert(m_root,20);m_root=insert(m_root,10);
    } else if(m_rot=="RR旋转"){
        addStep("RR旋转演示: 依次插入 10,20,30");
        m_root=insert(m_root,10);m_root=insert(m_root,20);m_root=insert(m_root,30);
    } else if(m_rot=="LR旋转"){
        addStep("LR旋转演示: 依次插入 30,10,20");
        m_root=insert(m_root,30);m_root=insert(m_root,10);m_root=insert(m_root,20);
    } else if(m_rot=="RL旋转"){
        addStep("RL旋转演示: 依次插入 10,30,20");
        m_root=insert(m_root,10);m_root=insert(m_root,30);m_root=insert(m_root,20);
    } else {
        addStep("完整AVL树构建: 50,30,70,20,40,60,80,10,25,35,45");
        for(int v:{50,30,70,20,40,60,80,10,25,35,45})m_root=insert(m_root,v);
    }

    QString s;inorder(m_root,s);
    addStep(QString("中序(平衡因子): %1").arg(s.trimmed()));
    m_resultText=QString("AVL %1 完成，中序: %2").arg(m_rot).arg(s.trimmed());

    QVariantMap viz;viz["type"]="tree";viz["title"]=QString("AVL树 - %1").arg(m_rot);
    QVariantList nodes;int id=0;buildViz(m_root,id,-1,nodes);viz["nodes"]=nodes;m_vizData=viz;

    deleteTree(m_root);m_root=nullptr;
    m_running=false;
}

void AVLTree::step() { if(m_stepIndex==0)run(); }
void AVLTree::reset() { deleteTree(m_root);m_root=nullptr;m_steps.clear();m_stepIndex=0;m_resultText.clear();m_vizData.clear(); }
