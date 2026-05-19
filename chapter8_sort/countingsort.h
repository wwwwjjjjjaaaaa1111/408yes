#ifndef COUNTINGSORT_H
#define COUNTINGSORT_H
#include "algorithmbase.h"

class CountingSort : public AlgorithmBase {
    Q_OBJECT
public:
    explicit CountingSort(QObject *parent = nullptr);
    QString name() const override { return "计数排序"; }
    QString chapter() const override { return "第8章 排序"; }
    QString description() const override { return "计数排序，适用于范围有限的整数排序"; }

    void run() override; void step() override; void reset() override;
    QVector<QString> parameterNames() const override { return {"size"}; }
    QVector<QString> parameterDescriptions() const override { return {"数据规模"}; }
    QVector<QVariant> parameterDefaults() const override { return {QVariant(15)}; }
private:
    int m_stepIndex = 0;
};

#endif
