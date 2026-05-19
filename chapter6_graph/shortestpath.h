#ifndef SHORTESTPATH_H
#define SHORTESTPATH_H

#include "algorithmbase.h"

class ShortestPath : public AlgorithmBase {
    Q_OBJECT
public:
    explicit ShortestPath(QObject *parent = nullptr);
    QString name() const override { return "最短路径(Dijkstra & Floyd)"; }
    QString chapter() const override { return "第6章 图"; }
    QString description() const override { return "Dijkstra算法和Floyd算法求最短路径，可切换"; }

    void run() override;
    void step() override;
    void reset() override;
    QVector<QString> parameterNames() const override;
    QVector<QString> parameterDescriptions() const override;
    QVector<QVariant> parameterDefaults() const override;

private:
    void runDijkstra();
    void runFloyd();
    QString m_mode = "Dijkstra";
    int m_stepIndex = 0;
};

#endif
