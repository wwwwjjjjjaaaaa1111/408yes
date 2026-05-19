#include "insertionsort.h"
#include <algorithm>

InsertionSort::InsertionSort(QObject *parent) : AlgorithmBase(parent) {}

QVector<QString> InsertionSort::parameterNames() const {
    return {"mode", "order", "size"};
}
QVector<QString> InsertionSort::parameterDescriptions() const {
    return {"排序算法", "数据初始顺序", "数据规模"};
}
QVector<QVariant> InsertionSort::parameterDefaults() const {
    return {QVariant(QVariantList{"直接插入", "折半插入", "希尔排序"}),
            QVariant(QVariantList{"随机", "正序", "逆序"}),
            QVariant(15)};
}

QVector<int> InsertionSort::generateArray(const QString &order) {
    int n = m_params.value("size", 15).toInt();
    n = qBound(5, n, 50);
    QVector<int> arr(n);
    if (order == "正序") for (int i = 0; i < n; i++) arr[i] = i + 1;
    else if (order == "逆序") for (int i = 0; i < n; i++) arr[i] = n - i;
    else for (int i = 0; i < n; i++) arr[i] = rand() % 100 + 1;
    return arr;
}

void InsertionSort::directInsertion(QVector<int> &arr) {
    int n = arr.size();
    m_comps = m_swaps = 0;
    addStep("===== 直接插入排序 =====");
    recordSnapshot(makeBarchartSnapshot(arr, "直接插入排序-初始"));
    for (int i = 1; i < n; i++) {
        int key = arr[i], j = i - 1;
        addStep(QString("第%1趟: 暂存key=%2").arg(i).arg(key));
        while (j >= 0 && ++m_comps && arr[j] > key) {
            arr[j + 1] = arr[j];
            m_swaps++;
            j--;
        }
        arr[j + 1] = key;
        m_swaps++;
        addStep(QString("  将%1插入到位置%2").arg(key).arg(j+1));
        recordSnapshot(makeBarchartSnapshot(arr, QString("直接插入-第%1趟").arg(i)));
    }
}

void InsertionSort::binaryInsertion(QVector<int> &arr) {
    int n = arr.size();
    m_comps = m_swaps = 0;
    addStep("===== 折半插入排序 =====");
    recordSnapshot(makeBarchartSnapshot(arr, "折半插入排序-初始"));
    for (int i = 1; i < n; i++) {
        int key = arr[i], left = 0, right = i - 1;
        while (left <= right) {
            int mid = (left + right) / 2;
            m_comps++;
            if (arr[mid] > key) right = mid - 1;
            else left = mid + 1;
        }
        for (int j = i - 1; j >= left; j--) { arr[j + 1] = arr[j]; m_swaps++; }
        arr[left] = key;
        addStep(QString("第%1趟: key=%2, 折半确定位置=%3").arg(i).arg(key).arg(left));
        recordSnapshot(makeBarchartSnapshot(arr, QString("折半插入-第%1趟").arg(i)));
    }
}

void InsertionSort::shellSort(QVector<int> &arr) {
    int n = arr.size();
    m_comps = m_swaps = 0;
    addStep("===== 希尔排序 =====");
    recordSnapshot(makeBarchartSnapshot(arr, "希尔排序-初始"));
    for (int gap = n / 2; gap > 0; gap /= 2) {
        addStep(QString("增量gap=%1:").arg(gap));
        for (int i = gap; i < n; i++) {
            int key = arr[i], j = i;
            while (j >= gap && ++m_comps && arr[j - gap] > key) {
                arr[j] = arr[j - gap]; m_swaps++; j -= gap;
            }
            arr[j] = key; m_swaps++;
        }
        recordSnapshot(makeBarchartSnapshot(arr, QString("希尔排序-gap=%1").arg(gap)));
    }
}

void InsertionSort::run() {
    reset();
    m_mode = m_params.value("mode", "直接插入").toString();
    m_order = m_params.value("order", "随机").toString();

    m_arr = generateArray(m_order);
    QString arrStr;
    for (auto v : m_arr) arrStr += QString::number(v) + " ";
    addStep(QString("原始数组(%1, %2个): %3").arg(m_order).arg(m_arr.size()).arg(arrStr.trimmed()));

    if (m_mode == "直接插入") directInsertion(m_arr);
    else if (m_mode == "折半插入") binaryInsertion(m_arr);
    else shellSort(m_arr);

    addStep(QString("比较次数: %1, 移动次数: %2").arg(m_comps).arg(m_swaps));

    QVariantMap viz; viz["type"]="barchart"; viz["title"]=QString("%1 排序结果").arg(m_mode);
    QVariantList vals, colors;
    for (int v : m_arr) { vals.append(v); colors.append("#4CAF50"); }
    viz["values"]=vals; viz["colors"]=colors; m_vizData=viz;

    m_resultText = QString("%1(%2, %3个): 比较%4次, 移动%5次")
        .arg(m_mode).arg(m_order).arg(m_arr.size()).arg(m_comps).arg(m_swaps);
    m_running=false;
}

void InsertionSort::step() { if(m_stepIndex==0)run(); }
void InsertionSort::reset() { m_steps.clear();m_snapshots.clear();m_stepIndex=0;m_comps=m_swaps=0;m_resultText.clear();m_vizData.clear(); }
