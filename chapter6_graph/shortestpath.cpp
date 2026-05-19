#include "shortestpath.h"
#include <climits>

ShortestPath::ShortestPath(QObject *parent) : AlgorithmBase(parent) {}

QVector<QString> ShortestPath::parameterNames() const { return {"mode"}; }
QVector<QString> ShortestPath::parameterDescriptions() const { return {"算法选择"}; }
QVector<QVariant> ShortestPath::parameterDefaults() const { return {QVariant(QVariantList{"Dijkstra", "Floyd"})}; }

void ShortestPath::runDijkstra() {
    addStep("===== Dijkstra算法 (单源最短路径) =====");
    int n = 6;
    QVector<QVector<int>> g = {
        {0,10,0,0,0,3},
        {0,0,7,0,0,0},
        {0,0,0,5,0,0},
        {0,0,0,0,4,0},
        {0,0,0,0,0,0},
        {0,2,0,0,8,0}
    };
    addStep("有向图(邻接矩阵): 0→1:10, 0→5:3, 1→2:7, 2→3:5, 3→4:4, 5→1:2, 5→4:8");

    int src = 0;
    QVector<int> dist(n, INT_MAX);
    QVector<bool> visited(n, false);
    dist[src] = 0;
    addStep(QString("起点: %1, dist[%1]=0").arg(src));

    for (int count = 0; count < n; count++) {
        int u = -1, minDist = INT_MAX;
        for (int i = 0; i < n; i++)
            if (!visited[i] && dist[i] < minDist) { minDist = dist[i]; u = i; }
        if (u == -1) break;
        visited[u] = true;
        addStep(QString("选取顶点%1 (最短距离=%2)，加入已确定集合").arg(u).arg(dist[u]));

        for (int v = 0; v < n; v++) {
            if (!visited[v] && g[u][v] && dist[u] != INT_MAX && dist[u] + g[u][v] < dist[v]) {
                dist[v] = dist[u] + g[u][v];
                addStep(QString("  松弛: dist[%1] = min(∞, %2+%3) = %4").arg(v).arg(dist[u]).arg(g[u][v]).arg(dist[v]));
            }
        }
    }

    m_resultText = "Dijkstra结果(从0出发):\n";
    for (int i = 0; i < n; i++)
        m_resultText += QString("  到%1: %2\n").arg(i).arg(dist[i]==INT_MAX?"∞":QString::number(dist[i]));
}

void ShortestPath::runFloyd() {
    addStep("===== Floyd算法 (多源最短路径) =====");
    int n = 4;
    const int INF = 999;
    QVector<QVector<int>> d = {
        {0,2,6,4},
        {INF,0,3,INF},
        {7,INF,0,1},
        {5,INF,12,0}
    };
    addStep("图(邻接矩阵): 4个顶点, INF表示不可达");

    for (int k = 0; k < n; k++) {
        addStep(QString("--- 以%1为中间节点 ---").arg(k));
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (d[i][k] + d[k][j] < d[i][j]) {
                    int old = d[i][j];
                    d[i][j] = d[i][k] + d[k][j];
                    addStep(QString("  d[%1][%2] = %3 → %4 (经过%5)").arg(i).arg(j).arg(old).arg(d[i][j]).arg(k));
                }
            }
        }
    }

    m_resultText = "Floyd结果(最短路径矩阵):\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            m_resultText += QString("%1 ").arg(d[i][j]>=INF?"∞":QString::number(d[i][j]));
        m_resultText += "\n";
    }
}

void ShortestPath::run() {
    reset();
    m_mode = m_params.value("mode", "Dijkstra").toString();
    if (m_mode == "Dijkstra") runDijkstra();
    else runFloyd();
    m_running=false;
}

void ShortestPath::step() { if(m_stepIndex==0)run(); }
void ShortestPath::reset() { m_steps.clear();m_stepIndex=0;m_resultText.clear(); }
