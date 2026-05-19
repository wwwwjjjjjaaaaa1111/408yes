#include "sequentialsearch.h"

SequentialSearch::SequentialSearch(QObject *parent) : AlgorithmBase(parent) {}
QVector<QString> SequentialSearch::parameterNames() const { return {"target"}; }
QVector<QString> SequentialSearch::parameterDescriptions() const { return {"查找目标值"}; }
QVector<QVariant> SequentialSearch::parameterDefaults() const { return {QVariant(37)}; }

void SequentialSearch::run() {
    reset();
    QVector<int> arr = {12, 45, 23, 67, 34, 89, 15, 37, 56, 78, 90, 21, 44, 33, 55};
    int target = m_params.value("target", 37).toInt();

    addStep(QString("顺序查找: 在数组中查找 %1").arg(target));
    addStep(QString("数组: [%1]").arg([&](){QString s;for(auto v:arr)s+=QString::number(v)+" ";return s.trimmed();}()));

    // Standard search
    int idx = -1, comps = 0;
    for (int i = 0; i < arr.size(); i++) {
        comps++;
        if (arr[i] == target) {
            idx = i;
            addStep(QString("  位置%1: %2 == %3 ✓ 找到!").arg(i).arg(arr[i]).arg(target));
            break;
        }
        addStep(QString("  位置%1: %2 != %3，继续...").arg(i).arg(arr[i]).arg(target));
    }

    // Sentinel search
    addStep("--- 哨兵优化版本 ---");
    arr.prepend(target); // sentinel at index 0 (real data starts at 1)
    int i_s = arr.size() - 1;
    int comps2 = 0;
    while (arr[i_s] != target) {
        comps2++;
        addStep(QString("  哨兵-位置%1: %2 != %3").arg(i_s).arg(arr[i_s]).arg(target));
        i_s--;
    }
    comps2++;
    if (i_s > 0) addStep(QString("  哨兵法找到: 位置%1, 比较%2次").arg(i_s-1).arg(comps2));
    else addStep("  哨兵法: 未找到（哨兵位置）");

    m_resultText = QString("普通查找: %1\n哨兵查找: %2\n比较次数: 普通=%3, 哨兵=%4")
        .arg(idx>=0?QString("位置%1").arg(idx):"未找到")
        .arg(i_s>0?QString("位置%1").arg(i_s-1):"未找到")
        .arg(comps).arg(comps2);
    m_running=false;
}

void SequentialSearch::step() { if(m_stepIndex==0)run(); }
void SequentialSearch::reset() { m_steps.clear();m_stepIndex=0;m_resultText.clear(); }
