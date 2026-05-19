#include "bplustree.h"

BPlusTree::BPlusTree(QObject *parent) : AlgorithmBase(parent) {}

void BPlusTree::run() {
    reset();
    addStep("===== B+树基本概念 =====");
    addStep("");
    addStep("B+树与B树的主要区别:");
    addStep("  1. 非叶子节点只存储索引键值，不存储数据(数据全在叶子)");
    addStep("  2. 所有叶子节点形成有序链表，便于范围查询");
    addStep("  3. 查找必须走到叶子节点（即使内部节点有匹配键）");
    addStep("  4. 非叶子节点的关键字也是对应子树中的最大/最小值");
    addStep("");
    addStep("B+树结构示意 (3阶):");
    addStep("            [20, 40]           ← 内部节点(仅索引)");
    addStep("          /    |    \\");
    addStep("     [10,15] [25,35] [50,60]   ← 内部节点");
    addStep("      / | \\   / | \\   / | \\");
    addStep("     [data]→[data]→[data]→...  ← 叶子节点(含数据+链表)");
    addStep("");
    addStep("B+树优势:");
    addStep("  - 范围查询效率高（叶子链表直接遍历）");
    addStep("  - 所有查找路径等长（都到叶子）");
    addStep("  - 适合文件系统和数据库索引");

    m_resultText = "B+树概念演示完成。核心: 数据只存叶子，叶子有链表。";
    m_running=false;
}

void BPlusTree::step() { if(m_stepIndex==0)run(); }
void BPlusTree::reset() { m_steps.clear();m_stepIndex=0;m_resultText.clear(); }
