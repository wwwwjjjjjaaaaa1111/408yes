#include "mergesort.h"

MergeSort::MergeSort(QObject *parent) : AlgorithmBase(parent) {}

QVector<QString> MergeSort::parameterNames() const { return {"order","size"}; }
QVector<QString> MergeSort::parameterDescriptions() const { return {"数据初始顺序", "数据规模"}; }
QVector<QVariant> MergeSort::parameterDefaults() const {
    return {QVariant(QVariantList{"随机","正序","逆序"}), QVariant(12)};
}

QVector<int> MergeSort::generateArray(const QString &order) {
    int n = m_params.value("size", 12).toInt(); n = qBound(4, n, 50);
    QVector<int> arr(n);
    if (order == "正序") for (int i = 0; i < n; i++) arr[i] = i + 1;
    else if (order == "逆序") for (int i = 0; i < n; i++) arr[i] = n - i;
    else for (int i = 0; i < n; i++) arr[i] = rand() % 100 + 1;
    return arr;
}

void MergeSort::merge(QVector<int> &arr, int left, int mid, int right) {
    QVector<int> tmp(right - left + 1);
    int i = left, j = mid + 1, k = 0;
    while (i <= mid && j <= right) {
        m_comps++;
        tmp[k++] = (arr[i] <= arr[j]) ? arr[i++] : arr[j++];
    }
    while (i <= mid) tmp[k++] = arr[i++];
    while (j <= right) tmp[k++] = arr[j++];
    for (int p = 0; p < k; p++) arr[left + p] = tmp[p];
    addStep(QString("合并区间[%1,%2]和[%3,%4] → [%5]")
            .arg(left).arg(mid).arg(mid+1).arg(right).arg(
        [&](){QString s;for(int p=left;p<=right;p++)s+=QString::number(arr[p])+" ";return s.trimmed();}()));
}

void MergeSort::mergeSort(QVector<int> &arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        addStep(QString("分解: [%1,%2] → [%1,%3] + [%4,%2]").arg(left).arg(right).arg(mid).arg(mid+1));
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
        recordSnapshot(makeBarchartSnapshot(arr, QString("归并-合并[%1,%2]").arg(left).arg(right)));
    }
}

void MergeSort::run() {
    reset();
    m_order = m_params.value("order", "随机").toString();
    m_arr = generateArray(m_order);
    m_comps = 0;

    addStep(QString("原始数组(%1个): [%2]").arg(m_arr.size()).arg(
        [&](){QString s;for(int v:m_arr)s+=QString::number(v)+" ";return s.trimmed();}()));
    recordSnapshot(makeBarchartSnapshot(m_arr, "归并排序-初始"));

    mergeSort(m_arr, 0, m_arr.size() - 1);

    QVariantMap viz; viz["type"]="barchart"; viz["title"]="归并排序";
    QVariantList vals; for(int v:m_arr)vals.append(v);
    viz["values"]=vals; m_vizData=viz;

    m_resultText = QString("归并排序: 比较%1次, 数据量%2").arg(m_comps).arg(m_arr.size());
    m_running=false;
}

void MergeSort::step() { if(m_stepIndex==0)run(); }
void MergeSort::reset() { m_steps.clear();m_snapshots.clear();m_stepIndex=0;m_comps=0;m_resultText.clear();m_vizData.clear(); }
