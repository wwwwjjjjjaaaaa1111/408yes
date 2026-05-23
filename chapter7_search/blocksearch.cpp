#include "blocksearch.h"
#include <algorithm>

BlockSearch::BlockSearch(QObject *parent) : AlgorithmBase(parent) {}

static const QVector<QString> blockPalette = {
    "#E3F2FD", "#FCE4EC", "#E8F5E9", "#FFF3E0",
    "#F3E5F5", "#E0F7FA", "#FFF9C4", "#EFEBE9"
};

QVariantMap BlockSearch::makeBlockSnapshot(const QVector<int> &arr,
    const QVector<QVector<int>> &blocks, int highlightBlock, int highlightIdx,
    bool indexPhase, const QString &title) {
    QVariantMap snap;
    snap["type"] = "barchart";
    snap["title"] = title;
    QVariantList vals, colors;
    int globalIdx = 0;

    for (int b = 0; b < blocks.size(); b++) {
        for (int j = 0; j < blocks[b].size(); j++) {
            vals.append(blocks[b][j]);
            if (indexPhase) {
                // Index phase: color by block
                if (highlightBlock >= 0 && b == highlightBlock)
                    colors.append("#FFAB91");  // current block being checked
                else if (highlightBlock >= 0 && b < highlightBlock)
                    colors.append("#BDBDBD");  // skipped
                else
                    colors.append(blockPalette[b % blockPalette.size()]);
            } else {
                // Within-block search phase
                if (b == highlightBlock && j == highlightIdx)
                    colors.append("#FF9800");  // current comparison
                else if (b == highlightBlock && highlightIdx < 0)
                    colors.append("#64B5F6");  // target block
                else if (b == highlightBlock)
                    colors.append("#64B5F6");  // target block (other elements)
                else
                    colors.append("#BDBDBD");  // non-target blocks
            }
            globalIdx++;
        }
    }
    snap["values"] = vals;
    snap["colors"] = colors;
    snap["highlights"] = QVariantList();
    return snap;
}

void BlockSearch::run() {
    reset();

    QString initialValues = m_params.value("initialValues",
        "3 8 12 15 21 22 27 29 34 39 42 45 51 58 63 67 71 75 82 88").toString();
    int blockSize = m_params.value("blockSize", 5).toInt();
    int target = m_params.value("target", 42).toInt();

    QVector<int> all;
    for (const QString &tok : initialValues.split(' ', Qt::SkipEmptyParts)) {
        bool ok; int v = tok.toInt(&ok);
        if (ok) all.append(v);
    }

    if (all.isEmpty()) {
        addStep("错误: 序列为空");
        m_resultText = "序列为空";
        m_running = false;
        return;
    }

    // Sort and partition into blocks
    std::sort(all.begin(), all.end());
    blockSize = qMax(2, blockSize);
    QVector<QVector<int>> blocks;
    for (int i = 0; i < all.size(); i += blockSize) {
        QVector<int> block;
        for (int j = i; j < qMin(i + blockSize, all.size()); j++)
            block.append(all[j]);
        blocks.append(block);
    }

    // Build index table
    QVector<int> indexMax;
    for (const auto &b : blocks)
        indexMax.append(b.last());

    addStep(QString("分块查找: 目标=%1, 块大小=%2, 共%3块")
            .arg(target).arg(blockSize).arg(blocks.size()));
    addStep("索引表(各块最大值): [" +
        [&](){QString s;for(int v:indexMax)s+=QString::number(v)+" ";return s.trimmed();}() + "]");

    // Initial snapshot: show all blocks in their colors
    recordSnapshot(makeBlockSnapshot(all, blocks, -1, -1, true,
                                     "分块查找 - 初始(各块着色)"));

    // ===== Phase 1: Search index table =====
    addStep("===== 第一步: 查找索引表 =====");
    int blockIdx = -1, comps = 0;

    for (int i = 0; i < indexMax.size(); i++) {
        comps++;
        if (target <= indexMax[i]) {
            blockIdx = i;
            addStep(QString("索引[%1]=%2 >= %3 ✓ 进入第%4块")
                    .arg(i).arg(indexMax[i]).arg(target).arg(i));
            recordSnapshot(makeBlockSnapshot(all, blocks, i, -1, true,
                QString("分块查找 - 定位第%1块").arg(i)));
            break;
        }
        addStep(QString("索引[%1]=%2 < %3 跳过第%4块")
                .arg(i).arg(indexMax[i]).arg(target).arg(i));
        recordSnapshot(makeBlockSnapshot(all, blocks, i, -1, true,
            QString("分块查找 - 跳过第%1块").arg(i)));
    }

    // ===== Phase 2: Search within block =====
    if (blockIdx >= 0) {
        addStep(QString("===== 第二步: 在第%1块内顺序查找 =====").arg(blockIdx));
        addStep(QString("块内元素: [%1]").arg(
            [&](){QString s;for(int v:blocks[blockIdx])s+=QString::number(v)+" ";return s.trimmed();}()));

        for (int j = 0; j < blocks[blockIdx].size(); j++) {
            comps++;
            if (blocks[blockIdx][j] == target) {
                addStep(QString("块内位置%1: %2 == %3 ✓ 找到!")
                        .arg(j).arg(blocks[blockIdx][j]).arg(target));

                // Found: green highlight
                QVariantMap snap = makeBlockSnapshot(all, blocks, blockIdx, j, false,
                    QString("分块查找 - 在第%1块内找到%2!").arg(blockIdx).arg(target));
                QVariantList colors = snap["colors"].toList();
                // Change the found element to green
                int gIdx = 0;
                for (int b = 0; b < blockIdx; b++) gIdx += blocks[b].size();
                gIdx += j;
                colors[gIdx] = "#4CAF50";
                snap["colors"] = colors;
                recordSnapshot(snap);

                m_resultText = QString("找到 %1，在第%2块内。总比较次数: %3\n"
                    "分块查找 = 索引查找 + 块内顺序查找，效率介于顺序和二分之间")
                    .arg(target).arg(blockIdx).arg(comps);
                m_running = false;
                return;
            }
            addStep(QString("块内位置%1: %2 != %3，继续").arg(j).arg(blocks[blockIdx][j]).arg(target));
            recordSnapshot(makeBlockSnapshot(all, blocks, blockIdx, j, false,
                QString("分块查找 - 块内比较位置%1").arg(j)));
        }
    }

    addStep(QString("未找到 %1").arg(target));
    recordSnapshot(makeBlockSnapshot(all, blocks, blockIdx, -1, false,
                                     "分块查找 - 未找到"));
    m_resultText = QString("未找到 %1。比较次数: %2").arg(target).arg(comps);

    // Final viz
    m_vizData = makeBlockSnapshot(all, blocks, blockIdx,
        blockIdx >= 0 ? -1 : -1, false, "分块查找结果");
    m_running = false;
}

void BlockSearch::step() { if (m_stepIndex == 0) run(); }
void BlockSearch::reset() {
    m_steps.clear();
    m_snapshots.clear();
    m_stepIndex = 0;
    m_resultText.clear();
    m_vizData.clear();
}
