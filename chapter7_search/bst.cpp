#include "bst.h"

BST::BST(QObject *parent) : AlgorithmBase(parent) {}

// ==================== Tree helpers (silent, no snapshots) ====================

BST::Node* BST::insert(Node *n, int v) {
    if (!n) return new Node(v);
    if (v < n->val) n->l = insert(n->l, v);
    else if (v > n->val) n->r = insert(n->r, v);
    return n;
}

BST::Node* BST::findMin(Node *n) {
    while (n && n->l) n = n->l;
    return n;
}

void BST::deleteTree(Node *n) {
    if (!n) return;
    deleteTree(n->l);
    deleteTree(n->r);
    delete n;
}

void BST::inorderString(Node *n, QString &s) {
    if (!n) return;
    inorderString(n->l, s);
    s += QString::number(n->val) + " ";
    inorderString(n->r, s);
}

// ==================== Snapshot building ====================

void BST::collectNodes(Node *n, int &id, int parentId,
                       QVariantList &nodes, const QMap<int, QString> &highlightVals) {
    if (!n) return;
    QVariantMap nd;
    nd["id"] = id;
    nd["label"] = QString::number(n->val);
    nd["parentId"] = parentId;
    if (highlightVals.contains(n->val))
        nd["color"] = highlightVals[n->val];

    int curId = id;
    nodes.append(nd);
    if (n->l) { id++; collectNodes(n->l, id, curId, nodes, highlightVals); }
    if (n->r) { id++; collectNodes(n->r, id, curId, nodes, highlightVals); }
}

QVariantMap BST::buildTreeSnapshot(const QString &title,
                                    const QMap<int, QString> &highlightVals) {
    QVariantList nodes;
    int id = 0;
    collectNodes(m_root, id, -1, nodes, highlightVals);
    QVariantMap snap;
    snap["type"] = "tree";
    snap["title"] = title;
    snap["nodes"] = nodes;
    return snap;
}

// ==================== Animated operations ====================

void BST::insertAnimated(int v) {
    addStep(QString("开始插入: %1").arg(v));

    if (!m_root) {
        m_root = new Node(v);
        addStep(QString("树为空，%1 成为根节点").arg(v));
        QMap<int, QString> hl; hl[v] = "#2196F3";
        recordSnapshot(buildTreeSnapshot(QString("BST - 插入%1为根").arg(v), hl));
        return;
    }

    // Walk tree to find insertion point, recording search path
    Node *cur = m_root;
    QMap<int, QString> pathHl;
    while (true) {
        pathHl[cur->val] = "#FF9800";
        if (v == cur->val) {
            addStep(QString("%1 已存在，跳过").arg(v));
            recordSnapshot(buildTreeSnapshot(QString("BST - %1已存在").arg(v), pathHl));
            return;
        }
        QString dir = (v < cur->val) ? "左" : "右";
        addStep(QString("比较 %1 与 %2: %1 < %3, 向%4").arg(v).arg(cur->val)
                .arg(v < cur->val ? cur->val : v).arg(dir));
        recordSnapshot(buildTreeSnapshot(QString("插入%1 - 比较%2").arg(v).arg(cur->val), pathHl));

        Node *next = (v < cur->val) ? cur->l : cur->r;
        if (!next) break;
        cur = next;
    }

    // Insert the node
    m_root = insert(m_root, v);
    addStep(QString("插入 %1 完成").arg(v));
    QMap<int, QString> hl; hl[v] = "#2196F3";
    recordSnapshot(buildTreeSnapshot(QString("BST - 已插入%1").arg(v), hl));
}

