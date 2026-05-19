#include "kmp.h"

KMPMatch::KMPMatch(QObject *parent) : AlgorithmBase(parent) {}

QVector<QString> KMPMatch::parameterNames() const {
    return {"text", "pattern"};
}
QVector<QString> KMPMatch::parameterDescriptions() const {
    return {"主串文本", "模式串"};
}
QVector<QVariant> KMPMatch::parameterDefaults() const {
    return {QString("ababcabcacbab"), QString("abcac")};
}

QVector<int> KMPMatch::computeNext(const QString &pattern) {
    int m = pattern.size();
    QVector<int> next(m, 0);
    next[0] = -1; // 约定next[0]=-1
    addStep("===== 计算next数组 =====");
    addStep(QString("next[0] = -1 (约定)"));

    int i = 0, j = -1;
    while (i < m - 1) {
        if (j == -1 || pattern[i] == pattern[j]) {
            i++; j++;
            next[i] = j;
            addStep(QString("P[%1]=%2 == P[%3]=%4 → next[%5]=%6")
                    .arg(i).arg(pattern[i]).arg(j-1>=0?j-1:0)
                    .arg(j-1>=0?pattern[j-1]:QChar('-'))
                    .arg(i).arg(j));
        } else {
            addStep(QString("P[%1]=%2 != P[%3]，j=next[%4]=%5 → 回溯")
                    .arg(i).arg(pattern[i]).arg(j>=0?pattern[j]:QChar('-'))
                    .arg(j).arg(next.value(j, -1)));
            j = next[j];
        }
    }

    QString nextStr = "next数组: [";
    for (int k = 0; k < m; k++) {
        nextStr += QString::number(next[k]) + (k < m-1 ? ", " : "");
    }
    nextStr += "]";
    addStep(nextStr);
    return next;
}

void KMPMatch::run() {
    reset();
    m_text = m_params.value("text", "ababcabcacbab").toString();
    m_pattern = m_params.value("pattern", "abcac").toString();
    m_compareCount = 0;

    addStep(QString("KMP匹配: 主串=\"%1\"，模式=\"%2\"").arg(m_text).arg(m_pattern));

    auto next = computeNext(m_pattern);

    addStep("===== KMP匹配过程 =====");
    int n = m_text.size(), m = m_pattern.size();
    int i = 0, j = 0;

    while (i < n) {
        if (j == -1 || m_text[i] == m_pattern[j]) {
            m_compareCount++;
            if (j >= 0)
                addStep(QString("比较: S[%1]=%2 == P[%3]=%4 ✓，i++, j++")
                        .arg(i).arg(m_text[i]).arg(j).arg(m_pattern[j]));
            else
                addStep(QString("j=-1，i=%1++, j=0").arg(i));
            i++; j++;
        } else {
            m_compareCount++;
            addStep(QString("比较: S[%1]=%2 != P[%3]=%4 ✗，j=next[%5]=%6")
                    .arg(i).arg(m_text[i]).arg(j).arg(m_pattern[j]).arg(j).arg(next[j]));
            j = next[j];
        }
        if (j == m) {
            addStep(QString(">>> 在位置%1找到匹配!").arg(i - m));
            j = next[j - 1];
        }
    }

    m_resultText = QString("比较次数: %1\nnext数组: [")
        .arg(m_compareCount);
    for (int k = 0; k < next.size(); k++)
        m_resultText += QString::number(next[k]) + (k < next.size()-1 ? ", " : "]");

    m_running = false;
}

void KMPMatch::step() { if (m_stepIndex == 0) run(); }
void KMPMatch::reset() { m_steps.clear(); m_stepIndex = 0; m_compareCount = 0; m_resultText.clear(); }
