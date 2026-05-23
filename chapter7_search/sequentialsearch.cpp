#include "sequentialsearch.h"

SequentialSearch::SequentialSearch(QObject *parent) : AlgorithmBase(parent) {}

void SequentialSearch::run() {
    reset();

    QString initialValues = m_params.value("initialValues",
        "12 45 23 67 34 89 15 37 56 78 90 21 44 33 55").toString();
    int target = m_params.value("target", 37).toInt();

    QVector<int> arr;
    for (const QString &tok : initialValues.split(' ', Qt::SkipEmptyParts)) {
        bool ok; int v = tok.toInt(&ok);
        if (ok) arr.append(v);
    }

    if (arr.isEmpty()) {
        addStep("错误: 数组为空或无效");
        m_resultText = "数组为空";
        m_running = false;
        return;
    }

    addStep(QString("顺序查找: 在数组中查找 %1").arg(target));
    addStep(QString("数组(%1个): [%2]").arg(arr.size()).arg(
        [&](){QString s;for(int v:arr)s+=QString::number(v)+" ";return s.trimmed();}()));

    // Initial snapshot
    recordSnapshot(makeBarchartSnapshot(arr, "顺序查找 - 初始数组", {}));

    // Standard sequential search with snapshots
    int idx = -1, comps = 0;
    for (int i = 0; i < arr.size(); i++) {
        comps++;
        QVector<int> hl = {i};
        if (arr[i] == target) {
            idx = i;
            addStep(QString("[标准] 位置%1: %2 == %3 ✓ 找到!").arg(i).arg(arr[i]).arg(target));
            // Blue for found
            QVariantMap snap = makeBarchartSnapshot(arr, QString("顺序查找 - 位置%1找到!").arg(i), {});
            QVariantList colors;
            for (int j = 0; j < arr.size(); j++)
                colors.append(j == i ? "#2196F3" : "#4CAF50");
            snap["colors"] = colors;
            recordSnapshot(snap);
            break;
        }
        addStep(QString("[标准] 位置%1: %2 != %3，继续...").arg(i).arg(arr[i]).arg(target));
        QVariantMap snap = makeBarchartSnapshot(arr, QString("顺序查找 - 比较位置%1").arg(i), hl);
        recordSnapshot(snap);
    }

    if (idx < 0) {
        addStep(QString("[标准] 未找到 %1，遍历完整个数组").arg(target));
        recordSnapshot(makeBarchartSnapshot(arr, "顺序查找 - 未找到", {}));
    }

    // Sentinel search (text-only, no extra snapshots)
    addStep("");
    addStep("--- 哨兵优化版本（仅文字说明）---");
    QVector<int> sArr = arr;
    sArr.prepend(target);
    int i_s = sArr.size() - 1;
    int comps2 = 0;
    while (sArr[i_s] != target) {
        comps2++;
        i_s--;
    }
    comps2++;
    if (i_s > 0) {
        addStep(QString("哨兵法: 在位置%1找到(比较%2次)，省略了越界检查").arg(i_s - 1).arg(comps2));
    } else {
        addStep("哨兵法: 未找到（命中哨兵位置）");
    }

    m_resultText = QString("标准查找: %1 (比较%2次)\n哨兵查找: %3 (比较%4次)")
        .arg(idx >= 0 ? QString("位置%1").arg(idx) : "未找到").arg(comps)
        .arg(i_s > 0 ? QString("位置%1").arg(i_s - 1) : "未找到").arg(comps2);

    m_vizData = makeBarchartSnapshot(arr, "顺序查找结果", {});
    m_running = false;
}

void SequentialSearch::step() { if (m_stepIndex == 0) run(); }
void SequentialSearch::reset() {
    m_steps.clear();
    m_snapshots.clear();
    m_stepIndex = 0;
    m_resultText.clear();
    m_vizData.clear();
}
