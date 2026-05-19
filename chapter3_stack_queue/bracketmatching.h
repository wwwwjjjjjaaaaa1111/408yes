#ifndef BRACKETMATCHING_H
#define BRACKETMATCHING_H

#include "algorithmbase.h"

class BracketMatching : public AlgorithmBase {
    Q_OBJECT
public:
    explicit BracketMatching(QObject *parent = nullptr);

    QString name() const override { return "栈在括号匹配中的应用"; }
    QString chapter() const override { return "第3章 栈、队列和数组"; }
    QString description() const override { return "使用栈检查表达式中括号是否匹配，支持 () [] {} 三种括号"; }

    void run() override;
    void step() override;
    void reset() override;

    QVector<QString> parameterNames() const override;
    QVector<QString> parameterDescriptions() const override;
    QVector<QVariant> parameterDefaults() const override;

private:
    QString m_expression;
    int m_stepIndex = 0;
    bool m_isBalanced = true;
    QString m_errorMsg;
};

#endif // BRACKETMATCHING_H
