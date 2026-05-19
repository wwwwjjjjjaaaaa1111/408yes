#ifndef AVL_H
#define AVL_H
#include "algorithmbase.h"

class AVLTree : public AlgorithmBase {
    Q_OBJECT
public:
    explicit AVLTree(QObject *parent = nullptr);
    QString name() const override { return "平衡二叉树(AVL)"; }
    QString chapter() const override { return "第7章 查找"; }
    QString description() const override { return "AVL树的插入与旋转（LL、RR、LR、RL）演示"; }

    void run() override; void step() override; void reset() override;
    QVector<QString> parameterNames() const override { return {"rotDemo"}; }
    QVector<QString> parameterDescriptions() const override { return {"旋转演示"}; }
    QVector<QVariant> parameterDefaults() const override {
        return {QVariant(QVariantList{"LL旋转", "RR旋转", "LR旋转", "RL旋转", "完整构建"})};
    }
private:
    struct Node { int val,h=1;Node *l=nullptr,*r=nullptr; };
    int height(Node*n){return n?n->h:0;}
    int balance(Node*n){return n?height(n->l)-height(n->r):0;}
    Node* rotateRight(Node*y);
    Node* rotateLeft(Node*x);
    Node* insert(Node*n,int v);
    void inorder(Node*n,QString&s);
    void buildViz(Node*n,int&id,int pid,QVariantList&nodes);
    void deleteTree(Node*n);
    Node* m_root=nullptr;
    QString m_rot="LL旋转";
    int m_stepIndex=0;
};

#endif
