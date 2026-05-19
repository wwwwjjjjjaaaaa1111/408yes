#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "algorithmbase.h"
#include <QMap>

class HuffmanCoding : public AlgorithmBase {
    Q_OBJECT
public:
    explicit HuffmanCoding(QObject *parent = nullptr);
    QString name() const override { return "哈夫曼树和哈夫曼编码"; }
    QString chapter() const override { return "第5章 树与二叉树"; }
    QString description() const override { return "输入权值，构造哈夫曼树并生成哈夫曼编码"; }

    void run() override;
    void step() override;
    void reset() override;

    QVector<QString> parameterNames() const override;
    QVector<QString> parameterDescriptions() const override;
    QVector<QVariant> parameterDefaults() const override;

private:
    struct HuffNode {
        int weight; char ch; HuffNode *left=nullptr, *right=nullptr;
        bool operator>(const HuffNode &o) const { return weight > o.weight; }
    };
    void buildCodes(HuffNode *node, QString code, QMap<char, QString> &codes);
    QString m_weights;
    int m_stepIndex = 0;
};

#endif
