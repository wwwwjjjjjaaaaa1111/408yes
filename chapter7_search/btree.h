#ifndef BTREE_H
#define BTREE_H
#include "algorithmbase.h"
#include <QMap>
#include <functional>

class BTree : public AlgorithmBase {
    Q_OBJECT
public:
    explicit BTree(QObject *parent = nullptr);
    QString name() const override { return "B树"; }
    QString chapter() const override { return "第7章 查找"; }
    QString description() const override {
        return "B树的交互式插入演示，阶数可调（3~7阶），含分裂动画";
    }

    void run() override; void step() override; void reset() override;
    QVector<QString> parameterNames() const override {
        return {"order", "initialValues"};
    }
    QVector<QString> parameterDescriptions() const override {
        return {"B树阶数(3~7)", "初始序列(空格分隔)"};
    }
    QVector<QVariant> parameterDefaults() const override {
        return {QVariant(3), QVariant(QString("10 20 30 40 50 15 25 35 45"))};
    }

private:
    struct BNode {
        QVector<int> keys;
        QVector<BNode*> children;
        bool leaf = true;
        ~BNode() { for (auto c : children) delete c; }
    };

    void insert(int k);
    void insertNonFull(BNode *node, int k);
    void splitChild(BNode *parent, int idx);

    QVariantMap buildTreeSnapshot(const QString &title);
    void collectViz(BNode *node, int &id, int parentId,
                     QVariantList &nodes, const QString &colorFilter);

    BNode *m_root = nullptr;
    int m_order = 3;
    int m_stepIndex = 0;
};

#endif // BTREE_H
