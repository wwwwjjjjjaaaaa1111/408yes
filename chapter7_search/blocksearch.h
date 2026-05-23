#ifndef BLOCKSEARCH_H
#define BLOCKSEARCH_H
#include "algorithmbase.h"

class BlockSearch : public AlgorithmBase {
    Q_OBJECT
public:
    explicit BlockSearch(QObject *parent = nullptr);
    QString name() const override { return "分块查找"; }
    QString chapter() const override { return "第7章 查找"; }
    QString description() const override {
        return "分块查找（索引顺序查找），自定义序列和块大小，两步动画演示";
    }
    void run() override; void step() override; void reset() override;
    QVector<QString> parameterNames() const override {
        return {"initialValues", "blockSize", "target"};
    }
    QVector<QString> parameterDescriptions() const override {
        return {"序列(空格分隔，自动排序)", "块大小", "查找目标值"};
    }
    QVector<QVariant> parameterDefaults() const override {
        return {QVariant(QString("3 8 12 15 21 22 27 29 34 39 42 45 51 58 63 67 71 75 82 88")),
                QVariant(5), QVariant(42)};
    }

private:
    QVariantMap makeBlockSnapshot(const QVector<int> &arr,
                                   const QVector<QVector<int>> &blocks,
                                   int highlightBlock, int highlightIdx,
                                   bool indexPhase, const QString &title);
    int m_stepIndex = 0;
};

#endif
