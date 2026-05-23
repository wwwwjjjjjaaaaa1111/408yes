#ifndef RBTREE_H
#define RBTREE_H
#include "algorithmbase.h"
#include <QMap>

class RBTree : public AlgorithmBase {
    Q_OBJECT
public:
    explicit RBTree(QObject *parent = nullptr);
    QString name() const override { return "红黑树"; }
    QString chapter() const override { return "第7章 查找"; }
    QString description() const override {
        return "红黑树的交互式插入演示（含颜色调整与旋转动画）";
    }

    void run() override; void step() override; void reset() override;

    QVector<QString> parameterNames() const override {
        return {"initialValues", "operation", "targetValue"};
    }
    QVector<QString> parameterDescriptions() const override {
        return {"初始序列(空格分隔)", "操作类型", "目标值"};
    }
    QVector<QVariant> parameterDefaults() const override {
        return {QVariant(QString("10 20 30 15 25 5 35")),
                QVariant(QVariantList{"构建", "插入"}),
                QVariant(40)};
    }

private:
    enum Color {RED, BLACK};
    struct Node { int val; Color c = RED; Node *l = nullptr, *r = nullptr, *p = nullptr;
                  Node(int v) : val(v) {} };

    void rotateLeft(Node *x);
    void rotateRight(Node *y);
    void fixInsert(Node *z);
    void insertAnimated(int v);
    void insertSilent(int v);  // no snapshots, used during fixInsert re-insertion

    void deleteTree(Node *n);
    void inorderString(Node *n, QString &s);

    QVariantMap buildTreeSnapshot(const QString &title,
                                   const QMap<int, QString> &highlightVals = {});
    void collectNodes(Node *n, int &id, int pid, QVariantList &nodes,
                      const QMap<int, QString> &highlightVals);

    Node *m_root = nullptr;
    int m_stepIndex = 0;
};

#endif // RBTREE_H
