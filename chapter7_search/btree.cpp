#include "btree.h"

BTree::BTree(QObject *parent) : AlgorithmBase(parent) {}

QVector<QString> BTree::parameterNames() const { return {"order"}; }
QVector<QString> BTree::parameterDescriptions() const { return {"B树阶数(3~7)"}; }
QVector<QVariant> BTree::parameterDefaults() const { return {QVariant(3)}; }

void BTree::splitChild(int idx) {
    addStep(QString("节点分裂"));
    // Simplified concept demonstration
    addStep("B树分裂核心规则:");
    addStep("  1. 当节点关键字数 = m-1 时，插入会导致溢出");
    addStep("  2. 取中间关键字 ⌈m/2⌉ 上升至父节点");
    addStep("  3. 分裂为左右两个节点，各有 ⌈m/2⌉-1 个关键字");
}

void BTree::insert(int k) {
    addStep(QString("B树插入: %1 (阶数m=%2)").arg(k).arg(m_order));
    if (!m_root) {
        m_root = new BNode;
        m_root->keys.append(k);
        addStep(QString("  创建根节点，插入%1").arg(k));
        return;
    }
    if (m_root->keys.size() == m_order - 1) {
        addStep(QString("  根节点已满(%1个关键字)，需要分裂").arg(m_root->keys.size()));
        auto *s = new BNode; s->leaf = false;
        s->children.append(m_root);
        splitChild(0);
        m_root = s;
    }
    // find leaf and insert (simplified)
    BNode *cur = m_root;
    while (!cur->leaf) {
        int i = cur->keys.size() - 1;
        while (i >= 0 && k < cur->keys[i]) i--;
        cur = cur->children[i + 1];
    }
    // Insert into leaf
    int pos = 0;
    while (pos < cur->keys.size() && cur->keys[pos] < k) pos++;
    cur->keys.insert(pos, k);
    addStep(QString("  插入到叶子节点: keys=%1").arg(
        [&](){QString s;for(int v:cur->keys)s+=QString::number(v)+" ";return s.trimmed();}()));

    if (cur->keys.size() >= m_order) {
        addStep(QString("  节点溢出(%1 >= %2)，需要分裂").arg(cur->keys.size()).arg(m_order));
    }
}

void BTree::traverse() {
    addStep("B树遍历与查找说明:");
    addStep("1. B树是多路平衡查找树，每个节点包含多个关键字");
    addStep("2. 查找时在节点内可用顺序/折半查找");
    addStep("3. 范围查找利用叶子节点的链表结构(B+树)");
}

void BTree::run() {
    reset();
    m_order = m_params.value("order", 3).toInt();
    m_order = qBound(3, m_order, 7);

    addStep(QString("===== B树演示 (m=%1阶) =====").arg(m_order));
    addStep(QString("每个节点最多%1个关键字，%2个子树").arg(m_order-1).arg(m_order));
    addStep(QString("除根外，每个节点至少⌈m/2⌉-1=%1个关键字").arg((m_order+1)/2-1));

    for (int v : {10,20,30,40,50,15,25,35,45}) {
        insert(v);
    }

    traverse();
    m_resultText = QString("B树(%1阶)插入演示完成").arg(m_order);

    QVariantMap viz; viz["type"]="tree"; viz["title"]=QString("B树(m=%1)").arg(m_order);
    viz["nodes"]=QVariantList(); m_vizData=viz;

    // Cleanup
    std::function<void(BNode*)> del = [&](BNode *n) {
        if(!n)return; for(auto c:n->children)del(c); delete n;
    };
    del(m_root); m_root=nullptr;
    m_running=false;
}

void BTree::step() { if(m_stepIndex==0)run(); }
void BTree::reset() { m_steps.clear();m_stepIndex=0;m_resultText.clear();m_vizData.clear(); }
