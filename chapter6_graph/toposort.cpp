#include "toposort.h"
#include <QQueue>

TopoSort::TopoSort(QObject *parent) : AlgorithmBase(parent) {}

void TopoSort::run() {
    reset();
    int n = 6;
    // Directed acyclic graph
    QVector<QVector<int>> g(n, QVector<int>(n, 0));
    g[0][1]=1; g[0][2]=1; g[0][3]=1;
    g[1][4]=1; g[2][3]=1; g[2][4]=1;
    g[3][5]=1; g[4][5]=1;

    addStep("AOV网: 6门课程的先修关系");
    addStep("  0→1, 0→2, 0→3");
    addStep("  1→4, 2→3, 2→4");
    addStep("  3→5, 4→5");

    QVector<int> indegree(n, 0);
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            if(g[i][j]) indegree[j]++;

    addStep("各顶点入度: [" + [&](){QString s;for(int i=0;i<n;i++)s+=QString::number(indegree[i])+" ";return s.trimmed();}() + "]");

    QQueue<int> q;
    for(int i=0;i<n;i++)
        if(indegree[i]==0) {
            q.enqueue(i);
            addStep(QString("入度为0的顶点%1入队").arg(i));
        }

    QString order;
    int count=0;
    while(!q.isEmpty()){
        int v=q.dequeue();
        order+=QString::number(v)+" ";
        count++;
        addStep(QString("输出顶点%1 (序列: %2)").arg(v).arg(order.trimmed()));
        for(int u=0;u<n;u++){
            if(g[v][u]){
                indegree[u]--;
                addStep(QString("  删除边%1→%2，顶点%3入度减为%4").arg(v).arg(u).arg(u).arg(indegree[u]));
                if(indegree[u]==0){
                    q.enqueue(u);
                    addStep(QString("  顶点%1入度为0，入队").arg(u));
                }
            }
        }
    }

    if(count<n){
        m_resultText=QString("图中存在环! 仅输出了%1/%2个顶点").arg(count).arg(n);
    } else {
        m_resultText=QString("拓扑序列: %1").arg(order.trimmed());
    }

    QVariantMap viz;viz["type"]="graph";viz["title"]="拓扑排序";viz["directed"]=true;
    QVariantList nodes,edges;
    for(int i=0;i<n;i++){QVariantMap nd;nd["id"]=i;nd["label"]=QString::number(i);nodes.append(nd);}
    for(int i=0;i<n;i++)for(int j=0;j<n;j++)if(g[i][j]){QVariantMap ed;ed["from"]=i;ed["to"]=j;edges.append(ed);}
    viz["nodes"]=nodes;viz["edges"]=edges;m_vizData=viz;
    m_running=false;
}

void TopoSort::step() { if(m_stepIndex==0)run(); }
void TopoSort::reset() { m_steps.clear();m_stepIndex=0;m_resultText.clear();m_vizData.clear(); }
