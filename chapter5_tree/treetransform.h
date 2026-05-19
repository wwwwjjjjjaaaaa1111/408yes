#ifndef TREETRANSFORM_H
#define TREETRANSFORM_H

#include "algorithmbase.h"

class TreeTransform : public AlgorithmBase {
    Q_OBJECT
public:
    explicit TreeTransform(QObject *parent = nullptr);
    QString name() const override { return "树、森林与二叉树的转换"; }
    QString chapter() const override { return "第5章 树与二叉树"; }
    QString description() const override { return "演示树↔二叉树、森林↔二叉树的转换过程(左孩子右兄弟)"; }

    void run() override;
    void step() override;
    void reset() override;

    QVector<QString> parameterNames() const override;
    QVector<QString> parameterDescriptions() const override;
    QVector<QVariant> parameterDefaults() const override;

private:
    int m_stepIndex = 0;
};

#endif