void BST::deleteAnimated(int v) {
    addStep(QString("开始删除: %1").arg(v));

    if (!m_root) {
        addStep("树为空，无法删除");
        recordSnapshot(buildTreeSnapshot("BST - 树为空"));
        return;
    }

    // Search for target node and its parent (iterative)
    Node *target = m_root, *targetParent = nullptr;
    QMap<int, QString> pathHl;
    while (target && target->val != v) {
        pathHl[target->val] = "#FF9800";
        addStep(QString("查找 %1: 比较 %2, 向%3")
                .arg(v).arg(target->val)
                .arg(v < target->val ? "左" : "右"));
        recordSnapshot(buildTreeSnapshot(QString("删除%1 - 查找路径").arg(v), pathHl));
        targetParent = target;
        target = (v < target->val) ? target->l : target->r;
    }

    if (!target) {
        addStep(QString("节点 %1 不存在").arg(v));
        recordSnapshot(buildTreeSnapshot(QString("BST - %1不存在").arg(v)));
        return;
    }

    pathHl[v] = "#F44336";
    addStep(QString("找到目标节点 %1").arg(v));
    recordSnapshot(buildTreeSnapshot(QString("删除%1 - 找到目标").arg(v), pathHl));

    // Case 1: Leaf node
    if (!target->l && !target->r) {
        addStep(QString("%1 是叶子节点，直接删除").arg(v));
        if (!targetParent) m_root = nullptr;
        else if (targetParent->l == target) targetParent->l = nullptr;
        else targetParent->r = nullptr;
        delete target;
    }
    // Case 2: One child
    else if (!target->l) {
        addStep(QString("%1 只有右孩子 %2, 用孩子替换").arg(v).arg(target->r->val));
        if (!targetParent) m_root = target->r;
        else if (targetParent->l == target) targetParent->l = target->r;
        else targetParent->r = target->r;
        delete target;
    }
    else if (!target->r) {
        addStep(QString("%1 只有左孩子 %2, 用孩子替换").arg(v).arg(target->l->val));
        if (!targetParent) m_root = target->l;
        else if (targetParent->l == target) targetParent->l = target->l;
        else targetParent->r = target->l;
        delete target;
    }
    // Case 3: Two children
    else {
        // Find successor (leftmost of right subtree) and its parent
        Node *succParent = target;
        Node *succ = target->r;
        while (succ->l) { succParent = succ; succ = succ->l; }

        QMap<int, QString> succHl;
        succHl[v] = "#F44336";
        succHl[succ->val] = "#2196F3";
        addStep(QString("%1 有两个孩子，后继为 %2").arg(v).arg(succ->val));
        recordSnapshot(buildTreeSnapshot(QString("删除%1 - 后继%2").arg(v).arg(succ->val), succHl));

        // Replace target value with successor value
        target->val = succ->val;
        addStep(QString("用后继 %1 的值替换 %2").arg(succ->val).arg(v));
        recordSnapshot(buildTreeSnapshot(QString("删除%1 - 值已替换").arg(v)));

        // Remove the successor node (it has at most a right child)
        if (succParent->l == succ) succParent->l = succ->r;
        else succParent->r = succ->r;
        delete succ;
    }

    addStep(QString("删除 %1 完成").arg(v));
    recordSnapshot(buildTreeSnapshot(QString("BST - 已删除%1").arg(v)));
}

void BST::searchAnimated(int v) {
    addStep(QString("开始查找: %1").arg(v));

    if (!m_root) {
        addStep("树为空");
        recordSnapshot(buildTreeSnapshot("BST - 树为空"));
        return;
    }

    Node *cur = m_root;
    QMap<int, QString> pathHl;
    while (cur) {
        pathHl[cur->val] = "#FF9800";
        if (cur->val == v) {
            pathHl[v] = "#2196F3";
            addStep(QString("查找 %1: 比较 %2 == %1, 找到!").arg(v).arg(cur->val));
            recordSnapshot(buildTreeSnapshot(QString("BST - 找到%1").arg(v), pathHl));
            return;
        }
        QString dir = (v < cur->val) ? "左" : "右";
        addStep(QString("查找 %1: 比较 %2, 向%3").arg(v).arg(cur->val).arg(dir));
        recordSnapshot(buildTreeSnapshot(QString("查找%1 - 比较%2").arg(v).arg(cur->val), pathHl));
        cur = (v < cur->val) ? cur->l : cur->r;
    }

    addStep(QString("未找到 %1").arg(v));
    recordSnapshot(buildTreeSnapshot(QString("BST - 未找到%1").arg(v)));
}

// ==================== run / step / reset ====================

void BST::run() {
    reset();

    QString initialValues = m_params.value("initialValues", "50 30 70 20 40 60 80").toString();
    QString operation = m_params.value("operation", "构建").toString();
    int targetValue = m_params.value("targetValue", 35).toInt();

    // Parse initial values
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

    addStep(QString("操作类型: %1").arg(operation));
    addStep(QString("初始序列: %1").arg(initialValues));
    if (operation != "构建")
        addStep(QString("目标值: %1").arg(targetValue));
    addStep("===== 开始构建初始树 =====");

    // Build initial tree with animation
    for (int v : vals) {
        insertAnimated(v);
    }

    // Perform the selected operation
    addStep(QString("===== 执行%1操作 =====").arg(operation));
    if (operation == "插入") {
        insertAnimated(targetValue);
    } else if (operation == "删除") {
        deleteAnimated(targetValue);
    } else if (operation == "查找") {
        searchAnimated(targetValue);
    }
    // "构建": already done, nothing extra

    // Result text
    QString inorder;
    inorderString(m_root, inorder);
    m_resultText = QString("中序遍历: %1\n快照数: %2").arg(inorder.trimmed()).arg(m_snapshots.size());

    // Final viz for backward compatibility
    m_vizData = buildTreeSnapshot("二叉排序树(BST)");

    deleteTree(m_root);
    m_root = nullptr;
    m_running = false;
}

void BST::step() { if (m_stepIndex == 0) run(); }

void BST::reset() {
    deleteTree(m_root);
    m_root = nullptr;
    m_steps.clear();
    m_snapshots.clear();
    m_stepIndex = 0;
    m_resultText.clear();
    m_vizData.clear();
}
