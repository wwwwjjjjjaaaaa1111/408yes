#include "expressioneval.h"
#include <QStack>

ExpressionEval::ExpressionEval(QObject *parent) : AlgorithmBase(parent) {}

QVector<QString> ExpressionEval::parameterNames() const {
    return {"expression"};
}
QVector<QString> ExpressionEval::parameterDescriptions() const {
    return {"输入中缀表达式(数字和+-*/())"};
}
QVector<QVariant> ExpressionEval::parameterDefaults() const {
    return {QString("3+5*(2-8)/3")};
}

int ExpressionEval::precedence(QChar op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

QString ExpressionEval::infixToPostfix(const QString &infix) {
    QStack<QChar> ops;
    QString postfix;
    int i = 0;

    addStep(QString("中缀转后缀: %1").arg(infix));

    while (i < infix.size()) {
        QChar ch = infix[i];
        if (ch.isDigit() || ch == '.') {
            QString num;
            while (i < infix.size() && (infix[i].isDigit() || infix[i] == '.')) {
                num += infix[i];
                i++;
            }
            postfix += num + " ";
            addStep(QString("读取操作数 %1，加入后缀表达式 → %2").arg(num).arg(postfix.trimmed()));
            continue;
        } else if (ch == '(') {
            ops.push(ch);
            addStep(QString("遇到 '('，入运算符栈"));
        } else if (ch == ')') {
            while (!ops.isEmpty() && ops.top() != '(') {
                postfix += ops.top();
                postfix += " ";
                addStep(QString("弹出运算符 '%1' → %2").arg(ops.top()).arg(postfix.trimmed()));
                ops.pop();
            }
            if (!ops.isEmpty()) ops.pop(); // remove '('
            addStep("遇到 ')'，弹出左括号");
        } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            while (!ops.isEmpty() && ops.top() != '(' &&
                   precedence(ops.top()) >= precedence(ch)) {
                postfix += ops.top();
                postfix += " ";
                addStep(QString("比较优先级: '%1' >= '%2'，弹出 '%3' → %4")
                        .arg(ops.top()).arg(ch).arg(ops.top()).arg(postfix.trimmed()));
                ops.pop();
            }
            ops.push(ch);
            addStep(QString("运算符 '%1' 入栈").arg(ch));
        }
        i++;
    }

    while (!ops.isEmpty()) {
        postfix += ops.top();
        postfix += " ";
        addStep(QString("弹出剩余运算符 '%1'").arg(ops.top()));
        ops.pop();
    }

    addStep(QString("后缀表达式: %1").arg(postfix.trimmed()));
    return postfix.trimmed();
}

double ExpressionEval::evalPostfix(const QString &postfix, bool &ok) {
    QStack<double> stack;
    ok = true;
    QStringList tokens = postfix.split(' ', Qt::SkipEmptyParts);
    addStep(QString("计算后缀表达式: %1").arg(postfix));

    for (const QString &token : tokens) {
        bool isNum = false;
        double val = token.toDouble(&isNum);
        if (isNum) {
            stack.push(val);
            addStep(QString("push %1，栈: %2").arg(val).arg(
                [&](){ QString s; for(auto v:stack)s+=QString::number(v)+" "; return s.trimmed(); }()));
        } else if (token.size() == 1) {
            QChar op = token[0];
            if (stack.size() < 2) { ok = false; return 0; }
            double b = stack.pop();
            double a = stack.pop();
            double result = 0;
            if (op == '+') result = a + b;
            else if (op == '-') result = a - b;
            else if (op == '*') result = a * b;
            else if (op == '/' && b != 0) result = a / b;
            stack.push(result);
            addStep(QString("%1 %2 %3 = %4，push %4").arg(a).arg(op).arg(b).arg(result));
        }
    }
    ok = stack.size() == 1;
    double result = ok ? stack.top() : 0;
    addStep(QString("最终结果: %1").arg(result));
    return result;
}

void ExpressionEval::run() {
    reset();
    m_infix = m_params.value("expression", "3+5*(2-8)/3").toString();
    addStep(QString("原始中缀表达式: %1").arg(m_infix));
    QString postfix = infixToPostfix(m_infix);
    bool ok;
    double result = evalPostfix(postfix, ok);
    if (ok)
        m_resultText = QString("后缀表达式: %1\n计算结果: %2").arg(postfix).arg(result);
    else
        m_resultText = "表达式求值失败!";
    m_running = false;
}

void ExpressionEval::step() { if (m_stepIndex == 0) run(); }
void ExpressionEval::reset() { m_steps.clear(); m_stepIndex = 0; m_resultText.clear(); }
