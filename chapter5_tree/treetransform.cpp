#include "treetransform.h"

TreeTransform::TreeTransform(QObject *parent) : AlgorithmBase(parent) {}
QVector<QString> TreeTransform::parameterNames() const { return {}; }
QVector<QString> TreeTransform::parameterDescriptions() const { return {}; }
QVector<QVariant> TreeTransform::parameterDefaults() const { return {}; }

void TreeTransform::run() {
    reset();
    addStep("===== 树→二叉树的转换（左孩子右兄弟表示法）=====");
    addStep("规则:");
    addStep("  1. 每个节点的第一个孩子作为其左孩子");
    addStep("  2. 每个节点的兄弟作为其右孩子");
    addStep("");
    addStep("示例: 树结构（A有三个孩子B,C,D；B有两个孩子E,F；C有两个孩子G,H）");
    addStep("");
    addStep("原始树:");
    addStep("        A");
    addStep("     /  |  \\");
    addStep("    B   C   D");
    addStep("   / \\  / \\");
    addStep("  E  F G  H");
    addStep("");
    addStep("转换后的二叉树:");
    addStep("        A");
    addStep("       /");
    addStep("      B");
    addStep("     / \\");
    addStep("    E   C");
    addStep("     \\ / \\");
    addStep("      F G D");
    addStep("       /");
    addStep("      H");
    addStep("");
    addStep("===== 森林→二叉树的转换 =====");
    addStep("规则: 第一棵树作为根，其余树依次作为右子树");
    addStep("");
    addStep("===== 二叉树→树的还原 =====");
    addStep("规则: 左孩子对应原孩子关系，右孩子对应原兄弟关系");

    QVariantMap viz; viz["type"]="tree"; viz["title"]="树→二叉树转换";
    QVariantList nodes;
    auto addN = [&](int id,QString l,int p){ QVariantMap n; n["id"]=id;n["label"]=l;n["parentId"]=p;nodes.append(n); };
    // Show the converted binary tree
    addN(1,"A",-1);addN(2,"B",1);addN(3,"E",2);addN(4,"C",2);addN(5,"F",3);addN(6,"G",4);addN(7,"D",4);addN(8,"H",6);
    viz["nodes"]=nodes; m_vizData=viz;

    m_resultText = QString::fromUtf8("转换演示完成。树→二叉树的核心是「左孩子右兄弟」表示法。");
    m_running=false;
}

void TreeTransform::step() { if(m_stepIndex==0)run(); }
void TreeTransform::reset() { m_steps.clear(); m_stepIndex=0; m_resultText.clear(); m_vizData.clear(); }
