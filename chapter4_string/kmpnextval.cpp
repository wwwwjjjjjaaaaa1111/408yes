#include "kmpnextval.h"

KMPNextVal::KMPNextVal(QObject *parent) : AlgorithmBase(parent) {}

QVector<QString> KMPNextVal::parameterNames() const {
    return {"pattern"};
}
QVector<QString> KMPNextVal::parameterDescriptions() const {
    return {"模式串(演示next和nextval数组生成)"};
}
QVector<QVariant> KMPNextVal::parameterDefaults() const {
    return {QString("aaaab")};
}

void KMPNextVal::run() {
    reset();
    m_pattern = m_params.value("pattern", "aaaab").toString();
    int m = m_pattern.size();

    addStep(QString("模式串: %1，演示next和nextval数组生成").arg(m_pattern));

    // next数组
    QVector<int> next(m, 0);
    next[0] = -1;
    addStep("===== 第1步：计算next数组 =====");
    int i = 0, j = -1;
    while (i < m - 1) {
        if (j == -1 || m_pattern[i] == m_pattern[j]) {
            i++; j++;
            next[i] = j;
            addStep(QString("next[%1] = %2").arg(i).arg(j));
        } else {
            j = next[j];
        }
    }

    QString nextStr;
    for (int k = 0; k < m; k++) nextStr += QString::number(next[k]) + " ";
    addStep(QString("next数组: [%1]").arg(nextStr.trimmed()));

    // nextval数组
    QVector<int> nextval(m, 0);
    nextval[0] = -1;
    addStep("===== 第2步：计算nextval数组 =====");
    for (int k = 1; k < m; k++) {
        if (m_pattern[k] == m_pattern[next[k]]) {
            nextval[k] = nextval[next[k]];
            addStep(QString("P[%1]='%2' == P[next[%3]]=P[%4]='%5'，nextval[%6]=nextval[%7]=%8")
                    .arg(k).arg(m_pattern[k]).arg(k).arg(next[k])
                    .arg(m_pattern[next[k]]).arg(k).arg(next[k]).arg(nextval[k]));
        } else {
            nextval[k] = next[k];
            addStep(QString("P[%1]='%2' != P[next[%3]]=P[%4]='%5'，nextval[%6]=next[%7]=%8")
                    .arg(k).arg(m_pattern[k]).arg(k).arg(next[k])
                    .arg(m_pattern[next[k]]).arg(k).arg(k).arg(next[k]));
        }
    }

    QString nvStr;
    for (int k = 0; k < m; k++) nvStr += QString::number(nextval[k]) + " ";
    addStep(QString("nextval数组: [%1]").arg(nvStr.trimmed()));

    m_resultText = QString("模式串: %1\nnext数组: [%2]\nnextval数组: [%3]")
        .arg(m_pattern).arg(nextStr.trimmed()).arg(nvStr.trimmed());

    QVariantMap vizData;
    vizData["type"] = "barchart";
    vizData["title"] = QString("next vs nextval 对比 (模式串: %1)").arg(m_pattern);
    QVariantList vals, colors;
    for (int k = 0; k < m; k++) {
        vals.append(next[k] + 1); // offset for display
        colors.append("#64B5F6");
    }
    for (int k = 0; k < m; k++) {
        vals.append(nextval[k] + 1);
        colors.append("#FF9800");
    }
    vizData["values"] = vals;
    vizData["colors"] = colors;
    m_vizData = vizData;

    m_running = false;
}

void KMPNextVal::step() { if (m_stepIndex == 0) run(); }
void KMPNextVal::reset() { m_steps.clear(); m_stepIndex = 0; m_resultText.clear(); m_vizData.clear(); }
