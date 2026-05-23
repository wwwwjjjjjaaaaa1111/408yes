#include "bplustree.h"

BPlusTree::BPlusTree(QObject *parent) : AlgorithmBase(parent) {}

// ==================== B+ tree core operations ====================

void BPlusTree::splitChild(BPNode *parent, int idx) {
    BPNode *child = parent->children[idx];
    int mid = (m_order - 1) / 2;

    if (child->isLeaf) {
        // Leaf split: COPY middle key up (key stays in leaf)
        BPNode *right = new BPNode;
        right->isLeaf = true;

        // Move keys [mid..end] from child to right (keeping mid in child too)
        // Actually: child keeps [0..mid-1], right gets [mid..end], copy child[mid] up
        for (int i = mid; i < child->keys.size(); i++)
            right->keys.append(child->keys[i]);

        // Maintain leaf linked list
        right->next = child->next;
        child->next = right;

        // Shrink child
        child->keys.resize(mid);

        // Copy the first key of right leaf up (NOT move)
        int copyUpKey = right->keys[0];
        parent->keys.insert(idx, copyUpKey);
        parent->children.insert(idx + 1, right);

        addStep(QString("叶子分裂: 复制关键字 %1 上升至父节点").arg(copyUpKey));
    } else {
        // Internal node split: MOVE middle key up (same as B-tree)
        BPNode *right = new BPNode;
        right->isLeaf = false;

        // Move keys [mid+1..end] to right
        for (int i = mid + 1; i < child->keys.size(); i++)
            right->keys.append(child->keys[i]);

        // Move children [mid+1..end] to right
        for (int i = mid + 1; i < child->children.size(); i++)
            right->children.append(child->children[i]);

        int middleKey = child->keys[mid];

        // Move middle key up to parent
        parent->keys.insert(idx, middleKey);
        parent->children.insert(idx + 1, right);

        // Shrink child
        child->keys.resize(mid);
        child->children.resize(mid + 1);

        addStep(QString("内部节点分裂: 关键字 %1 上升至父节点").arg(middleKey));
    }

    recordSnapshot(buildTreeSnapshot(QString("B+树(m=%1) - 分裂后").arg(m_order)));
}

void BPlusTree::insertNonFull(BPNode *node, int k) {
    if (node->isLeaf) {
        // Insert into leaf in sorted position
        int i = node->keys.size() - 1;
        while (i >= 0 && k < node->keys[i]) i--;
        i++;
        node->keys.insert(i, k);
        addStep(QString("插入 %1 到叶子节点: keys=%2").arg(k)
                .arg([&](){QString s; for(int v:node->keys) s+=QString::number(v)+" "; return s.trimmed();}()));
        recordSnapshot(buildTreeSnapshot(QString("B+树(m=%1) - 插入%2").arg(m_order).arg(k)));
    } else {
        // Find child to descend into
        int i = node->keys.size() - 1;
        while (i >= 0 && k < node->keys[i]) i--;
        i++;

        // If child is full, split first
        if (node->children[i]->keys.size() == m_order - 1) {
            splitChild(node, i);
            if (k > node->keys[i]) i++;
        }
        insertNonFull(node->children[i], k);
    }
}

void BPlusTree::insert(int k) {
    addStep(QString("B+树插入 %1 (阶数 m=%2)").arg(k).arg(m_order));

    if (!m_root) {
        m_root = new BPNode;
        m_root->isLeaf = true;
        m_root->keys.append(k);
        addStep(QString("创建根叶子节点，插入 %1").arg(k));
        recordSnapshot(buildTreeSnapshot(QString("B+树(m=%1) - 插入根%2").arg(m_order).arg(k)));
        return;
    }

    // If root is full, handle specially
    if (m_root->keys.size() == m_order - 1) {
        BPNode *newRoot = new BPNode;
        newRoot->isLeaf = false;
        newRoot->children.append(m_root);
        splitChild(newRoot, 0);
        m_root = newRoot;
    }

    insertNonFull(m_root, k);
}

// ==================== Visualization ====================

void BPlusTree::collectViz(BPNode *node, int &id, int parentId, QVariantList &nodes) {
    if (!node) return;

    QVariantMap nd;
    nd["id"] = id;
    nd["isBTreeNode"] = true;

    QStringList keyStrs;
    for (int v : node->keys)
        keyStrs.append(QString::number(v));
    nd["label"] = keyStrs.join(",");
    QVariantList kl;
    for (const QString &s : keyStrs)
        kl.append(s);
    nd["keys"] = kl;

    nd["parentId"] = parentId;
    // Leaves green (with linked list indicator), internal nodes blue-gray
    nd["color"] = node->isLeaf ? "#C8E6C9" : "#E3F2FD";

    int curId = id;
    nodes.append(nd);

    for (auto *child : node->children) {
        id++;
        collectViz(child, id, curId, nodes);
    }
}

QVariantMap BPlusTree::buildTreeSnapshot(const QString &title) {
    QVariantList nodes;
    int id = 0;
    collectViz(m_root, id, -1, nodes);
    QVariantMap snap;
    snap["type"] = "tree";
    snap["title"] = title;
    snap["nodes"] = nodes;
    return snap;
}

// ==================== run / step / reset ====================

void BPlusTree::run() {
    reset();

    m_order = m_params.value("order", 3).toInt();
    m_order = qBound(3, m_order, 7);

    QString initialValues = m_params.value("initialValues", "10 20 30 40 50 15 25 35 45").toString();

    QVector<int> vals;
    for (const QString &tok : initialValues.split(' ', Qt::SkipEmptyParts)) {
        bool ok; int v = tok.toInt(&ok);
        if (ok) vals.append(v);
    }

    if (vals.isEmpty()) {
        addStep("错误: 初始序列为空或无效");
        m_resultText = "初始序列为空";
        m_running = false;
        return;
    }

    addStep(QString("===== B+树演示 (m=%1阶) =====").arg(m_order));
    addStep("B+树特点: 数据仅在叶子节点，内部节点存储索引");
    addStep("叶子节点形成有序链表，便于范围查询");
    addStep(QString("初始序列: %1").arg(initialValues));

    for (int v : vals)
        insert(v);

    // Show leaf linked list
    BPNode *leaf = m_root;
    while (leaf && !leaf->isLeaf) leaf = leaf->children[0];
    QString leafChain;
    while (leaf) {
        for (int v : leaf->keys) leafChain += QString::number(v) + " ";
        leafChain += "→ ";
        leaf = leaf->next;
    }
    addStep(QString("叶子链表: %1").arg(leafChain));

    m_resultText = QString("B+树(%1阶)构建完成，快照数: %2\n叶子链表: %3")
                   .arg(m_order).arg(m_snapshots.size()).arg(leafChain);

    m_vizData = buildTreeSnapshot(QString("B+树(m=%1)").arg(m_order));

    delete m_root;
    m_root = nullptr;
    m_running = false;
}

void BPlusTree::step() { if (m_stepIndex == 0) run(); }

void BPlusTree::reset() {
    delete m_root;
    m_root = nullptr;
    m_steps.clear();
    m_snapshots.clear();
    m_stepIndex = 0;
    m_resultText.clear();
    m_vizData.clear();
}
