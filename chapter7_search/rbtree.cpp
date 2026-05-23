#include "rbtree.h"

RBTree::RBTree(QObject *parent) : AlgorithmBase(parent) {}

// ==================== Rotations ====================

void RBTree::rotateLeft(Node *x) {
    Node *y = x->r; x->r = y->l;
    if (y->l) y->l->p = x; y->p = x->p;
    if (!x->p) m_root = y;
    else if (x == x->p->l) x->p->l = y;
    else x->p->r = y;
    y->l = x; x->p = y;
    addStep(QString("左旋: %1 ← %2").arg(x->val).arg(y->val));
}

void RBTree::rotateRight(Node *y) {
    Node *x = y->l; y->l = x->r;
    if (x->r) x->r->p = y; x->p = y->p;
    if (!y->p) m_root = x;
    else if (y == y->p->l) y->p->l = x;
    else y->p->r = x;
    x->r = y; y->p = x;
    addStep(QString("右旋: %1 → %2").arg(x->val).arg(y->val));
}

// ==================== fixInsert with snapshots ====================

void RBTree::fixInsert(Node *z) {
    while (z->p && z->p->c == RED) {
        if (z->p == z->p->p->l) {
            Node *y = z->p->p->r;
            if (y && y->c == RED) {
                addStep(QString("情况1(叔红): 父%1、叔%2 变黑，祖父%3 变红")
                        .arg(z->p->val).arg(y->val).arg(z->p->p->val));
                z->p->c = BLACK; y->c = BLACK; z->p->p->c = RED; z = z->p->p;
                recordSnapshot(buildTreeSnapshot(QString("红黑树 - 情况1(变色)"), {}));
            } else {
                if (z == z->p->r) {
                    addStep(QString("情况2(LR): 对%1左旋").arg(z->p->val));
                    z = z->p; rotateLeft(z);
                    recordSnapshot(buildTreeSnapshot(QString("红黑树 - 情况2(LR旋转)"), {}));
                }
                addStep(QString("情况3(LL): 对%1右旋, 父黑祖父红").arg(z->p->p->val));
                z->p->c = BLACK; z->p->p->c = RED; rotateRight(z->p->p);
                recordSnapshot(buildTreeSnapshot(QString("红黑树 - 情况3(LL旋转+变色)"), {}));
            }
        } else {
            Node *y = z->p->p->l;
            if (y && y->c == RED) {
                addStep(QString("情况1(叔红): 父%1、叔%2 变黑，祖父%3 变红")
                        .arg(z->p->val).arg(y->val).arg(z->p->p->val));
                z->p->c = BLACK; y->c = BLACK; z->p->p->c = RED; z = z->p->p;
                recordSnapshot(buildTreeSnapshot(QString("红黑树 - 情况1(变色)"), {}));
            } else {
                if (z == z->p->l) {
                    addStep(QString("情况2(RL): 对%1右旋").arg(z->p->val));
                    z = z->p; rotateRight(z);
                    recordSnapshot(buildTreeSnapshot(QString("红黑树 - 情况2(RL旋转)"), {}));
                }
                addStep(QString("情况3(RR): 对%1左旋, 父黑祖父红").arg(z->p->p->val));
                z->p->c = BLACK; z->p->p->c = RED; rotateLeft(z->p->p);
                recordSnapshot(buildTreeSnapshot(QString("红黑树 - 情况3(RR旋转+变色)"), {}));
            }
        }
    }
    m_root->c = BLACK;
}

// ==================== Animated insert ====================

