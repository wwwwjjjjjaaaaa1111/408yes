#include "unionfind.h"

UnionFind::UnionFind(QObject *parent) : AlgorithmBase(parent) {}

QVector<QString> UnionFind::parameterNames() const {
    return {"size"};
}
QVector<QString> UnionFind::parameterDescriptions() const {
    return {"元素个数"};
}
QVector<QVariant> UnionFind::parameterDefaults() const {
    return {QVariant(10)};
}

int UnionFind::find(QVector<int> &parent, int x) {
    if (parent[x] != x) {
        int oldParent = parent[x];
        parent[x] = find(parent, parent[x]); // path compression
        addStep(QString("查找 %1: 路径压缩 %1→%2 (原指向%3)").arg(x).arg(parent[x]).arg(oldParent));
    } else {
        addStep(QString("查找 %1: 根节点就是自己").arg(x));
    }
    return parent[x];
}

void UnionFind::unionSet(QVector<int> &parent, QVector<int> &rank, int a, int b) {
    int ra = find(parent, a);
    int rb = find(parent, b);
    if (ra == rb) {
        addStep(QString("合并 %1 和 %2: 已在同一集合(根=%3)").arg(a).arg(b).arg(ra));
        return;
    }
    if (rank[ra] < rank[rb]) {
        parent[ra] = rb;
        addStep(QString("合并: %1→%2 (rank[%3]=%4 < rank[%5]=%6，小树合并到大树)")
                .arg(ra).arg(rb).arg(ra).arg(rank[ra]).arg(rb).arg(rank[rb]));
    } else if (rank[ra] > rank[rb]) {
        parent[rb] = ra;
        addStep(QString("合并: %1→%2").arg(rb).arg(ra));
    } else {
        parent[rb] = ra; rank[ra]++;
        addStep(QString("合并: %1→%2，rank[%3]增加到%4")
                .arg(rb).arg(ra).arg(ra).arg(rank[ra]));
    }
}

void UnionFind::run() {
    reset();
    m_size = m_params.value("size", 10).toInt();
    m_size = qBound(2, m_size, 20);

    QVector<int> parent(m_size), rank(m_size, 0);
    for (int i = 0; i < m_size; i++) parent[i] = i;

    addStep(QString("初始化并查集，%1个元素各自独立").arg(m_size));
    addStep(QString("初始状态: parent = [%1]").arg(
        [&](){QString s; for(int i=0;i<m_size;i++)s+=QString::number(parent[i])+" ";return s.trimmed();}()));

    // Demonstrate some operations
    struct Op { int a, b; };
    QVector<Op> ops = {{0,1},{2,3},{4,5},{6,7},{8,9},{0,2},{4,6},{0,4}};
    for (const auto &op : ops) {
        if (op.a >= m_size || op.b >= m_size) continue;
        unionSet(parent, rank, op.a, op.b);
    }

    // Final find with path compression
    addStep("--- 路径压缩演示 ---");
    for (int i = 0; i < m_size; i++) {
        find(parent, i);
    }

    addStep(QString("最终状态: parent = [%1]").arg(
        [&](){QString s; for(int i=0;i<m_size;i++)s+=QString::number(parent[i])+" ";return s.trimmed();}()));

    m_resultText = QString("并查集演示完成（%1个元素）").arg(m_size);
    m_running = false;
}

void UnionFind::step() { if(m_stepIndex==0)run(); }
void UnionFind::reset() { m_steps.clear(); m_stepIndex=0; m_resultText.clear(); }
