#include "binarytreetraversal.h"
#include <QQueue>

BinaryTreeTraversal::BinaryTreeTraversal(QObject *parent) : AlgorithmBase(parent) {}

QVector<QString> BinaryTreeTraversal::parameterNames() const {
    return {"mode"};
}
QVector<QString> BinaryTreeTraversal::parameterDescriptions() const {
    return {"遍历模式"};
}
QVector<QVariant> BinaryTreeTraversal::parameterDefaults() const {
    return {QVariant(QVariantList{"递归", "非递归"})};
}

BinaryTreeTraversal::Node* BinaryTreeTraversal::buildTree() {
    //      1
    //     / \
    //    2   3
    //   / \   \
    //  4   5   6
    auto *r = new Node{1};
    r->left = new Node{2}; r->right = new Node{3};
    r->left->left = new Node{4}; r->left->right = new Node{5};
    r->right->right = new Node{6};
    return r;
}

void BinaryTreeTraversal::deleteTree(Node *n) {
    if (!n) return; deleteTree(n->left); deleteTree(n->right); delete n;
}

void BinaryTreeTraversal::preOrder(Node *root) {
    if (!root) return;
    m_result += QString::number(root->val) + " ";
    addStep(QString("前序-访问节点: %1").arg(root->val));
    preOrder(root->left);
    preOrder(root->right);
}

void BinaryTreeTraversal::inOrder(Node *root) {
    if (!root) return;
    inOrder(root->left);
    m_result += QString::number(root->val) + " ";
    addStep(QString("中序-访问节点: %1").arg(root->val));
    inOrder(root->right);
}

void BinaryTreeTraversal::postOrder(Node *root) {
    if (!root) return;
    postOrder(root->left);
    postOrder(root->right);
    m_result += QString::number(root->val) + " ";
    addStep(QString("后序-访问节点: %1").arg(root->val));
}

void BinaryTreeTraversal::levelOrder(Node *root) {
    if (!root) return;
    QQueue<Node*> q; q.enqueue(root);
    while (!q.isEmpty()) {
        Node *n = q.dequeue();
        m_result += QString::number(n->val) + " ";
        addStep(QString("层序-访问节点: %1").arg(n->val));
        if (n->left) q.enqueue(n->left);
        if (n->right) q.enqueue(n->right);
    }
}

void BinaryTreeTraversal::preOrderNR(Node *root) {
    QStack<Node*> s;
    Node *p = root;
    while (p || !s.isEmpty()) {
        while (p) {
            m_result += QString::number(p->val) + " ";
            addStep(QString("前序(非递归)-访问: %1，入栈").arg(p->val));
            s.push(p); p = p->left;
        }
        if (!s.isEmpty()) { p = s.pop()->right; }
    }
}

void BinaryTreeTraversal::inOrderNR(Node *root) {
    QStack<Node*> s;
    Node *p = root;
    while (p || !s.isEmpty()) {
        while (p) {
            addStep(QString("中序(非递归)-入栈: %1").arg(p->val));
            s.push(p); p = p->left;
        }
        if (!s.isEmpty()) {
            p = s.pop();
            m_result += QString::number(p->val) + " ";
            addStep(QString("中序(非递归)-访问: %1").arg(p->val));
            p = p->right;
        }
    }
}

void BinaryTreeTraversal::postOrderNR(Node *root) {
    QStack<Node*> s1, s2;
    s1.push(root);
    while (!s1.isEmpty()) {
        Node *n = s1.pop(); s2.push(n);
        if (n->left) s1.push(n->left);
        if (n->right) s1.push(n->right);
        addStep(QString("后序(非递归)-处理节点: %1").arg(n->val));
    }
    while (!s2.isEmpty()) {
        m_result += QString::number(s2.pop()->val) + " ";
    }
}

void BinaryTreeTraversal::run() {
    reset();
    m_mode = m_params.value("mode", "递归").toString();
    m_root = buildTree();

    addStep(QString("二叉树结构:\n       1\n      / \\\n     2   3\n    / \\   \\\n   4   5   6"));
    addStep(QString("遍历模式: %1").arg(m_mode));

    if (m_mode == "递归") {
        m_result.clear(); addStep("--- 前序遍历 ---");
        preOrder(m_root); addStep(QString("前序结果: %1").arg(m_result.trimmed()));
        m_result.clear(); addStep("--- 中序遍历 ---");
        inOrder(m_root); addStep(QString("中序结果: %1").arg(m_result.trimmed()));
        m_result.clear(); addStep("--- 后序遍历 ---");
        postOrder(m_root); addStep(QString("后序结果: %1").arg(m_result.trimmed()));
        m_result.clear(); addStep("--- 层次遍历 ---");
        levelOrder(m_root); addStep(QString("层序结果: %1").arg(m_result.trimmed()));
    } else {
        m_result.clear(); addStep("--- 前序遍历(非递归) ---");
        preOrderNR(m_root); addStep(QString("前序结果: %1").arg(m_result.trimmed()));
        m_result.clear(); addStep("--- 中序遍历(非递归) ---");
        inOrderNR(m_root); addStep(QString("中序结果: %1").arg(m_result.trimmed()));
        m_result.clear(); addStep("--- 后序遍历(非递归) ---");
        postOrderNR(m_root); addStep(QString("后序结果: %1").arg(m_result.trimmed()));
    }

    QVariantMap viz;
    viz["type"] = "tree"; viz["title"] = "二叉树遍历";
    QVariantList nodes;
    auto addN = [&](int id, QString l, int pid) {
        QVariantMap n; n["id"]=id; n["label"]=l; n["parentId"]=pid; nodes.append(n);
    };
    addN(1,"1",-1); addN(2,"2",1); addN(3,"3",1);
    addN(4,"4",2); addN(5,"5",2); addN(6,"6",3);
    viz["nodes"] = nodes;
    m_vizData = viz;

    m_resultText = "遍历完成，详见上方日志";
    deleteTree(m_root); m_root = nullptr;
    m_running = false;
}

void BinaryTreeTraversal::step() { if (m_stepIndex==0) run(); }
void BinaryTreeTraversal::reset() { m_steps.clear(); m_stepIndex=0; m_result.clear(); m_resultText.clear(); m_vizData.clear(); }
