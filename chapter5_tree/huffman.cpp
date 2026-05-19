#include "huffman.h"
#include <QQueue>
#include <algorithm>

HuffmanCoding::HuffmanCoding(QObject *parent) : AlgorithmBase(parent) {}

QVector<QString> HuffmanCoding::parameterNames() const {
    return {"weights"};
}
QVector<QString> HuffmanCoding::parameterDescriptions() const {
    return {"权值序列(空格分隔，如: 5 29 7 8 14 23 3 11)"};
}
QVector<QVariant> HuffmanCoding::parameterDefaults() const {
    return {QString("5 29 7 8 14 23 3 11")};
}

void HuffmanCoding::buildCodes(HuffNode *node, QString code, QMap<char, QString> &codes) {
    if (!node) return;
    if (!node->left && !node->right) {
        codes[node->ch] = code.isEmpty() ? "0" : code;
        addStep(QString("字符'%1' 权值=%2 编码=%3").arg(node->ch).arg(node->weight).arg(codes[node->ch]));
        return;
    }
    buildCodes(node->left, code + "0", codes);
    buildCodes(node->right, code + "1", codes);
}

void HuffmanCoding::run() {
    reset();
    m_weights = m_params.value("weights", "5 29 7 8 14 23 3 11").toString();

    QStringList parts = m_weights.split(' ', Qt::SkipEmptyParts);
    QVector<int> weights;
    for (const auto &p : parts) weights.append(p.toInt());

    if (weights.size() < 2) {
        m_resultText = "至少需要2个权值";
        m_running = false; return;
    }

    addStep(QString("输入权值: %1").arg(m_weights));
    addStep("===== 构造哈夫曼树 =====");

    // Min-heap by weight
    QVector<HuffNode*> heap;
    for (int i = 0; i < weights.size(); i++) {
        auto *n = new HuffNode{weights[i], (char)('A' + i)};
        heap.append(n);
        addStep(QString("创建叶子节点: %1 (权值=%2)").arg(n->ch).arg(weights[i]));
    }

    auto cmp = [](HuffNode *a, HuffNode *b) { return a->weight > b->weight; };

    while (heap.size() > 1) {
        std::sort(heap.begin(), heap.end(), cmp);
        auto *left = heap.takeFirst();
        auto *right = heap.takeFirst();
        auto *parent = new HuffNode{left->weight + right->weight, ' '};
        parent->left = left; parent->right = right;
        heap.append(parent);
        addStep(QString("合并: %1(%2) + %3(%4) = 新节点(权值=%5)")
                .arg(left->ch!=' '?QString(left->ch):QString("内部"))
                .arg(left->weight)
                .arg(right->ch!=' '?QString(right->ch):QString("内部"))
                .arg(right->weight).arg(parent->weight));
    }

    HuffNode *root = heap.isEmpty() ? nullptr : heap[0];
    addStep("===== 生成哈夫曼编码 =====");
    QMap<char, QString> codes;
    buildCodes(root, "", codes);

    m_resultText = "哈夫曼编码:\n";
    for (auto it = codes.begin(); it != codes.end(); ++it) {
        m_resultText += QString("  %1 (权值): %2\n").arg(it.key());
        // find weight
    }

    // Calculate WPL
    int wpl = 0;
    for (int i = 0; i < weights.size(); i++) {
        char ch = 'A' + i;
        wpl += weights[i] * codes[ch].size();
    }
    m_resultText += QString("\nWPL(带权路径长度) = %1").arg(wpl);

    // Cleanup
    std::function<void(HuffNode*)> del = [&](HuffNode *n) {
        if (!n) return; del(n->left); del(n->right); delete n;
    };
    del(root);

    QVariantMap viz; viz["type"]="tree"; viz["title"]="哈夫曼树"; viz["nodes"]=QVariantList(); m_vizData=viz;
    m_running=false;
}

void HuffmanCoding::step() { if(m_stepIndex==0)run(); }
void HuffmanCoding::reset() { m_steps.clear(); m_stepIndex=0; m_resultText.clear(); m_vizData.clear(); }
