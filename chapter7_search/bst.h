#ifndef BST_H
#define BST_H
#include "algorithmbase.h"
#include <QMap>

class BST : public AlgorithmBase {
    Q_OBJECT
public:
    explicit BST(QObject *parent = nullptr);

    QString name() const override { return "二叉排序树(BST)"; }
    QString chapter() const override { return "第7章 查找"; }
    QString description() const override {
        return "BST的交互式构建、插入、删除、查找操作，含逐步动画演示";
    }

    void run() override; void step() override; void reset() override;

    QVector<QString> parameterNames() const override {
        return {"initialValues", "operation", "targetValue"};
    }
    QVector<QString> parameterDescriptions() const override {
        return {"初始序列(空格分隔)", "操作类型", "目标值"};
    }
    QVector<QVariant> parameterDefaults() const override {
        return {QVariant(QString("50 30 70 20 40 60 80")),
                QVariant(QVariantList{"构建", "插入", "删除", "查找"}),
                QVariant(35)};
    }

private:
    struct Node { int val; Node *l = nullptr, *r = nullptr; Node(int v) : val(v) {} };

    Node* insert(Node *n, int v);
    Node* findMin(Node *n);
    void deleteTree(Node *n);
    void inorderString(Node *n, QString &s);

    void insertAnimated(int v);
    void deleteAnimated(int v);
    void searchAnimated(int v);

    QVariantMap buildTreeSnapshot(const QString &title,
                                   const QMap<int, QString> &highlightVals = {});
    void collectNodes(Node *n, int &id, int parentId,
                      QVariantList &nodes, const QMap<int, QString> &highlightVals);

    Node *m_root = nullptr;
    int m_stepIndex = 0;
};

#endif // BST_H