void RBTree::insertAnimated(int v) {
    addStep(QString("开始插入: %1").arg(v));

    if (!m_root) {
        m_root = new Node(v);
        m_root->c = BLACK;
        addStep(QString("树为空，%1 成为黑色根节点").arg(v));
        QMap<int, QString> hl; hl[v] = "#2196F3";
        recordSnapshot(buildTreeSnapshot(QString("红黑树 - 插入根%1").arg(v), hl));
        return;
    }

    // Search walk with snapshots
    Node *cur = m_root;
    QMap<int, QString> pathHl;
    while (true) {
        pathHl[cur->val] = "#FF9800";
        if (v == cur->val) {
            addStep(QString("%1 已存在，跳过").arg(v));
            recordSnapshot(buildTreeSnapshot(QString("红黑树 - %1已存在").arg(v), pathHl));
            return;
        }
        QString dir = (v < cur->val) ? "左" : "右";
        addStep(QString("比较 %1 与 %2, 向%3").arg(v).arg(cur->val).arg(dir));
        recordSnapshot(buildTreeSnapshot(QString("插入%1 - 比较%2").arg(v).arg(cur->val), pathHl));

        Node *next = (v < cur->val) ? cur->l : cur->r;
        if (!next) break;
        cur = next;
    }

    // Create and attach new red node
    Node *z = new Node(v);
    z->c = RED; z->p = cur;
    if (v < cur->val) {
        cur->l = z;
        addStep(QString("插入红色节点 %1 作为 %2 的左孩子").arg(v).arg(cur->val));
    } else {
        cur->r = z;
        addStep(QString("插入红色节点 %1 作为 %2 的右孩子").arg(v).arg(cur->val));
    }

    QMap<int, QString> insHl; insHl[v] = "#2196F3";
    recordSnapshot(buildTreeSnapshot(QString("红黑树 - 已插入%1(红)").arg(v), insHl));

    // Fix violations
    if (z->p && z->p->c == RED) {
        addStep("违反红黑性质: 连续两个红色节点，开始修复");
        fixInsert(z);
    }

    m_root->c = BLACK;
    addStep(QString("插入 %1 完成，根保持黑色").arg(v));
    recordSnapshot(buildTreeSnapshot(QString("红黑树 - 完成插入%1").arg(v)));
}

// ==================== Tree helpers ====================

void RBTree::deleteTree(Node *n) { if (!n) return; deleteTree(n->l); deleteTree(n->r); delete n; }

void RBTree::inorderString(Node *n, QString &s) {
    if (!n) return;
    inorderString(n->l, s);
    s += QString::number(n->val) + (n->c == RED ? "R" : "B") + " ";
    inorderString(n->r, s);
}

// ==================== Snapshot building ====================

void RBTree::collectNodes(Node *n, int &id, int pid, QVariantList &nodes,
                           const QMap<int, QString> &highlightVals) {
    if (!n) return;
    QVariantMap nd;
    nd["id"] = id;
    nd["label"] = QString::number(n->val);
    nd["parentId"] = pid;
    nd["isRed"] = (n->c == RED);
    if (highlightVals.contains(n->val))
        nd["color"] = highlightVals[n->val];

    int cid = id;
    nodes.append(nd);
    if (n->l) { id++; collectNodes(n->l, id, cid, nodes, highlightVals); }
    if (n->r) { id++; collectNodes(n->r, id, cid, nodes, highlightVals); }
}

QVariantMap RBTree::buildTreeSnapshot(const QString &title,
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

// ==================== run / step / reset ====================

void RBTree::run() {
    reset();

    QString initialValues = m_params.value("initialValues", "10 20 30 15 25 5 35").toString();
    QString operation = m_params.value("operation", "构建").toString();
    int targetValue = m_params.value("targetValue", 40).toInt();

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
    addStep("===== 开始构建红黑树 =====");

    for (int v : vals)
        insertAnimated(v);

    if (operation == "插入") {
        addStep(QString("===== 执行插入 %1 =====").arg(targetValue));
        insertAnimated(targetValue);
    }

    QString inorder;
    inorderString(m_root, inorder);
    m_resultText = QString("中序遍历(R=红色,B=黑色): %1\n快照数: %2")
                   .arg(inorder.trimmed()).arg(m_snapshots.size());

    m_vizData = buildTreeSnapshot("红黑树");

    deleteTree(m_root);
    m_root = nullptr;
    m_running = false;
}

void RBTree::step() { if (m_stepIndex == 0) run(); }

void RBTree::reset() {
    deleteTree(m_root);
    m_root = nullptr;
    m_steps.clear();
    m_snapshots.clear();
    m_stepIndex = 0;
    m_resultText.clear();
    m_vizData.clear();
}
