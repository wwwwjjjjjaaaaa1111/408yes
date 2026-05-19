#ifndef TOPOSORT_H
#define TOPOSORT_H

#include "algorithmbase.h"

class TopoSort : public AlgorithmBase {
    Q_OBJECT
public:
    explicit TopoSort(QObject *parent = nullptr);
    QString name() const override { return "拓扑排序"; }
    QString chapter() const override { return "第6章 图"; }
    QString description() const override { return "AOV网的拓扑排序，显示结果与环检测"; }

    void run() override;
    void step() override;
    void reset() override;
    QVector<QString> parameterNames() const override { return {}; }
    QVector<QString> parameterDescriptions() const override { return {}; }
    QVector<QVariant> parameterDefaults() const override { return {}; }
private:
    int m_stepIndex = 0;
};

#endif
