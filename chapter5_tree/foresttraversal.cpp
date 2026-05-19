#include "foresttraversal.h"

ForestTraversal::ForestTraversal(QObject *parent) : AlgorithmBase(parent) {}

void ForestTraversal::run() {
    reset();
    addStep("===== 树的遍历 =====");
    addStep("");
    addStep("示例树结构:");
    addStep("        A");
    addStep("     /  |  \\");
    addStep("    B   C   D");
    addStep("   / \\     / \\");
    addStep("  E   F   G   H");
    addStep("");
    addStep("1. 先根遍历 (对应转换后二叉树的前序遍历):");
    addStep("   访问顺序: A → B → E → F → C → D → G → H");
    addStep("   规则: 先访问根，再依次先根遍历每棵子树");
    addStep("");
    addStep("2. 后根遍历 (对应转换后二叉树的中序遍历):");
    addStep("   访问顺序: E → F → B → C → G → H → D → A");
    addStep("   规则: 先依次后根遍历每棵子树，再访问根");
    addStep("");
    addStep("===== 森林的遍历 =====");
    addStep("");
    addStep("1. 先序遍历森林 (对每棵树进行先根遍历):");
    addStep("   即: 依次先根遍历森林中的每棵树");
    addStep("");
    addStep("2. 中序遍历森林 (对每棵树进行后根遍历):");
    addStep("   即: 依次后根遍历森林中的每棵树");
    addStep("   对应转换后二叉树的中序遍历");
    addStep("");
    addStep("关键性质: 树的先根遍历 = 对应二叉树的前序遍历");
    addStep("         树的后根遍历 = 对应二叉树的中序遍历");

    m_resultText = "树的先根遍历=二叉树前序，后根遍历=二叉树中序";
    m_running=false;
}

void ForestTraversal::step() { if(m_stepIndex==0)run(); }
void ForestTraversal::reset() { m_steps.clear(); m_stepIndex=0; m_resultText.clear(); }
