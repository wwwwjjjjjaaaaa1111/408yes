#ifndef SEQUENTIALSEARCH_H
#define SEQUENTIALSEARCH_H

#include "algorithmbase.h"

class SequentialSearch : public AlgorithmBase {
    Q_OBJECT
public:
    explicit SequentialSearch(QObject *parent = nullptr);
    QString name() const override { return "顺序查找"; }
    QString chapter() const override { return "第7章 查找"; }
    QString description() const override {
        return "顺序查找（含哨兵优化），自定义数组，逐步显示查找过程";
    }

    void run() override; void step() override; void reset() override;
    QVector<QString> parameterNames() const override {
        return {"initialValues", "target"};
    }
    QVector<QString> parameterDescriptions() const override {
        return {"数组序列(空格分隔)", "查找目标值"};
    }
    QVector<QVariant> parameterDefaults() const override {
        return {QVariant(QString("12 45 23 67 34 89 15 37 56 78 90 21 44 33 55")),
                QVariant(37)};
    }
private:
    int m_stepIndex = 0;
};

#endif
