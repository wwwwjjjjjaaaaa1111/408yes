#ifndef FORESTTRAVERSAL_H
#define FORESTTRAVERSAL_H

#include "algorithmbase.h"

class ForestTraversal : public AlgorithmBase {
    Q_OBJECT
public:
    explicit ForestTraversal(QObject *parent = nullptr);
    QString name() const override { return "树和森林的遍历"; }
    QString chapter() const override { return "第5章 树与二叉树"; }
    QString description() const override { return "演示树的先根遍历和后根遍历（对应二叉树的前序和中序）"; }

    void run() override;
    void step() override;
    void reset() override;
    QVector<QString> parameterNames() const override { return {}; }
    QVector<QString> parameterDescriptions() const override { return {}; }
    QVector<QVariant> parameterDefaults() const override { return {}; }
private:
    int m_stepIndex = 0;
};

#endif
