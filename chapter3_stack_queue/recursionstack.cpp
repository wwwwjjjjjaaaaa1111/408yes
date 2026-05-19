#include "recursionstack.h"
#include <QStack>

RecursionStack::RecursionStack(QObject *parent) : AlgorithmBase(parent) {}

QVector<QString> RecursionStack::parameterNames() const {
    return {"n"};
}
QVector<QString> RecursionStack::parameterDescriptions() const {
    return {"斐波那契数列项数(n≥0)"};
}
QVector<QVariant> RecursionStack::parameterDefaults() const {
    return {QVariant(10)};
}

int RecursionStack::fibRecursive(int n) {
    if (n <= 1) {
        addStep(QString("递归: fib(%1) = %1 (基准情况)").arg(n));
        return n;
    }
    addStep(QString("递归: fib(%1) = fib(%2) + fib(%3)，展开").arg(n).arg(n-1).arg(n-2));
    int result = fibRecursive(n-1) + fibRecursive(n-2);
    addStep(QString("递归返回: fib(%1) = %2").arg(n).arg(result));
    return result;
}

int RecursionStack::fibIterative(int n) {
    if (n <= 1) {
        addStep(QString("非递归: fib(%1) = %1").arg(n));
        return n;
    }

    QStack<QPair<int, int>> stack; // <n, state: 0=enter, 1=after first, 2=done>
    stack.push({n, 0});
    QMap<int, int> memo;
    int finalResult = 0;

    addStep(QString("非递归方式: 手动栈模拟fib(%1)").arg(n));

    while (!stack.isEmpty()) {
        auto top = stack.pop();
        int k = top.first, state = top.second;

        if (k <= 1) {
            memo[k] = k;
            addStep(QString("栈处理: fib(%1) = %1 (基准)").arg(k));
        } else if (state == 0) {
            stack.push({k, 1});
            stack.push({k-1, 0});
            addStep(QString("栈: push fib(%1)，状态=1；push fib(%2)").arg(k).arg(k-1));
        } else if (state == 1) {
            stack.push({k, 2});
            stack.push({k-2, 0});
            addStep(QString("栈: push fib(%1)，状态=2；push fib(%2)").arg(k).arg(k-2));
        } else {
            int f1 = memo.value(k-1, 0);
            int f2 = memo.value(k-2, 0);
            memo[k] = f1 + f2;
            addStep(QString("栈计算: fib(%1) = fib(%2)+fib(%3) = %4+%5 = %6")
                    .arg(k).arg(k-1).arg(k-2).arg(f1).arg(f2).arg(memo[k]));
        }
    }
    finalResult = memo[n];
    addStep(QString("最终结果: fib(%1) = %2").arg(n).arg(finalResult));
    return finalResult;
}

void RecursionStack::run() {
    reset();
    m_n = m_params.value("n", 10).toInt();
    m_n = qBound(0, m_n, 30);

    addStep(QString("===== 递归方式计算 fib(%1) =====").arg(m_n));
    int r1 = fibRecursive(m_n);

    m_steps.clear();
    addStep(QString("===== 非递归方式(栈模拟)计算 fib(%1) =====").arg(m_n));
    int r2 = fibIterative(m_n);

    m_resultText = QString("递归结果: %1\n非递归(栈模拟)结果: %2\n两种方式结果一致: %3")
        .arg(r1).arg(r2).arg(r1 == r2 ? "是" : "否");
    m_running = false;
}

void RecursionStack::step() { if (m_stepIndex == 0) run(); }
void RecursionStack::reset() { m_steps.clear(); m_stepIndex = 0; m_resultText.clear(); }
