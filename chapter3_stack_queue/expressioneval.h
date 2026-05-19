#ifndef EXPRESSIONEVAL_H
#define EXPRESSIONEVAL_H

#include "algorithmbase.h"

class ExpressionEval : public AlgorithmBase {
    Q_OBJECT
public:
    explicit ExpressionEval(QObject *parent = nullptr);
    QString name() const override { return "栈在表达式求值中的应用"; }
    QString chapter() const override { return "第3章 栈、队列和数组"; }
    QString description() const override { return "中缀表达式转后缀表达式并计算求值"; }

    void run() override;
    void step() override;
    void reset() override;

    QVector<QString> parameterNames() const override;
    QVector<QString> parameterDescriptions() const override;
    QVector<QVariant> parameterDefaults() const override;

private:
    int precedence(QChar op);
    QString infixToPostfix(const QString &infix);
    double evalPostfix(const QString &postfix, bool &ok);
    QString m_infix;
    int m_stepIndex = 0;
};

#endif // EXPRESSIONEVAL_H
