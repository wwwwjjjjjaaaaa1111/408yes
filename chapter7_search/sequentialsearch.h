#ifndef SEQUENTIALSEARCH_H
#define SEQUENTIALSEARCH_H

#include "algorithmbase.h"

class SequentialSearch : public AlgorithmBase {
    Q_OBJECT
public:
    explicit SequentialSearch(QObject *parent = nullptr);
    QString name() const override { return "顺序查找"; }
    QString chapter() const override { return "第7章 查找"; }
    QString description() const override { return "顺序查找（含哨兵优化），统计比较次数"; }

    void run() override; void step() override; void reset() override;
    QVector<QString> parameterNames() const override;
    QVector<QString> parameterDescriptions() const override;
    QVector<QVariant> parameterDefaults() const override;
private:
    int m_stepIndex = 0;
};

#endif
