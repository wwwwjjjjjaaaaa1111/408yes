#ifndef BINARYSEARCH_H
#define BINARYSEARCH_H
#include "algorithmbase.h"
class BinarySearch : public AlgorithmBase {
    Q_OBJECT
public:
    explicit BinarySearch(QObject *parent = nullptr);
    QString name() const override { return "折半查找"; }
    QString chapter() const override { return "第7章 查找"; }
    QString description() const override { return "二分查找（要求数据有序），显示查找过程和判定树"; }
    void run() override; void step() override; void reset() override;
    QVector<QString> parameterNames() const override { return {"target"}; }
    QVector<QString> parameterDescriptions() const override { return {"查找目标值"}; }
    QVector<QVariant> parameterDefaults() const override { return {QVariant(34)}; }
private:
    int m_stepIndex = 0;
};
#endif
