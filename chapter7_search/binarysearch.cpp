#include "binarysearch.h"
BinarySearch::BinarySearch(QObject *parent) : AlgorithmBase(parent) {}

void BinarySearch::run() {
    reset();
    QVector<int> arr = {3,7,12,18,22,27,34,39,45,51,58,63,69,75,82,88,94,99};
    int target = m_params.value("target", 34).toInt();

    addStep(QString("折半查找: 在有序数组中查找 %1").arg(target));
    addStep(QString("数组(%1个): [%2]").arg(arr.size()).arg([&](){QString s;for(auto v:arr)s+=QString::number(v)+" ";return s.trimmed();}()));

    int left = 0, right = arr.size() - 1, comps = 0, found = -1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        comps++;
        addStep(QString("区间[%1, %2], mid=%3, arr[%3]=%4").arg(left).arg(right).arg(mid).arg(arr[mid]));
        if (arr[mid] == target) {
            found = mid;
            addStep(QString("  %1 == %2 ✓ 找到!").arg(arr[mid]).arg(target));
            break;
        } else if (arr[mid] < target) {
            addStep(QString("  %1 < %2，到右半部分查找").arg(arr[mid]).arg(target));
            left = mid + 1;
        } else {
            addStep(QString("  %1 > %2，到左半部分查找").arg(arr[mid]).arg(target));
            right = mid - 1;
        }
    }

    m_resultText = found>=0
        ? QString("找到: arr[%1]=%2，比较%3次").arg(found).arg(target).arg(comps)
        : QString("未找到 %1，比较%2次").arg(target).arg(comps);

    QVariantMap viz; viz["type"]="barchart"; viz["title"]="二分查找";
    QVariantList vals, colors;
    for(int i=0;i<arr.size();i++){vals.append(arr[i]);colors.append(i==found?"#FF5722":"#64B5F6");}
    if(left<=right&&found<0){colors[left]="#FF9800";colors[right]="#FF9800";}
    viz["values"]=vals;viz["colors"]=colors;m_vizData=viz;
    m_running=false;
}
void BinarySearch::step() { if(m_stepIndex==0)run(); }
void BinarySearch::reset() { m_steps.clear();m_stepIndex=0;m_resultText.clear();m_vizData.clear(); }
