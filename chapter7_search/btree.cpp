#include "btree.h"

BTree::BTree(QObject *parent) : AlgorithmBase(parent) {}

// ==================== B-tree core operations ====================

void BTree::splitChild(BNode *parent, int idx) {
    BNode *child = parent->children[idx];
    int mid = (m_order - 1) / 2;  // index of middle key (0-based)

    BNode *right = new BNode;
    right->leaf = child->leaf;

    // Move keys [mid+1..end] to right
    for (int i = mid + 1; i < child->keys.size(); i++)
        right->keys.append(child->keys[i]);

    // Move children [mid+1..end] to right (for internal nodes)
    if (!child->leaf) {
        for (int i = mid + 1; i < child->children.size(); i++)
            right->children.append(child->children[i]);
        child->children.resize(mid + 1);
    }

    int middleKey = child->keys[mid];

    // Insert middle key into parent
    parent->keys.insert(idx, middleKey);

    // Insert right as new child after idx
    parent->children.insert(idx + 1, right);

    // Shrink child: keep only keys [0..mid-1]
    child->keys.resize(mid);

    addStep(QString("分裂节点: 中间关键字 %1 上升至父节点，分为左右两个节点")
            .arg(middleKey));
    recordSnapshot(buildTreeSnapshot(QString("B树(m=%1) - 分裂后").arg(m_order)));
}

void BTree::insertNonFull(BNode *node, int k) {
    int i = node->keys.size() - 1;

    if (node->leaf) {
        // Insert into leaf
        while (i >= 0 && k < node->keys[i]) i--;
        i++;
        node->keys.insert(i, k);
        addStep(QString("插入 %1 到叶子节点: keys=%2").arg(k)
                .arg([&](){QString s; for(int v:node->keys) s+=QString::number(v)+" "; return s.trimmed();}()));
        recordSnapshot(buildTreeSnapshot(QString("B树(m=%1) - 插入%2").arg(m_order).arg(k)));
    } else {
        // Find child to descend into
        while (i >= 0 && k < node->keys[i]) i--;
        i++;

        // If child is full, split first
        if (node->children[i]->keys.size() == m_order - 1) {
            splitChild(node, i);
            // After split, determine which side to go
            if (k > node->keys[i]) i++;
        }
        insertNonFull(node->children[i], k);
    }
}

void BTree::insert(int k) {
    addStep(QString("B树插入 %1 (阶数 m=%2, 最大%3关键字/节点)")
            .arg(k).arg(m_order).arg(m_order - 1));

    if (!m_root) {
        m_root = new BNode;
        m_root->keys.append(k);
        addStep(QString("创建根节点，插入 %1").arg(k));
        recordSnapshot(buildTreeSnapshot(QString("B树(m=%1) - 插入根%2").arg(m_order).arg(k)));
        return;
    }

    // If root is full, split it first
    if (m_root->keys.size() == m_order - 1) {
        BNode *newRoot = new BNode;
        newRoot->leaf = false;
        newRoot->children.append(m_root);
        splitChild(newRoot, 0);
        m_root = newRoot;
    }

    insertNonFull(m_root, k);
}

// ==================== Visualization ====================

void BTree::collectViz(BNode *node, int &id, int parentId,
                        QVariantList &nodes, const QString &colorFilter) {
    if (!node) return;

    QVariantMap nd;
    nd["id"] = id;
    nd["isBTreeNode"] = true;

    // Build label and keys list
    QStringList keyStrs;
    for (int v : node->keys)
        keyStrs.append(QString::number(v));
    nd["label"] = keyStrs.join(",");
    QVariantList kl;
    for (const QString &s : keyStrs)
        kl.append(s);
    nd["keys"] = kl;

    nd["parentId"] = parentId;
    nd["color"] = node->leaf ? "#C8E6C9" : "#E3F2FD";

    int curId = id;
    nodes.append(nd);

    for (auto *child : node->children) {
        id++;
        collectViz(child, id, curId, nodes, colorFilter);
    }
}

QVariantMap BTree::buildTreeSnapshot(const QString &title) {
    QVariantList nodes;
    int id = 0;
    collectViz(m_root, id, -1, nodes, "");
    QVariantMap snap;
    snap["type"] = "tree";
    snap["title"] = title;
    snap["nodes"] = nodes;
    return snap;
}

// ==================== run / step / reset ====================

void BTree::run() {
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

    addStep(QString("===== B树演示 (m=%1阶) =====").arg(m_order));
    addStep(QString("每个节点最多 %1 个关键字，%2 个子树").arg(m_order - 1).arg(m_order));
    int minKeys = (m_order + 1) / 2 - 1;
    addStep(QString("除根外，每个节点至少 %1 个关键字").arg(minKeys));
    addStep(QString("初始序列: %1").arg(initialValues));

    for (int v : vals)
        insert(v);

    m_resultText = QString("B树(%1阶)构建完成，共%2个快照").arg(m_order).arg(m_snapshots.size());

    m_vizData = buildTreeSnapshot(QString("B树(m=%1)").arg(m_order));

    delete m_root;
    m_root = nullptr;
    m_running = false;
}

void BTree::step() { if (m_stepIndex == 0) run(); }

void BTree::reset() {
    delete m_root;
    m_root = nullptr;
    m_steps.clear();
    m_snapshots.clear();
    m_stepIndex = 0;
    m_resultText.clear();
    m_vizData.clear();
}
