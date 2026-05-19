#ifndef MERGESORT_H
#define MERGESORT_H
#include "algorithmbase.h"

class MergeSort : public AlgorithmBase {
    Q_OBJECT
public:
    explicit MergeSort(QObject *parent = nullptr);
    QString name() const override { return "归并排序"; }
    QString chapter() const override { return "第8章 排序"; }
    QString description() const override { return "二路归并排序，显示分治与合并过程"; }

    void run() override; void step() override; void reset() override;
    QVector<QString> parameterNames() const override;
    QVector<QString> parameterDescriptions() const override;
    QVector<QVariant> parameterDefaults() const override;

private:
    void mergeSort(QVector<int> &arr, int left, int right);
    void merge(QVector<int> &arr, int left, int mid, int right);
    QVector<int> generateArray(const QString &order);
    QString m_order = "随机";
    int m_stepIndex = 0;
    QVector<int> m_arr;
    int m_comps = 0;
};

#endif
