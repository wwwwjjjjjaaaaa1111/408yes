#ifndef KMP_H
#define KMP_H

#include "algorithmbase.h"

class KMPMatch : public AlgorithmBase {
    Q_OBJECT
public:
    explicit KMPMatch(QObject *parent = nullptr);
    QString name() const override { return "KMP算法"; }
    QString chapter() const override { return "第4章 串"; }
    QString description() const override { return "KMP模式匹配算法，含next数组生成过程"; }

    void run() override;
    void step() override;
    void reset() override;

    QVector<QString> parameterNames() const override;
    QVector<QString> parameterDescriptions() const override;
    QVector<QVariant> parameterDefaults() const override;

private:
    QVector<int> computeNext(const QString &pattern);
    QString m_text;
    QString m_pattern;
    int m_stepIndex = 0;
    int m_compareCount = 0;
};

#endif // KMP_H
