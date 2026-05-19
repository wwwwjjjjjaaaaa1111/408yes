#ifndef BTREE_H
#define BTREE_H
#include "algorithmbase.h"

class BTree : public AlgorithmBase {
    Q_OBJECT
public:
    explicit BTree(QObject *parent = nullptr);
    QString name() const override { return "B树"; }
    QString chapter() const override { return "第7章 查找"; }
    QString description() const override { return "B树的插入与删除，阶数可调（3~7阶）"; }

    void run() override; void step() override; void reset() override;
    QVector<QString> parameterNames() const override;
    QVector<QString> parameterDescriptions() const override;
    QVector<QVariant> parameterDefaults() const override;

private:
    void insert(int k);
    void splitChild(int idx);
    void traverse();
    struct BNode {
        QVector<int> keys;
        QVector<BNode*> children;
        bool leaf=true;
    };
    BNode *m_root=nullptr;
    int m_order=3;
    int m_stepIndex=0;
};

#endif
