#include "threadedtree.h"

ThreadedTree::ThreadedTree(QObject *parent) : AlgorithmBase(parent) {}

QVector<QString> ThreadedTree::parameterNames() const { return {}; }
QVector<QString> ThreadedTree::parameterDescriptions() const { return {}; }
QVector<QVariant> ThreadedTree::parameterDefaults() const { return {}; }

ThreadedTree::ThreadNode* ThreadedTree::buildTree() {
    auto *r = new ThreadNode{1};
    r->left = new ThreadNode{2}; r->right = new ThreadNode{3};
    r->left->left = new ThreadNode{4}; r->left->right = new ThreadNode{5};
    r->right->right = new ThreadNode{6};
    return r;
}

void ThreadedTree::deleteTree(ThreadNode *n) {
    if (!n) return;
    if (!n->ltag) deleteTree(n->left);
    if (!n->rtag) deleteTree(n->right);
    delete n;
}

void ThreadedTree::inThread(ThreadNode *p, ThreadNode *&pre) {
    if (!p) return;
    if (!p->ltag) inThread(p->left, pre);
    addStep(QString("处理节点 %1").arg(p->val));
    if (!p->left) { p->ltag = true; p->left = pre; addStep(QString("  节点%1 左线索→ %2").arg(p->val).arg(pre?pre->val:-1)); }
    if (pre && !pre->right) { pre->rtag = true; pre->right = p; addStep(QString("  节点%1 右线索→ %2").arg(pre->val).arg(p->val)); }
    pre = p;
    if (!p->rtag) inThread(p->right, pre);
}

QString ThreadedTree::traverseThreaded(ThreadNode *root) {
    QString result;
    ThreadNode *p = root;
    while (p) {
        while (!p->ltag) p = p->left;
        result += QString::number(p->val) + " ";
        addStep(QString("线索遍历-访问: %1").arg(p->val));
        while (p->rtag) { p = p->right; result += QString::number(p->val) + " "; addStep(QString("线索遍历-访问(后继): %1").arg(p->val)); }
        p = p->right;
    }
    return result;
}

void ThreadedTree::run() {
    reset();
    m_root = buildTree();
    addStep("构建二叉树，进行中序线索化...");
    ThreadNode *pre = nullptr;
    inThread(m_root, pre);
    addStep("线索化完成，开始线索遍历:");
    QString traversal = traverseThreaded(m_root);
    m_resultText = "中序线索遍历: " + traversal.trimmed();

    QVariantMap viz; viz["type"]="tree"; viz["title"]="线索二叉树(中序)";
    QVariantList nodes;
    auto addN = [&](int id,QString l,int p){ QVariantMap n; n["id"]=id;n["label"]=l;n["parentId"]=p;nodes.append(n); };
    addN(1,"1",-1);addN(2,"2",1);addN(3,"3",1);addN(4,"4",2);addN(5,"5",2);addN(6,"6",3);
    viz["nodes"]=nodes; m_vizData=viz;
    deleteTree(m_root); m_root=nullptr;
    m_running=false;
}

void ThreadedTree::step() { if(m_stepIndex==0)run(); }
void ThreadedTree::reset() { m_steps.clear(); m_stepIndex=0; m_resultText.clear(); m_vizData.clear(); }
