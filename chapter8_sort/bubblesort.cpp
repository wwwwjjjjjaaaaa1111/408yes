#include "bubblesort.h"
#include <algorithm>

ExchangeSort::ExchangeSort(QObject *parent) : AlgorithmBase(parent) {}

QVector<QString> ExchangeSort::parameterNames() const {
    return {"mode", "order", "size"};
}
QVector<QString> ExchangeSort::parameterDescriptions() const {
    return {"排序算法", "数据初始顺序", "数据规模"};
}
QVector<QVariant> ExchangeSort::parameterDefaults() const {
    return {QVariant(QVariantList{"冒泡排序", "快速排序"}),
            QVariant(QVariantList{"随机", "正序", "逆序"}),
            QVariant(15)};
}

QVector<int> ExchangeSort::generateArray(const QString &order) {
    int n = m_params.value("size", 15).toInt();
    n = qBound(5, n, 50);
    QVector<int> arr(n);
    if (order == "正序") for (int i = 0; i < n; i++) arr[i] = i + 1;
    else if (order == "逆序") for (int i = 0; i < n; i++) arr[i] = n - i;
    else for (int i = 0; i < n; i++) arr[i] = rand() % 100 + 1;
    return arr;
}

void ExchangeSort::bubbleSort(QVector<int> &arr) {
    int n = arr.size();
    m_comps = m_swaps = 0;
    addStep("===== 冒泡排序 =====");
    recordSnapshot(makeBarchartSnapshot(arr, "冒泡排序-初始"));
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - 1 - i; j++) {
            m_comps++;
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]); m_swaps++; swapped = true;
            }
        }
        recordSnapshot(makeBarchartSnapshot(arr, QString("冒泡-第%1趟").arg(i+1)));
        addStep(QString("第%1趟后: [%2]").arg(i+1).arg(
            [&](){QString s;for(int v:arr)s+=QString::number(v)+" ";return s.trimmed();}()));
        if (!swapped) { addStep("未发生交换，提前终止"); break; }
    }
}

int ExchangeSort::partition(QVector<int> &arr, int low, int high) {
    int pivot = arr[low];
    addStep(QString("分区: 枢轴=%1, 区间[%2,%3]").arg(pivot).arg(low).arg(high));
    while (low < high) {
        while (low < high && arr[high] >= pivot) { m_comps++; high--; }
        arr[low] = arr[high]; m_swaps++;
        while (low < high && arr[low] <= pivot) { m_comps++; low++; }
        arr[high] = arr[low]; m_swaps++;
    }
    arr[low] = pivot; m_swaps++;
    recordSnapshot(makeBarchartSnapshot(arr, QString("快排-枢轴%1归位").arg(pivot), {low}));
    addStep(QString("  枢轴%1归位到%2").arg(pivot).arg(low));
    return low;
}

void ExchangeSort::quickSort(QVector<int> &arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void ExchangeSort::run() {
    reset();
    m_mode = m_params.value("mode", "冒泡排序").toString();
    m_order = m_params.value("order", "随机").toString();

    m_arr = generateArray(m_order);
    addStep(QString("原始数组: [%1]").arg(
        [&](){QString s;for(int v:m_arr)s+=QString::number(v)+" ";return s.trimmed();}()));

    if (m_mode == "冒泡排序") bubbleSort(m_arr);
    else {
        recordSnapshot(makeBarchartSnapshot(m_arr, "快速排序-初始"));
        quickSort(m_arr, 0, m_arr.size() - 1);
    }

    addStep(QString("最终结果: [%1]").arg(
        [&](){QString s;for(int v:m_arr)s+=QString::number(v)+" ";return s.trimmed();}()));
    addStep(QString("比较次数: %1, 交换次数: %2").arg(m_comps).arg(m_swaps));

    QVariantMap viz; viz["type"]="barchart"; viz["title"]=QString("%1 排序结果").arg(m_mode);
    QVariantList vals; for(int v:m_arr)vals.append(v);
    viz["values"]=vals; m_vizData=viz;

    m_resultText = QString("%1: 比较%2次, 交换%3次").arg(m_mode).arg(m_comps).arg(m_swaps);
    m_running=false;
}

void ExchangeSort::step() { if(m_stepIndex==0)run(); }
void ExchangeSort::reset() { m_steps.clear();m_snapshots.clear();m_stepIndex=0;m_comps=m_swaps=0;m_resultText.clear();m_vizData.clear(); }
