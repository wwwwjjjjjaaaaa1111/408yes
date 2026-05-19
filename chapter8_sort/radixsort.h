#ifndef RADIXSORT_H
#define RADIXSORT_H
#include "algorithmbase.h"

class RadixSort : public AlgorithmBase {
    Q_OBJECT
public:
    explicit RadixSort(QObject *parent = nullptr);
    QString name() const override { return "基数排序"; }
    QString chapter() const override { return "第8章 排序"; }
    QString description() const override { return "LSD基数排序，按位分配与收集"; }

    void run() override; void step() override; void reset() override;
    QVector<QString> parameterNames() const override { return {"size"}; }
    QVector<QString> parameterDescriptions() const override { return {"数据规模"}; }
    QVector<QVariant> parameterDefaults() const override { return {QVariant(10)}; }
private:
    int m_stepIndex = 0;
};

#endif
