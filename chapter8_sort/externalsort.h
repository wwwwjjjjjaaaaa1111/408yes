#ifndef EXTERNALSORT_H
#define EXTERNALSORT_H
#include "algorithmbase.h"

class ExternalSort : public AlgorithmBase {
    Q_OBJECT
public:
    explicit ExternalSort(QObject *parent = nullptr);
    QString name() const override { return "外部排序"; }
    QString chapter() const override { return "第8章 排序"; }
    QString description() const override { return "多路平衡归并、败者树、置换-选择排序、最佳归并树"; }

    void run() override; void step() override; void reset() override;
    QVector<QString> parameterNames() const override { return {"mode"}; }
    QVector<QString> parameterDescriptions() const override { return {"演示主题"}; }
    QVector<QVariant> parameterDefaults() const override {
        return {QVariant(QVariantList{"多路平衡归并", "败者树", "置换-选择排序", "最佳归并树"})};
    }
private:
    void demoMultiwayMerge();
    void demoLoserTree();
    void demoReplacementSelection();
    void demoOptimalMergeTree();
    QString m_mode;
    int m_stepIndex = 0;
};

#endif
