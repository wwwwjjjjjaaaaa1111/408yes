#include "bracketmatching.h"
#include <QStack>

BracketMatching::BracketMatching(QObject *parent) : AlgorithmBase(parent) {}

QVector<QString> BracketMatching::parameterNames() const {
    return {"expression"};
}
QVector<QString> BracketMatching::parameterDescriptions() const {
    return {"输入表达式"};
}
QVector<QVariant> BracketMatching::parameterDefaults() const {
    return {QString("{a+[b*(c+d)]-e}")};
}

void BracketMatching::reset() {
    m_steps.clear();
    m_stepIndex = 0;
    m_isBalanced = true;
    m_errorMsg.clear();
    m_resultText.clear();
}

void BracketMatching::run() {
    reset();
    m_expression = m_params.value("expression", "{a+[b*(c+d)]-e}").toString();

    QStack<QPair<QChar, int>> stack;
    QString openBrackets = "([{";
    QString closeBrackets = ")]}";

    addStep(QString("开始检查表达式: %1").arg(m_expression));

    for (int i = 0; i < m_expression.size(); i++) {
        QChar ch = m_expression[i];
        int openIdx = openBrackets.indexOf(ch);
        int closeIdx = closeBrackets.indexOf(ch);

        if (openIdx >= 0) {
            stack.push({ch, i});
            addStep(QString("位置%1: 遇到左括号 '%2'，入栈。栈深度=%3")
                    .arg(i).arg(ch).arg(stack.size()));
        } else if (closeIdx >= 0) {
            if (stack.isEmpty()) {
                m_isBalanced = false;
                m_errorMsg = QString("位置%1: 遇到右括号 '%2'，但栈为空，不匹配!")
                    .arg(i).arg(ch);
                addStep(m_errorMsg);
                break;
            }
            auto top = stack.pop();
            if (openBrackets.indexOf(top.first) != closeIdx) {
                m_isBalanced = false;
                m_errorMsg = QString("位置%1: 右括号 '%2' 与栈顶 '%3'(位置%4) 类型不匹配!")
                    .arg(i).arg(ch).arg(top.first).arg(top.second);
                addStep(m_errorMsg);
                break;
            }
            addStep(QString("位置%1: 右括号 '%2' 与栈顶 '%3'(位置%4) 匹配，出栈。栈深度=%5")
                    .arg(i).arg(ch).arg(top.first).arg(top.second).arg(stack.size()));
        }
    }

    if (m_isBalanced && !stack.isEmpty()) {
        m_isBalanced = false;
        m_errorMsg = QString("栈中还有未匹配的左括号 %1 个").arg(stack.size());
        addStep(m_errorMsg);
    }

    if (m_isBalanced) {
        m_resultText = "括号匹配正确！";
        addStep("结论: 表达式括号完全匹配");
    } else {
        m_resultText = "括号不匹配: " + m_errorMsg;
    }
    m_running = false;
}

void BracketMatching::step() {
    // Simplified: just run all at once for bracket matching
    if (m_stepIndex == 0) run();
}
