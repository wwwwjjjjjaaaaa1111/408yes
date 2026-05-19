#ifndef BINARYTREETRAVERSAL_H
#define BINARYTREETRAVERSAL_H

#include "algorithmbase.h"
#include <QStack>

class BinaryTreeTraversal : public AlgorithmBase {
    Q_OBJECT
public:
    explicit BinaryTreeTraversal(QObject *parent = nullptr);
    QString name() const override { return "二叉树的四种遍历"; }
    QString chapter() const override { return "第5章 树与二叉树"; }
    QString description() const override { return "前序、中序、后序、层序遍历，含递归和非递归实现"; }

    void run() override;
    void step() override;
    void reset() override;

    QVector<QString> parameterNames() const override;
    QVector<QString> parameterDescriptions() const override;
    QVector<QVariant> parameterDefaults() const override;

private:
    struct Node { int val; Node *left=nullptr; Node *right=nullptr; };
    Node *buildTree();
    void deleteTree(Node *n);
    void preOrder(Node *root);
    void inOrder(Node *root);
    void postOrder(Node *root);
    void levelOrder(Node *root);
    void preOrderNR(Node *root);
    void inOrderNR(Node *root);
    void postOrderNR(Node *root);
    Node *m_root = nullptr;
    QString m_mode = "递归";
    int m_stepIndex = 0;
    QString m_result;
};

#endif // BINARYTREETRAVERSAL_H
