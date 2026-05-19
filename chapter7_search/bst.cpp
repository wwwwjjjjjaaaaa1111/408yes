#include "bst.h"

BST::BST(QObject *parent) : AlgorithmBase(parent) {}

BST::Node* BST::insert(Node *n, int v) {
    if (!n) { addStep(QString("插入节点: %1").arg(v)); return new Node{v}; }
    if (v < n->val) n->l = insert(n->l, v);
    else if (v > n->val) n->r = insert(n->r, v);
    else addStep(QString("%1 已存在，不插入").arg(v));
    return n;
}

BST::Node* BST::findMin(Node *n) { while(n&&n->l)n=n->l; return n; }

BST::Node* BST::remove(Node *n, int v, bool &found) {
    if (!n) return nullptr;
    if (v < n->val) n->l = remove(n->l, v, found);
    else if (v > n->val) n->r = remove(n->r, v, found);
    else {
        found = true;
        addStep(QString("找到待删除节点: %1").arg(v));
        if (!n->l) { Node *t=n->r; delete n; addStep("删除叶子节点(无左孩子)"); return t; }
        else if (!n->r) { Node *t=n->l; delete n; addStep("删除节点(无右孩子)"); return t; }
        Node *t=findMin(n->r);
        n->val=t->val;
        addStep(QString("用后继%1替换").arg(t->val));
        n->r=remove(n->r,t->val,found);
    }
    return n;
}

bool BST::search(Node *n, int v, int &comps) {
    if (!n) return false;
    comps++;
    if (n->val==v) { addStep(QString("查找%1: 比较%2 == %3 ✓").arg(v).arg(n->val).arg(v)); return true; }
    if (v<n->val) { addStep(QString("查找%1: %2 > %1, 向左").arg(v).arg(n->val)); return search(n->l,v,comps); }
    addStep(QString("查找%1: %2 < %1, 向右").arg(v).arg(n->val)); return search(n->r,v,comps);
}

void BST::inorder(Node *n, QString &s) { if(!n)return;inorder(n->l,s);s+=QString::number(n->val)+" ";inorder(n->r,s); }

void BST::deleteTree(Node *n) { if(!n)return;deleteTree(n->l);deleteTree(n->r);delete n; }

void BST::buildViz(Node *n, int &id, int parentId, QVariantList &nodes) {
    if(!n)return;
    QVariantMap nd; nd["id"]=id; nd["label"]=QString::number(n->val); nd["parentId"]=parentId; nodes.append(nd);
    int curId=id;
    if(n->l){id++;buildViz(n->l,id,curId,nodes);}
    if(n->r){id++;buildViz(n->r,id,curId,nodes);}
}

void BST::run() {
    reset();
    m_op = m_params.value("operation", "构建与查找").toString();
    QVector<int> vals = {50,30,70,20,40,60,80,35,45,55,65};

    addStep(QString("BST操作: %1").arg(m_op));
    for(int v:vals) m_root=insert(m_root,v);

    QString s; inorder(m_root,s);
    addStep(QString("BST中序遍历: %1").arg(s.trimmed()));

    if(m_op=="构建与查找"){
        int comps=0;
        bool found=search(m_root,40,comps);
        m_resultText=QString("中序: %1\n查找40: %2, 比较%3次").arg(s.trimmed()).arg(found?"找到":"未找到").arg(comps);
    } else if(m_op=="插入"){
        m_root=insert(m_root,42);
        s.clear();inorder(m_root,s);
        m_resultText=QString("插入42后中序: %1").arg(s.trimmed());
    } else if(m_op=="删除"){
        bool found=false;
        m_root=remove(m_root,30,found);
        s.clear();inorder(m_root,s);
        m_resultText=QString("删除30后中序: %1").arg(s.trimmed());
    }

    QVariantMap viz; viz["type"]="tree"; viz["title"]="二叉排序树(BST)";
    QVariantList nodes; int id=0; buildViz(m_root,id,-1,nodes);
    viz["nodes"]=nodes; m_vizData=viz;

    deleteTree(m_root); m_root=nullptr;
    m_running=false;
}

void BST::step() { if(m_stepIndex==0)run(); }
void BST::reset() { deleteTree(m_root); m_root=nullptr; m_steps.clear();m_stepIndex=0;m_resultText.clear();m_vizData.clear(); }
