#ifndef KMPNEXTVAL_H
#define KMPNEXTVAL_H

#include "algorithmbase.h"

class KMPNextVal : public AlgorithmBase {
    Q_OBJECT
public:
    explicit KMPNextVal(QObject *parent = nullptr);
    QString name() const override { return "KMP算法的优化"; }
    QString chapter() const override { return "第4章 串"; }
    QString description() const override { return "KMP算法的nextval数组优化"; }

    void run() override;
    void step() override;
    void reset() override;

    QVector<QString> parameterNames() const override;
    QVector<QString> parameterDescriptions() const override;
    QVector<QVariant> parameterDefaults() const override;

private:
    QString m_pattern;
    int m_stepIndex = 0;
};

#endif // KMPNEXTVAL_H
