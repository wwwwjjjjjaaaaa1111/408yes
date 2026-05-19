#ifndef UNIONFIND_H
#define UNIONFIND_H

#include "algorithmbase.h"

class UnionFind : public AlgorithmBase {
    Q_OBJECT
public:
    explicit UnionFind(QObject *parent = nullptr);
    QString name() const override { return "并查集"; }
    QString chapter() const override { return "第5章 树与二叉树"; }
    QString description() const override { return "演示并查集的合并、查找、路径压缩操作"; }

    void run() override;
    void step() override;
    void reset() override;

    QVector<QString> parameterNames() const override;
    QVector<QString> parameterDescriptions() const override;
    QVector<QVariant> parameterDefaults() const override;

private:
    int find(QVector<int> &parent, int x);
    void unionSet(QVector<int> &parent, QVector<int> &rank, int a, int b);
    int m_size = 10;
    int m_stepIndex = 0;
};

#endif
