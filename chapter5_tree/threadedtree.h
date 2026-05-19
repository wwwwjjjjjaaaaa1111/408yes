#ifndef THREADEDTREE_H
#define THREADEDTREE_H

#include "algorithmbase.h"

class ThreadedTree : public AlgorithmBase {
    Q_OBJECT
public:
    explicit ThreadedTree(QObject *parent = nullptr);
    QString name() const override { return "线索二叉树"; }
    QString chapter() const override { return "第5章 树与二叉树"; }
    QString description() const override { return "中序线索化二叉树及线索遍历"; }

    void run() override;
    void step() override;
    void reset() override;

    QVector<QString> parameterNames() const override;
    QVector<QString> parameterDescriptions() const override;
    QVector<QVariant> parameterDefaults() const override;

private:
    struct ThreadNode {
        int val;
        ThreadNode *left=nullptr, *right=nullptr;
        bool ltag=false, rtag=false; // true=thread, false=child
    };
    ThreadNode *buildTree();
    void deleteTree(ThreadNode *n);
    void inThread(ThreadNode *p, ThreadNode *&pre);
    QString traverseThreaded(ThreadNode *root);
    ThreadNode *m_root = nullptr;
    ThreadNode *m_pre = nullptr;
    int m_stepIndex = 0;
};

#endif
