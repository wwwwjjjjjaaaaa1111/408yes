#ifndef BST_H
#define BST_H
#include "algorithmbase.h"

class BST : public AlgorithmBase {
    Q_OBJECT
public:
    explicit BST(QObject *parent = nullptr);
    QString name() const override { return "二叉排序树(BST)"; }
    QString chapter() const override { return "第7章 查找"; }
    QString description() const override { return "BST的构建、查找、插入、删除操作"; }

    void run() override; void step() override; void reset() override;
    QVector<QString> parameterNames() const override { return {"operation"}; }
    QVector<QString> parameterDescriptions() const override { return {"操作类型"}; }
    QVector<QVariant> parameterDefaults() const override {
        return {QVariant(QVariantList{"构建与查找", "插入", "删除"})};
    }
private:
    struct Node { int val; Node *l=nullptr,*r=nullptr; };
    Node *insert(Node *n, int v);
    Node *remove(Node *n, int v, bool &found);
    Node *findMin(Node *n);
    bool search(Node *n, int v, int &comps);
    void inorder(Node *n, QString &s);
    void buildViz(Node *n, int &id, int parentId, QVariantList &nodes);
    void deleteTree(Node *n);
    Node *m_root = nullptr;
    QString m_op = "构建与查找";
    int m_stepIndex = 0;
};

#endif
