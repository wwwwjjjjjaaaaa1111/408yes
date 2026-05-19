#ifndef BLOCKSEARCH_H
#define BLOCKSEARCH_H
#include "algorithmbase.h"
class BlockSearch : public AlgorithmBase {
    Q_OBJECT
public:
    explicit BlockSearch(QObject *parent = nullptr);
    QString name() const override { return "分块查找"; }
    QString chapter() const override { return "第7章 查找"; }
    QString description() const override { return "分块查找（索引顺序查找），可调整块大小"; }
    void run() override; void step() override; void reset() override;
    QVector<QString> parameterNames() const override { return {"blockSize","target"}; }
    QVector<QString> parameterDescriptions() const override { return {"块大小","查找目标值"}; }
    QVector<QVariant> parameterDefaults() const override { return {QVariant(5),QVariant(42)}; }
private:
    int m_stepIndex = 0;
};
#endif
