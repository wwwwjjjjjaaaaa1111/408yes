#ifndef BFS_H
#define BFS_H

#include "algorithmbase.h"

class BFS : public AlgorithmBase {
    Q_OBJECT
public:
    explicit BFS(QObject *parent = nullptr);
    QString name() const override { return "广度优先搜索(BFS)"; }
    QString chapter() const override { return "第6章 图"; }
    QString description() const override { return "图的广度优先搜索，显示遍历顺序"; }

    void run() override;
    void step() override;
    void reset() override;
    QVector<QString> parameterNames() const override;
    QVector<QString> parameterDescriptions() const override;
    QVector<QVariant> parameterDefaults() const override;

private:
    QVector<QVector<int>> buildSampleGraph();
    int m_startVertex = 0;
    int m_stepIndex = 0;
};

#endif
