#include "radixsort.h"
#include <algorithm>

RadixSort::RadixSort(QObject *parent) : AlgorithmBase(parent) {}

void RadixSort::run() {
    reset();
    int n = m_params.value("size", 10).toInt();
    n = qBound(5, n, 20);
    QVector<int> arr(n);
    for (int i = 0; i < n; i++) arr[i] = rand() % 900 + 100; // 3-digit numbers

    addStep(QString("原始数组: [%1]").arg(
        [&](){QString s;for(int v:arr)s+=QString::number(v)+" ";return s.trimmed();}()));
    addStep("===== LSD基数排序 (从低位到高位) =====");

    int maxVal = *std::max_element(arr.begin(), arr.end());

    for (int exp = 1; maxVal / exp > 0; exp *= 10) {
        QVector<QVector<int>> buckets(10);
        addStep(QString("--- 第%1趟(位值=%2) ---").arg(
            [&](){ if(exp==1)return "1(个位)";if(exp==10)return "2(十位)";return "3(百位)";}(),exp));

        // Distribute
        for (int v : arr) {
            int digit = (v / exp) % 10;
            buckets[digit].append(v);
        }
        QString distStr = "分配到桶: ";
        for (int d = 0; d < 10; d++) {
            if (!buckets[d].isEmpty()) {
                distStr += QString("%1:[%2] ").arg(d).arg(
                    [&](){QString s;for(int v:buckets[d])s+=QString::number(v)+",";return s;}());
            }
        }
        addStep(distStr);

        // Collect
        arr.clear();
        for (int d = 0; d < 10; d++)
            for (int v : buckets[d])
                arr.append(v);

        addStep(QString("收集后: [%1]").arg(
            [&](){QString s;for(int v:arr)s+=QString::number(v)+" ";return s.trimmed();}()));
    }

    QVariantMap viz; viz["type"]="barchart"; viz["title"]="基数排序";
    QVariantList vals; for(int v:arr)vals.append(v);
    viz["values"]=vals; m_vizData=viz;

    m_resultText = QString("基数排序完成(%1个元素)").arg(arr.size());
    m_running=false;
}

void RadixSort::step() { if(m_stepIndex==0)run(); }
void RadixSort::reset() { m_steps.clear();m_stepIndex=0;m_resultText.clear();m_vizData.clear(); }
