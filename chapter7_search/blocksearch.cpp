#include "blocksearch.h"
#include <algorithm>

BlockSearch::BlockSearch(QObject *parent) : AlgorithmBase(parent) {}

void BlockSearch::run() {
    reset();
    // Build sorted blocks
    QVector<QVector<int>> blocks = {
        {3,8,12,15,21},
        {22,27,29,34,39},
        {42,45,51,58,63},
        {67,71,75,82,88}
    };
    int target = m_params.value("target", 42).toInt();
    int blockSize = m_params.value("blockSize", 5).toInt();

    // Index table: max value of each block
    QVector<int> indexMax;
    for(const auto &b:blocks) indexMax.append(b.last());

    addStep(QString("分块查找: 目标=%1, 块大小≈%2").arg(target).arg(blockSize));
    addStep("索引表(各块最大值): [" + [&](){QString s;for(int v:indexMax)s+=QString::number(v)+" ";return s.trimmed();}()+"]");

    // Step 1: search index
    int blockIdx = -1, comps = 0;
    for (int i = 0; i < indexMax.size(); i++) {
        comps++;
        if (target <= indexMax[i]) {
            blockIdx = i;
            addStep(QString("索引查找: 目标%1 ≤ 索引[%2]=%3，进入第%4块").arg(target).arg(i).arg(indexMax[i]).arg(i));
            break;
        }
        addStep(QString("索引查找: 目标%1 > 索引[%2]=%3，跳过").arg(target).arg(i).arg(indexMax[i]));
    }

    // Step 2: sequential search within block
    if (blockIdx >= 0) {
        addStep(QString("在第%1块内顺序查找: [%2]").arg(blockIdx).arg(
            [&](){QString s;for(int v:blocks[blockIdx])s+=QString::number(v)+" ";return s.trimmed();}()));
        for (int j = 0; j < blocks[blockIdx].size(); j++) {
            comps++;
            if (blocks[blockIdx][j] == target) {
                addStep(QString("  块内位置%1: %2 == %3 ✓ 找到!").arg(j).arg(blocks[blockIdx][j]).arg(target));
                m_resultText = QString("找到 %1，在第%2块内。总比较次数: %3").arg(target).arg(blockIdx).arg(comps);
                m_running=false; return;
            }
            addStep(QString("  块内位置%1: %2 != %3").arg(j).arg(blocks[blockIdx][j]).arg(target));
        }
    }
    m_resultText = QString("未找到 %1。比较次数: %2").arg(target).arg(comps);
    m_running=false;
}

void BlockSearch::step() { if(m_stepIndex==0)run(); }
void BlockSearch::reset() { m_steps.clear();m_stepIndex=0;m_resultText.clear(); }
