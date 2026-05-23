#ifndef BINARYSEARCH_H
#define BINARYSEARCH_H
#include "algorithmbase.h"

class BinarySearch : public AlgorithmBase {
    Q_OBJECT
public:
    explicit BinarySearch(QObject *parent = nullptr);
    QString name() const override { return "折半查找"; }
    QString chapter() const override { return "第7章 查找"; }
    QString description() const override {
        return "二分查找（要求数据有序），自定义序列，显示区间收缩过程";
    }
    void run() override; void step() override; void reset() override;
    QVector<QString> parameterNames() const override {
        return {"initialValues", "target"};
    }
    QVector<QString> parameterDescriptions() const override {
        return {"有序序列(空格分隔，自动排序)", "查找目标值"};
    }
    QVector<QVariant> parameterDefaults() const override {
        return {QVariant(QString("3 7 12 18 22 27 34 39 45 51 58 63 69 75 82 88 94 99")),
                QVariant(34)};
    }

private:
    QVariantMap makeIntervalSnapshot(const QVector<int> &arr, int left, int right,
                                      int mid, bool found, const QString &title);
    int m_stepIndex = 0;
};

#endif
