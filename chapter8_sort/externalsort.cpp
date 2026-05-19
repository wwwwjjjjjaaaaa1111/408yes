#include "externalsort.h"
#include <QColor>

ExternalSort::ExternalSort(QObject *parent) : AlgorithmBase(parent) {}

void ExternalSort::demoMultiwayMerge() {
    addStep("===== 多路平衡归并 =====");
    addStep("");
    addStep("原理: 将大文件分成多个初始归并段，然后多路归并");
    addStep("");
    addStep("示例 (5路归并，每路已排序):");
    addStep("  路0: [8, 23, 45, 67]");
    addStep("  路1: [3, 17, 34, 56]");
    addStep("  路2: [1, 29, 42, 88]");
    addStep("  路3: [12, 31, 55, 73]");
    addStep("  路4: [5, 19, 38, 62]");
    addStep("");
    addStep("比较-选择过程:");
    addStep("  取各路段最小值比较: min(8,3,1,12,5)=1(路2) → 输出1");
    addStep("  路2下一个值29参与: min(8,3,29,12,5)=3(路1) → 输出3");
    addStep("  路1下一个值17参与: min(8,17,29,12,5)=5(路4) → 输出5");
    addStep("  ... 依此类推，直到所有数据归并完成");
    addStep("");
    addStep("关键: 增大归并路数k可以减少归并趟数，但需要使用败者树");

    // Visual: set up the merge concept
    QVariantMap viz; viz["type"]="barchart"; viz["title"]="多路平衡归并";
    QVariantList vals;
    // Show each run as grouped bars
    QVector<int> runs = {8,23,45,67, 3,17,34,56, 1,29,42,88, 12,31,55,73, 5,19,38,62};
    for(int v:runs)vals.append(v);
    viz["values"]=vals;
    QVariantList colors;
    for(int i=0;i<runs.size();i++)colors.append(QColor::fromHsv((i/4)*72,200,200).name());
    viz["colors"]=colors; m_vizData=viz;

    m_resultText = "多路平衡归并: 增大归并路数k → 减少归并趟数 → 减少I/O次数";
}

void ExternalSort::demoLoserTree() {
    addStep("===== 败者树 =====");
    addStep("");
    addStep("败者树是一种用于多路归并的数据结构");
    addStep("树的叶子节点对应各归并段当前最小值");
    addStep(QString::fromUtf8("内部节点记录「败者」(即较大的值)"));
    addStep("树根记录最小值来自哪一路");
    addStep("");
    addStep("以4路归并为例，败者树结构:");
    addStep("        winner=2(最小来自路2)");
    addStep("           /    \\");
    addStep("    loser=1   loser=0");
    addStep("      /  \\      /  \\");
    addStep("   路0:8 路1:3 路2:1 路3:12  ← 叶子(各路段当前值)");
    addStep("");
    addStep("优势: 每次选择最小只需O(log k)次比较，而非k-1次");

    QVariantMap viz; viz["type"]="tree"; viz["title"]="败者树(4路归并)";
    QVariantList nodes;
    auto addN = [&](int id,QString l,int p){QVariantMap n;n["id"]=id;n["label"]=l;n["parentId"]=p;nodes.append(n);};
    addN(0,"winner=2",-1); addN(1,"los=1",0); addN(2,"los=0",0);
    addN(3,"R0:8",1); addN(4,"R1:3",1); addN(5,"R2:1",2); addN(6,"R3:12",2);
    viz["nodes"]=nodes; m_vizData=viz;

    m_resultText = "败者树: 每次选最小值只需O(log k)比较，显著提升多路归并效率";
}

void ExternalSort::demoReplacementSelection() {
    addStep("===== 置换-选择排序 =====");
    addStep("");
    addStep("用于生成初始归并段，可生成长度不等的归并段");
    addStep("平均段长为2×工作区大小");
    addStep("");
    addStep("算法步骤:");
    addStep("  1. 读入W个记录到工作区(WA)");
    addStep("  2. 选WA中最小值输出 → 形成归并段");
    addStep("  3. 读入新记录填补空位");
    addStep("    - 若新记录≥刚输出记录: 仍属当前归并段");
    addStep("    - 若新记录<刚输出记录: 暂不能输出，属于下一段");
    addStep("  4. 当WA中所有记录都属于下一段时，开始新段");
    addStep("");
    addStep("示例(WA大小=3, 输入: 21,11,09,12,33,57,44,27,16,08):");
    addStep("  读入 21,11,09 → WA={9,11,21}");
    addStep("  输出09，读入12 → WA={11,12,21}");
    addStep("  输出11，读入33 → WA={12,21,33}");
    addStep("  ... 最终生成归并段: 09,11,12,21,27,33,44,57 | 08,16");
    addStep("  (第1段8个, 第2段2个)");

    m_resultText = "置换-选择排序: 工作区W=3, 生成2个归并段\n段1(8个): 09~57, 段2(2个): 08,16";
}

void ExternalSort::demoOptimalMergeTree() {
    addStep("===== 最佳归并树 =====");
    addStep("");
    addStep("当各归并段长度不等时，归并顺序不同，总I/O次数也不同");
    addStep("最佳归并树就是哈夫曼树在外部排序中的应用");
    addStep("");
    addStep("示例: 有5个归并段，长度分别为 2,3,6,9,12,15,18 (单位:块)");
    addStep("");
    addStep("按哈夫曼算法构造最佳3路归并树:");
    addStep("  需添加虚段(长度为0)使满足(n₀-1)%(k-1)=0");
    addStep("  n₀=7, k=3 → (7-1)%2=0, 不需要虚段");
    addStep("");
    addStep("最佳归并树合并序列:");
    addStep("  1. 合并 2+3+6 = 11 (I/O: 11)");
    addStep("  2. 合并 9+11+12 = 32 (I/O: 32)");
    addStep("  3. 合并 15+18+32 = 65 (I/O: 65)");
    addStep("  总I/O次数 = 11+32+65 = 108 块");

    m_resultText = "最佳归并树: 总I/O次数 = 108块\nWPL = 2×3+3×3+6×3+9×2+12×2+15×1+18×1 = 108";
}

void ExternalSort::run() {
    reset();
    m_mode = m_params.value("mode", "多路平衡归并").toString();

    if (m_mode == "多路平衡归并") demoMultiwayMerge();
    else if (m_mode == "败者树") demoLoserTree();
    else if (m_mode == "置换-选择排序") demoReplacementSelection();
    else demoOptimalMergeTree();

    m_running=false;
}

void ExternalSort::step() { if(m_stepIndex==0)run(); }
void ExternalSort::reset() { m_steps.clear();m_stepIndex=0;m_resultText.clear();m_vizData.clear(); }
