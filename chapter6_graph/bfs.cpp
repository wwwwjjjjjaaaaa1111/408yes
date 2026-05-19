#include "bfs.h"
#include <QQueue>

BFS::BFS(QObject *parent) : AlgorithmBase(parent) {}

QVector<QString> BFS::parameterNames() const { return {"startVertex"}; }
QVector<QString> BFS::parameterDescriptions() const { return {"起始顶点编号(0开始)"}; }
QVector<QVariant> BFS::parameterDefaults() const { return {QVariant(0)}; }

QVector<QVector<int>> BFS::buildSampleGraph() {
    // 6 vertices, adjacency matrix
    //     0 -- 1 -- 2
    //     |    |    |
    //     3 -- 4 -- 5
    QVector<QVector<int>> g(6, QVector<int>(6, 0));
    g[0][1]=1; g[1][0]=1; g[0][3]=1; g[3][0]=1;
    g[1][2]=1; g[2][1]=1; g[1][4]=1; g[4][1]=1;
    g[2][5]=1; g[5][2]=1;
    g[3][4]=1; g[4][3]=1;
    g[4][5]=1; g[5][4]=1;
    return g;
}

void BFS::run() {
    reset();
    m_startVertex = m_params.value("startVertex", 0).toInt();
    auto graph = buildSampleGraph();
    int n = graph.size();
    m_startVertex = qBound(0, m_startVertex, n-1);

    addStep("图结构 (邻接矩阵):");
    addStep("    0 -- 1 -- 2");
    addStep("    |    |    |");
    addStep("    3 -- 4 -- 5");
    addStep(QString("从顶点 %1 开始BFS").arg(m_startVertex));

    QVector<bool> visited(n, false);
    QQueue<int> q;
    q.enqueue(m_startVertex);
    visited[m_startVertex] = true;
    addStep(QString("顶点%1入队，标记已访问").arg(m_startVertex));

    QString order;
    int level = 0;

    while (!q.isEmpty()) {
        int levelSize = q.size();
        addStep(QString("--- 第%1层 ---").arg(level++));
        for (int i = 0; i < levelSize; i++) {
            int v = q.dequeue();
            order += QString::number(v) + " ";
            addStep(QString("出队顶点: %1").arg(v));
            for (int u = 0; u < n; u++) {
                if (graph[v][u] && !visited[u]) {
                    visited[u] = true;
                    q.enqueue(u);
                    addStep(QString("  访问邻接点%1，入队").arg(u));
                }
            }
        }
    }

    m_resultText = QString("BFS遍历顺序: %1").arg(order.trimmed());

    QVariantMap viz; viz["type"]="graph"; viz["title"]="BFS遍历";
    QVariantList nodes, edges;
    for (int i=0;i<n;i++){QVariantMap nd; nd["id"]=i; nd["label"]=QString::number(i); nodes.append(nd);}
    for (int i=0;i<n;i++)for(int j=i+1;j<n;j++)if(graph[i][j]){QVariantMap ed; ed["from"]=i;ed["to"]=j;ed["weight"]=1;edges.append(ed);}
    viz["nodes"]=nodes;viz["edges"]=edges; m_vizData=viz;
    m_running=false;
}

void BFS::step() { if(m_stepIndex==0)run(); }
void BFS::reset() { m_steps.clear();m_stepIndex=0;m_resultText.clear();m_vizData.clear(); }
