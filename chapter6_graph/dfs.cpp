#include "dfs.h"

DFS::DFS(QObject *parent) : AlgorithmBase(parent) {}

QVector<QString> DFS::parameterNames() const { return {"startVertex"}; }
QVector<QString> DFS::parameterDescriptions() const { return {"起始顶点编号"}; }
QVector<QVariant> DFS::parameterDefaults() const { return {QVariant(0)}; }

void DFS::dfsRecursive(int v, QVector<bool> &visited, const QVector<QVector<int>> &graph, QString &order) {
    visited[v] = true;
    order += QString::number(v) + " ";
    addStep(QString("DFS访问顶点: %1").arg(v));
    for (int u = 0; u < graph.size(); u++) {
        if (graph[v][u] && !visited[u]) {
            addStep(QString("  从%1探索邻接点%2").arg(v).arg(u));
            dfsRecursive(u, visited, graph, order);
        }
    }
    addStep(QString("顶点%1 回溯").arg(v));
}

void DFS::run() {
    reset();
    m_startVertex = m_params.value("startVertex", 0).toInt();

    QVector<QVector<int>> graph(6, QVector<int>(6, 0));
    // Same graph as BFS
    graph[0][1]=1;graph[1][0]=1;graph[0][3]=1;graph[3][0]=1;
    graph[1][2]=1;graph[2][1]=1;graph[1][4]=1;graph[4][1]=1;
    graph[2][5]=1;graph[5][2]=1;graph[3][4]=1;graph[4][3]=1;graph[4][5]=1;graph[5][4]=1;

    int n = graph.size();
    m_startVertex = qBound(0, m_startVertex, n-1);
    addStep(QString("从顶点 %1 开始DFS").arg(m_startVertex));

    QVector<bool> visited(n, false);
    QString order;
    dfsRecursive(m_startVertex, visited, graph, order);

    // Check for disconnected components
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            addStep(QString("发现未连通分量，从顶点%1继续").arg(i));
            dfsRecursive(i, visited, graph, order);
        }
    }

    m_resultText = QString("DFS遍历顺序: %1").arg(order.trimmed());

    QVariantMap viz; viz["type"]="graph"; viz["title"]="DFS遍历";
    QVariantList nodes, edges;
    for(int i=0;i<n;i++){QVariantMap nd;nd["id"]=i;nd["label"]=QString::number(i);nodes.append(nd);}
    for(int i=0;i<n;i++)for(int j=i+1;j<n;j++)if(graph[i][j]){QVariantMap ed;ed["from"]=i;ed["to"]=j;edges.append(ed);}
    viz["nodes"]=nodes;viz["edges"]=edges;m_vizData=viz;
    m_running=false;
}

void DFS::step() { if(m_stepIndex==0)run(); }
void DFS::reset() { m_steps.clear();m_stepIndex=0;m_resultText.clear();m_vizData.clear(); }
