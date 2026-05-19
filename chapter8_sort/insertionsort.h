#ifndef INSERTIONSORT_H
#define INSERTIONSORT_H
#include "algorithmbase.h"

class InsertionSort : public AlgorithmBase {
    Q_OBJECT
public:
    explicit InsertionSort(QObject *parent = nullptr);
    QString name() const override { return "插入排序(直接/折半/希尔)"; }
    QString chapter() const override { return "第8章 排序"; }
    QString description() const override { return "直接插入排序、折半插入排序、希尔排序，可切换算法"; }

    void run() override; void step() override; void reset() override;
    QVector<QString> parameterNames() const override;
    QVector<QString> parameterDescriptions() const override;
    QVector<QVariant> parameterDefaults() const override;

private:
    void directInsertion(QVector<int> &arr);
    void binaryInsertion(QVector<int> &arr);
    void shellSort(QVector<int> &arr);
    QVector<int> generateArray(const QString &order);
    QString m_mode = "直接插入";
    QString m_order = "随机";
    int m_stepIndex = 0;
    QVector<int> m_arr;
    int m_comps = 0, m_swaps = 0;
};

#endif
