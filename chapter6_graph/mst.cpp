#include "mst.h"
#include <algorithm>
#include <climits>

MST::MST(QObject *parent) : AlgorithmBase(parent) {}

QVector<QString> MST::parameterNames() const { return {"mode"}; }
QVector<QString> MST::parameterDescriptions() const { return {"算法选择"}; }
QVector<QVariant> MST::parameterDefaults() const { return {QVariant(QVariantList{"Prim", "Kruskal"})}; }

void MST::runPrim() {
    addStep("===== Prim算法 =====");
    // Graph: weighted undirected
    int n = 6;
    QVector<QVector<int>> g = {
        {0,6,1,5,0,0},
        {6,0,5,0,3,0},
        {1,5,0,5,6,4},
        {5,0,5,0,0,2},
        {0,3,6,0,0,6},
        {0,0,4,2,6,0}
    };
    addStep("图(邻接矩阵): 0-1:6, 0-2:1, 0-3:5, 1-2:5, 1-4:3, 2-3:5, 2-4:6, 2-5:4, 3-5:2, 4-5:6");

    QVector<bool> inMST(n, false);
    QVector<int> key(n, INT_MAX), parent(n, -1);
    key[0] = 0;
    int totalWeight = 0;

    for (int count = 0; count < n; count++) {
        int u = -1, minKey = INT_MAX;
        for (int i = 0; i < n; i++)
            if (!inMST[i] && key[i] < minKey) { minKey = key[i]; u = i; }

        if (u == -1) break;
        inMST[u] = true;
        if (parent[u] != -1) {
            totalWeight += g[u][parent[u]];
            addStep(QString("选中边: %1-%2 (权值=%3)")
                    .arg(parent[u]).arg(u).arg(g[u][parent[u]]));
        } else {
            addStep(QString("起始顶点: %1").arg(u));
        }

        for (int v = 0; v < n; v++) {
            if (!inMST[v] && g[u][v] && g[u][v] < key[v]) {
                key[v] = g[u][v]; parent[v] = u;
                addStep(QString("  更新: key[%1]=%2, parent[%1]=%3").arg(v).arg(key[v]).arg(u));
            }
        }
    }
    addStep(QString("最小生成树总权值: %1").arg(totalWeight));
    m_resultText = QString("Prim算法: 最小生成树总权值 = %1").arg(totalWeight);
}

void MST::runKruskal() {
    addStep("===== Kruskal算法 =====");
    struct Edge { int u, v, w; };
    QVector<Edge> edges = {
        {0,1,6},{0,2,1},{0,3,5},{1,2,5},{1,4,3},{2,3,5},{2,4,6},{2,5,4},{3,5,2},{4,5,6}
    };
    std::sort(edges.begin(), edges.end(), [](Edge &a, Edge &b){return a.w<b.w;});

    addStep("所有边按权值排序: " + [&](){QString s; for(auto&e:edges)s+=QString("(%1,%2:%3) ").arg(e.u).arg(e.v).arg(e.w);return s;}());

    int n = 6;
    QVector<int> parent(n), rank(n,0);
    for(int i=0;i<n;i++)parent[i]=i;

    std::function<int(int)> find = [&](int x)->int{
        if(parent[x]!=x)parent[x]=find(parent[x]);
        return parent[x];
    };
    auto unite = [&](int a,int b){
        int ra=find(a),rb=find(b);
        if(ra==rb)return false;
        if(rank[ra]<rank[rb])parent[ra]=rb;
        else if(rank[ra]>rank[rb])parent[rb]=ra;
        else{parent[rb]=ra;rank[ra]++;}
        return true;
    };

    int totalWeight=0,edgeCount=0;
    for(const auto &e:edges){
        if(unite(e.u,e.v)){
            totalWeight+=e.w; edgeCount++;
            addStep(QString("选中边: (%1,%2) 权值=%3").arg(e.u).arg(e.v).arg(e.w));
            if(edgeCount==n-1)break;
        } else {
            addStep(QString("跳过边: (%1,%2) 权值=%3 (会形成环)").arg(e.u).arg(e.v).arg(e.w));
        }
    }
    addStep(QString("最小生成树总权值: %1").arg(totalWeight));
    m_resultText = QString("Kruskal算法: 最小生成树总权值 = %1").arg(totalWeight);
}

void MST::run() {
    reset();
    m_mode = m_params.value("mode", "Prim").toString();

    if (m_mode == "Prim") runPrim();
    else runKruskal();

    m_running=false;
}

void MST::step() { if(m_stepIndex==0)run(); }
void MST::reset() { m_steps.clear();m_stepIndex=0;m_resultText.clear(); }
