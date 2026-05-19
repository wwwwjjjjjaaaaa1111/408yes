#ifndef BUBBLESORT_H
#define BUBBLESORT_H
#include "algorithmbase.h"

class ExchangeSort : public AlgorithmBase {
    Q_OBJECT
public:
    explicit ExchangeSort(QObject *parent = nullptr);
    QString name() const override { return "交换排序(冒泡/快速)"; }
    QString chapter() const override { return "第8章 排序"; }
    QString description() const override { return "冒泡排序和快速排序（显示枢轴选择及分区过程）"; }

    void run() override; void step() override; void reset() override;
    QVector<QString> parameterNames() const override;
    QVector<QString> parameterDescriptions() const override;
    QVector<QVariant> parameterDefaults() const override;

private:
    void bubbleSort(QVector<int> &arr);
    void quickSort(QVector<int> &arr, int low, int high);
    int partition(QVector<int> &arr, int low, int high);
    QVector<int> generateArray(const QString &order);
    QString m_mode = "冒泡排序";
    QString m_order = "随机";
    int m_stepIndex = 0;
    QVector<int> m_arr;
    int m_comps = 0, m_swaps = 0;
};

#endif
