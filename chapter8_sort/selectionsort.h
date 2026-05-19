#ifndef SELECTIONSORT_H
#define SELECTIONSORT_H
#include "algorithmbase.h"

class SelectionSort : public AlgorithmBase {
    Q_OBJECT
public:
    explicit SelectionSort(QObject *parent = nullptr);
    QString name() const override { return "选择排序(简单选择/堆排序)"; }
    QString chapter() const override { return "第8章 排序"; }
    QString description() const override { return "简单选择排序和堆排序（显示建堆与调整过程）"; }

    void run() override; void step() override; void reset() override;
    QVector<QString> parameterNames() const override;
    QVector<QString> parameterDescriptions() const override;
    QVector<QVariant> parameterDefaults() const override;

private:
    void selectionSort(QVector<int> &arr);
    void heapSort(QVector<int> &arr);
    void heapify(QVector<int> &arr, int n, int i);
    QVector<int> generateArray(const QString &order);
    QString m_mode = "简单选择排序";
    QString m_order = "随机";
    int m_stepIndex = 0;
    QVector<int> m_arr;
    int m_comps = 0, m_swaps = 0;
};

#endif
