#include "countingsort.h"
#include <algorithm>

CountingSort::CountingSort(QObject *parent) : AlgorithmBase(parent) {}

void CountingSort::run() {
    reset();
    int n = m_params.value("size", 15).toInt();
    n = qBound(5, n, 30);
    QVector<int> arr(n);
    for (int i = 0; i < n; i++) arr[i] = rand() % 50 + 1;

    addStep(QString("原始数组: [%1]").arg(
        [&](){QString s;for(int v:arr)s+=QString::number(v)+" ";return s.trimmed();}()));
    addStep("===== 计数排序 =====");

    int minVal = *std::min_element(arr.begin(), arr.end());
    int maxVal = *std::max_element(arr.begin(), arr.end());
    int range = maxVal - minVal + 1;

    addStep(QString("范围: [%1, %2], 计数数组大小=%3").arg(minVal).arg(maxVal).arg(range));

    QVector<int> count(range, 0);
    for (int v : arr) count[v - minVal]++;

    addStep(QString("计数数组: [%1]").arg(
        [&](){QString s;for(int c:count)s+=QString::number(c)+" ";return s.trimmed();}()));

    // Accumulate
    for (int i = 1; i < range; i++) count[i] += count[i - 1];
    addStep(QString("累加计数: [%1]").arg(
        [&](){QString s;for(int c:count)s+=QString::number(c)+" ";return s.trimmed();}()));

    // Build output
    QVector<int> output(n);
    for (int i = n - 1; i >= 0; i--) {
        output[count[arr[i] - minVal] - 1] = arr[i];
        count[arr[i] - minVal]--;
    }

    addStep(QString("排序结果: [%1]").arg(
        [&](){QString s;for(int v:output)s+=QString::number(v)+" ";return s.trimmed();}()));

    QVariantMap viz; viz["type"]="barchart"; viz["title"]="计数排序";
    QVariantList vals; for(int v:output)vals.append(v);
    viz["values"]=vals; m_vizData=viz;

    m_resultText = QString("计数排序完成(%1个元素, 范围%2~%3)").arg(n).arg(minVal).arg(maxVal);
    m_running=false;
}

void CountingSort::step() { if(m_stepIndex==0)run(); }
void CountingSort::reset() { m_steps.clear();m_stepIndex=0;m_resultText.clear();m_vizData.clear(); }
