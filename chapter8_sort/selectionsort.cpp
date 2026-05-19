#include "selectionsort.h"
#include <algorithm>

SelectionSort::SelectionSort(QObject *parent) : AlgorithmBase(parent) {}

QVector<QString> SelectionSort::parameterNames() const {
    return {"mode", "order", "size"};
}
QVector<QString> SelectionSort::parameterDescriptions() const {
    return {"排序算法", "数据初始顺序", "数据规模"};
}
QVector<QVariant> SelectionSort::parameterDefaults() const {
    return {QVariant(QVariantList{"简单选择排序", "堆排序"}),
            QVariant(QVariantList{"随机", "正序", "逆序"}),
            QVariant(15)};
}

QVector<int> SelectionSort::generateArray(const QString &order) {
    int n = m_params.value("size", 15).toInt();
    n = qBound(5, n, 50);
    QVector<int> arr(n);
    if (order == "正序") for (int i = 0; i < n; i++) arr[i] = i + 1;
    else if (order == "逆序") for (int i = 0; i < n; i++) arr[i] = n - i;
    else for (int i = 0; i < n; i++) arr[i] = rand() % 100 + 1;
    return arr;
}

void SelectionSort::selectionSort(QVector<int> &arr) {
    int n = arr.size();
    m_comps = m_swaps = 0;
    addStep("===== 简单选择排序 =====");
    recordSnapshot(makeBarchartSnapshot(arr, "简单选择排序-初始"));
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++) {
            m_comps++;
            if (arr[j] < arr[minIdx]) minIdx = j;
        }
        if (minIdx != i) { std::swap(arr[i], arr[minIdx]); m_swaps++; }
        addStep(QString("第%1趟: 选最小arr[%2]=%3, 交换到位置%1").arg(i).arg(minIdx).arg(arr[i]));
        recordSnapshot(makeBarchartSnapshot(arr, QString("选择排序-第%1趟").arg(i+1)));
    }
}

void SelectionSort::heapify(QVector<int> &arr, int n, int i) {
    int largest = i, left = 2 * i + 1, right = 2 * i + 2;
    if (left < n && ++m_comps && arr[left] > arr[largest]) largest = left;
    if (right < n && ++m_comps && arr[right] > arr[largest]) largest = right;
    if (largest != i) {
        std::swap(arr[i], arr[largest]); m_swaps++;
        heapify(arr, n, largest);
    }
}

void SelectionSort::heapSort(QVector<int> &arr) {
    int n = arr.size();
    m_comps = m_swaps = 0;
    addStep("===== 堆排序 =====");
    recordSnapshot(makeBarchartSnapshot(arr, "堆排序-初始"));

    // Build max heap
    addStep("--- 建堆过程 ---");
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
        addStep(QString("调整节点%1后: [%2]").arg(i).arg(
            [&](){QString s;for(int v:arr)s+=QString::number(v)+" ";return s.trimmed();}()));
    }
    recordSnapshot(makeBarchartSnapshot(arr, "堆排序-建堆完成"));

    // Extract elements
    addStep("--- 排序过程 ---");
    for (int i = n - 1; i > 0; i--) {
        std::swap(arr[0], arr[i]); m_swaps++;
        heapify(arr, i, 0);
        addStep(QString("取堆顶%1放到位置%2, 调整后: [%3]").arg(arr[i]).arg(i).arg(
            [&](){QString s;for(int j=0;j<i;j++)s+=QString::number(arr[j])+" ";return s.trimmed();}()));
        recordSnapshot(makeBarchartSnapshot(arr, QString("堆排序-取出第%1大").arg(n-i)));
    }
}

void SelectionSort::run() {
    reset();
    m_mode = m_params.value("mode", "简单选择排序").toString();
    m_order = m_params.value("order", "随机").toString();

    m_arr = generateArray(m_order);
    addStep(QString("原始数组: [%1]").arg(
        [&](){QString s;for(int v:m_arr)s+=QString::number(v)+" ";return s.trimmed();}()));

    if (m_mode == "简单选择排序") selectionSort(m_arr);
    else heapSort(m_arr);

    QVariantMap viz; viz["type"]="barchart"; viz["title"]=QString("%1 排序结果").arg(m_mode);
    QVariantList vals; for(int v:m_arr)vals.append(v);
    viz["values"]=vals; m_vizData=viz;

    addStep(QString("比较: %1, 交换: %2").arg(m_comps).arg(m_swaps));
    m_resultText = QString("%1: 比较%2次, 交换%3次").arg(m_mode).arg(m_comps).arg(m_swaps);
    m_running=false;
}

void SelectionSort::step() { if(m_stepIndex==0)run(); }
void SelectionSort::reset() { m_steps.clear();m_snapshots.clear();m_stepIndex=0;m_comps=m_swaps=0;m_resultText.clear();m_vizData.clear(); }
