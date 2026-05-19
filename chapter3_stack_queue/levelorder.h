#ifndef LEVELORDER_H
#define LEVELORDER_H

#include "algorithmbase.h"
#include <QQueue>

class LevelOrder : public AlgorithmBase {
    Q_OBJECT
public:
    explicit LevelOrder(QObject *parent = nullptr);
    QString name() const override { return "队列在层次遍历中的应用"; }
    QString chapter() const override { return "第3章 栈、队列和数组"; }
    QString description() const override { return "使用队列对二叉树进行层次遍历(BFS)"; }

    void run() override;
    void step() override;
    void reset() override;

    QVector<QString> parameterNames() const override;
    QVector<QString> parameterDescriptions() const override;
    QVector<QVariant> parameterDefaults() const override;

private:
    struct TreeNode {
        int val;
        TreeNode *left = nullptr;
        TreeNode *right = nullptr;
    };
    TreeNode *buildSampleTree();
    void deleteTree(TreeNode *node);
    QString levelOrderTraversal(TreeNode *root);
    TreeNode *m_root = nullptr;
    int m_stepIndex = 0;
};

#endif // LEVELORDER_H
