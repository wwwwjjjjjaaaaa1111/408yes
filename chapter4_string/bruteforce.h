#ifndef BRUTEFORCE_H
#define BRUTEFORCE_H

#include "algorithmbase.h"

class BruteForceMatch : public AlgorithmBase {
    Q_OBJECT
public:
    explicit BruteForceMatch(QObject *parent = nullptr);
    QString name() const override { return "简单的模式匹配"; }
    QString chapter() const override { return "第4章 串"; }
    QString description() const override { return "暴力匹配算法(BF算法)，逐个字符比较"; }

    void run() override;
    void step() override;
    void reset() override;

    QVector<QString> parameterNames() const override;
    QVector<QString> parameterDescriptions() const override;
    QVector<QVariant> parameterDefaults() const override;

private:
    QString m_text;
    QString m_pattern;
    int m_stepIndex = 0;
    int m_compareCount = 0;
};

#endif // BRUTEFORCE_H
