#ifndef RBTREE_H
#define RBTREE_H
#include "algorithmbase.h"

class RBTree : public AlgorithmBase {
    Q_OBJECT
public:
    explicit RBTree(QObject *parent = nullptr);
    QString name() const override { return "红黑树"; }
    QString chapter() const override { return "第7章 查找"; }
    QString description() const override { return "红黑树的插入演示（含颜色调整与旋转）"; }

    void run() override; void step() override; void reset() override;
    QVector<QString> parameterNames() const override { return {}; }
    QVector<QString> parameterDescriptions() const override { return {}; }
    QVector<QVariant> parameterDefaults() const override { return {}; }

private:
    enum Color {RED,BLACK};
    struct Node { int val; Color c=RED; Node *l=nullptr,*r=nullptr,*p=nullptr; };
    void rotateLeft(Node *x);
    void rotateRight(Node *y);
    void fixInsert(Node *z);
    void insert(int v);
    void inorder(Node *n, QString &s);
    void buildViz(Node *n, int &id, int pid, QVariantList &nodes);
    void deleteTree(Node *n);
    Node *m_root=nullptr;
    int m_stepIndex=0;
};

#endif
