#include "binarysearch.h"
#include <algorithm>

BinarySearch::BinarySearch(QObject *parent) : AlgorithmBase(parent) {}

QVariantMap BinarySearch::makeIntervalSnapshot(const QVector<int> &arr,
    int left, int right, int mid, bool found, const QString &title) {
    QVariantMap snap;
    snap["type"] = "barchart";
    snap["title"] = title;
    QVariantList vals, colors;
    for (int i = 0; i < arr.size(); i++) {
        vals.append(arr[i]);
        if (i == mid && found)
            colors.append("#4CAF50");       // green = found
        else if (i == mid)
            colors.append("#FF9800");       // orange = mid
        else if (i >= left && i <= right)
            colors.append("#64B5F6");       // blue = in search interval
        else
            colors.append("#BDBDBD");       // grey = excluded
    }
    snap["values"] = vals;
    snap["colors"] = colors;
    snap["highlights"] = QVariantList();
    return snap;
}

void BinarySearch::run() {
    reset();

    QString initialValues = m_params.value("initialValues",
        "3 7 12 18 22 27 34 39 45 51 58 63 69 75 82 88 94 99").toString();
    int target = m_params.value("target", 34).toInt();

    QVector<int> arr;
    for (const QString &tok : initialValues.split(' ', Qt::SkipEmptyParts)) {
        bool ok; int v = tok.toInt(&ok);
        if (ok) arr.append(v);
    }

    if (arr.isEmpty()) {
        addStep("错误: 序列为空或无效");
        m_resultText = "序列为空";
        m_running = false;
        return;
    }

    // Ensure sorted (binary search requires sorted array)
    std::sort(arr.begin(), arr.end());

    addStep(QString("折半查找: 在有序数组中查找 %1").arg(target));
    addStep(QString("有序数组(%1个): [%2]").arg(arr.size()).arg(
        [&](){QString s;for(int v:arr)s+=QString::number(v)+" ";return s.trimmed();}()));

    // Initial snapshot: all blue (entire array is the search interval)
    recordSnapshot(makeIntervalSnapshot(arr, 0, arr.size() - 1, -1, false,
                                        "折半查找 - 初始"));

    int left = 0, right = arr.size() - 1, comps = 0, found = -1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        comps++;

        addStep(QString("区间[%1, %2], mid=%3, arr[%3]=%4")
                .arg(left).arg(right).arg(mid).arg(arr[mid]));

        if (arr[mid] == target) {
            found = mid;
            addStep(QString("  %1 == %2 ✓ 找到!").arg(arr[mid]).arg(target));
            recordSnapshot(makeIntervalSnapshot(arr, left, right, mid, true,
                QString("折半查找 - 在mid=%1找到!").arg(mid)));
            break;
        } else if (arr[mid] < target) {
            addStep(QString("  %1 < %2，去右半部分 [%3, %4]")
                    .arg(arr[mid]).arg(target).arg(mid + 1).arg(right));
            // Snapshot: show current comparison, then narrow
            recordSnapshot(makeIntervalSnapshot(arr, left, right, mid, false,
                QString("折半查找 - mid=%1 < %2, 向右").arg(mid).arg(target)));
            left = mid + 1;
        } else {
            addStep(QString("  %1 > %2，去左半部分 [%3, %4]")
                    .arg(arr[mid]).arg(target).arg(left).arg(mid - 1));
            recordSnapshot(makeIntervalSnapshot(arr, left, right, mid, false,
                QString("折半查找 - mid=%1 > %2, 向左").arg(mid).arg(target)));
            right = mid - 1;
        }
    }

    if (found < 0) {
        addStep(QString("未找到 %1，区间为空").arg(target));
        recordSnapshot(makeIntervalSnapshot(arr, 0, arr.size() - 1, -1, false,
                                            "折半查找 - 未找到"));
    }

    m_resultText = found >= 0
        ? QString("找到: arr[%1]=%2，比较%3次\n区间每次缩小一半，时间复杂度O(log n)")
          .arg(found).arg(target).arg(comps)
        : QString("未找到 %1，比较%2次").arg(target).arg(comps);

    // Final viz
    QVariantMap viz = makeIntervalSnapshot(arr, 0, arr.size() - 1,
        found >= 0 ? found : -1, found >= 0, "折半查找结果");
    m_vizData = viz;
    m_running = false;
}

void BinarySearch::step() { if (m_stepIndex == 0) run(); }
void BinarySearch::reset() {
    m_steps.clear();
    m_snapshots.clear();
    m_stepIndex = 0;
    m_resultText.clear();
    m_vizData.clear();
}
