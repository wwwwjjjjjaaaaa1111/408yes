#ifndef DFS_H
#define DFS_H

#include "algorithmbase.h"

class DFS : public AlgorithmBase {
    Q_OBJECT
public:
    explicit DFS(QObject *parent = nullptr);
    QString name() const override { return "深度优先搜索(DFS)"; }
    QString chapter() const override { return "第6章 图"; }
    QString description() const override { return "图的深度优先搜索（递归与非递归实现）"; }

    void run() override;
    void step() override;
    void reset() override;
    QVector<QString> parameterNames() const override;
    QVector<QString> parameterDescriptions() const override;
    QVector<QVariant> parameterDefaults() const override;

private:
    void dfsRecursive(int v, QVector<bool> &visited, const QVector<QVector<int>> &graph, QString &order);
    int m_startVertex = 0;
    int m_stepIndex = 0;
};

#endif
