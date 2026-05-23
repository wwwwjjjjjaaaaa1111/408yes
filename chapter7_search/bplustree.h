#ifndef BPLUSTREE_H
#define BPLUSTREE_H
#include "algorithmbase.h"
#include <functional>

class BPlusTree : public AlgorithmBase {
    Q_OBJECT
public:
    explicit BPlusTree(QObject *parent = nullptr);
    QString name() const override { return "B+树"; }
    QString chapter() const override { return "第7章 查找"; }
    QString description() const override {
        return "B+树的交互式插入演示，阶数可调（3~7阶），含分裂动画";
    }

    void run() override; void step() override; void reset() override;
    QVector<QString> parameterNames() const override {
        return {"order", "initialValues"};
    }
    QVector<QString> parameterDescriptions() const override {
        return {"B+树阶数(3~7)", "初始序列(空格分隔)"};
    }
    QVector<QVariant> parameterDefaults() const override {
        return {QVariant(3), QVariant(QString("10 20 30 40 50 15 25 35 45"))};
    }

private:
    struct BPNode {
        QVector<int> keys;
        QVector<BPNode*> children;
        BPNode *next = nullptr;
        bool isLeaf = true;
        ~BPNode() { for (auto c : children) delete c; }
    };

    void insert(int k);
    void insertNonFull(BPNode *node, int k);
    void splitChild(BPNode *parent, int idx);

    QVariantMap buildTreeSnapshot(const QString &title);
    void collectViz(BPNode *node, int &id, int parentId, QVariantList &nodes);

    BPNode *m_root = nullptr;
    int m_order = 3;
    int m_stepIndex = 0;
};

#endif // BPLUSTREE_H
