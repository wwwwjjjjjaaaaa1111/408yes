#include "levelorder.h"

LevelOrder::LevelOrder(QObject *parent) : AlgorithmBase(parent) {}

QVector<QString> LevelOrder::parameterNames() const { return {}; }
QVector<QString> LevelOrder::parameterDescriptions() const { return {}; }
QVector<QVariant> LevelOrder::parameterDefaults() const { return {}; }

LevelOrder::TreeNode *LevelOrder::buildSampleTree() {
    // Build:      1
    //           /   \
    //          2     3
    //         / \   / \
    //        4   5 6   7
    auto *root = new TreeNode{1};
    root->left = new TreeNode{2};
    root->right = new TreeNode{3};
    root->left->left = new TreeNode{4};
    root->left->right = new TreeNode{5};
    root->right->left = new TreeNode{6};
    root->right->right = new TreeNode{7};
    return root;
}

void LevelOrder::deleteTree(TreeNode *node) {
    if (!node) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

QString LevelOrder::levelOrderTraversal(TreeNode *root) {
    if (!root) return "";
    QQueue<TreeNode*> q;
    q.enqueue(root);
    QString result;

    addStep("将根节点1入队");
    int level = 0;

    while (!q.isEmpty()) {
        int levelSize = q.size();
        QString levelNodes;
        for (int i = 0; i < levelSize; i++) {
            TreeNode *node = q.dequeue();
            levelNodes += QString::number(node->val) + " ";
            if (node->left) {
                q.enqueue(node->left);
                addStep(QString("访问节点%1，将其左孩子%2入队").arg(node->val).arg(node->left->val));
            }
            if (node->right) {
                q.enqueue(node->right);
                addStep(QString("访问节点%1，将其右孩子%2入队").arg(node->val).arg(node->right->val));
            }
            if (!node->left && !node->right) {
                addStep(QString("访问节点%1(叶子节点，无孩子)").arg(node->val));
            }
        }
        result += QString("第%1层: %2\n").arg(level++).arg(levelNodes.trimmed());
    }
    addStep(QString("层次遍历完成: ") + result.replace("\n", " → "));
    return result;
}

void LevelOrder::run() {
    reset();
    m_root = buildSampleTree();
    addStep("构建示例二叉树:\n       1\n      / \\\n     2   3\n    / \\ / \\\n    4 5 6 7");
    QString traversal = levelOrderTraversal(m_root);
    m_resultText = "层次遍历结果:\n" + traversal;

    QVariantMap vizData;
    vizData["type"] = "tree";
    vizData["title"] = "二叉树层次遍历";
    QVariantList nodes;
    auto addNode = [&](int id, QString label, int parent) {
        QVariantMap n;
        n["id"] = id; n["label"] = label; n["parentId"] = parent;
        nodes.append(n);
    };
    addNode(1, "1", -1);
    addNode(2, "2", 1); addNode(3, "3", 1);
    addNode(4, "4", 2); addNode(5, "5", 2);
    addNode(6, "6", 3); addNode(7, "7", 3);
    vizData["nodes"] = nodes;
    m_vizData = vizData;

    deleteTree(m_root);
    m_root = nullptr;
    m_running = false;
}

void LevelOrder::step() { if (m_stepIndex == 0) run(); }
void LevelOrder::reset() { m_steps.clear(); m_stepIndex = 0; m_resultText.clear(); m_vizData.clear(); }
