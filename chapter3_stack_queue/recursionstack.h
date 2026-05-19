#ifndef RECURSIONSTACK_H
#define RECURSIONSTACK_H

#include "algorithmbase.h"

class RecursionStack : public AlgorithmBase {
    Q_OBJECT
public:
    explicit RecursionStack(QObject *parent = nullptr);
    QString name() const override { return "栈在递归中的应用"; }
    QString chapter() const override { return "第3章 栈、队列和数组"; }
    QString description() const override { return "演示斐波那契递归转非递归(手动栈模拟)"; }

    void run() override;
    void step() override;
    void reset() override;

    QVector<QString> parameterNames() const override;
    QVector<QString> parameterDescriptions() const override;
    QVector<QVariant> parameterDefaults() const override;

private:
    int fibRecursive(int n);
    int fibIterative(int n);
    int m_n = 10;
    int m_stepIndex = 0;
};

#endif // RECURSIONSTACK_H
