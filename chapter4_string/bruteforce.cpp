#include "bruteforce.h"

BruteForceMatch::BruteForceMatch(QObject *parent) : AlgorithmBase(parent) {}

QVector<QString> BruteForceMatch::parameterNames() const {
    return {"text", "pattern"};
}
QVector<QString> BruteForceMatch::parameterDescriptions() const {
    return {"主串文本", "模式串"};
}
QVector<QVariant> BruteForceMatch::parameterDefaults() const {
    return {QString("ababcabcacbab"), QString("abcac")};
}

void BruteForceMatch::run() {
    reset();
    m_text = m_params.value("text", "ababcabcacbab").toString();
    m_pattern = m_params.value("pattern", "abcac").toString();
    m_compareCount = 0;

    addStep(QString("暴力匹配: 主串=\"%1\"(%2), 模式=\"%3\"(%4)")
            .arg(m_text).arg(m_text.size()).arg(m_pattern).arg(m_pattern.size()));

    int n = m_text.size(), m = m_pattern.size();
    QVector<int> matches;

    for (int i = 0; i <= n - m; i++) {
        int j = 0;
        addStep(QString("--- 主串位置i=%1，比较子串\"%2\" ---")
                .arg(i).arg(m_text.mid(i, m)));
        while (j < m) {
            m_compareCount++;
            if (m_text[i + j] == m_pattern[j]) {
                addStep(QString("  比较: S[%1]=%2 == P[%3]=%4 ✓")
                        .arg(i+j).arg(m_text[i+j]).arg(j).arg(m_pattern[j]));
                j++;
            } else {
                addStep(QString("  比较: S[%1]=%2 != P[%3]=%4 ✗ 失配")
                        .arg(i+j).arg(m_text[i+j]).arg(j).arg(m_pattern[j]));
                break;
            }
        }
        if (j == m) {
            matches.append(i);
            addStep(QString(">>> 在位置%1找到匹配!").arg(i));
        }
    }

    m_resultText = matches.isEmpty()
        ? QString("未找到匹配。比较次数: %1").arg(m_compareCount)
        : QString("匹配位置: %1。比较次数: %2")
            .arg([&](){QString s; for(int p:matches)s+=QString::number(p)+" ";return s.trimmed();}())
            .arg(m_compareCount);
    m_running = false;
}

void BruteForceMatch::step() { if (m_stepIndex == 0) run(); }
void BruteForceMatch::reset() { m_steps.clear(); m_stepIndex = 0; m_compareCount = 0; m_resultText.clear(); }
