#ifndef BPLUSTREE_H
#define BPLUSTREE_H
#include "algorithmbase.h"

class BPlusTree : public AlgorithmBase {
    Q_OBJECT
public:
    explicit BPlusTree(QObject *parent = nullptr);
    QString name() const override { return "B+树"; }
    QString chapter() const override { return "第7章 查找"; }
    QString description() const override { return "B+树的基本概念和结构展示"; }

    void run() override; void step() override; void reset() override;
    QVector<QString> parameterNames() const override { return {}; }
    QVector<QString> parameterDescriptions() const override { return {}; }
    QVector<QVariant> parameterDefaults() const override { return {}; }
private:
    int m_stepIndex = 0;
};

#endif
