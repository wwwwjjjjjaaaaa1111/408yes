#include "criticalpath.h"
#include <QQueue>
#include <climits>

CriticalPath::CriticalPath(QObject *parent) : AlgorithmBase(parent) {}

void CriticalPath::run() {
    reset();
    int n = 6; // vertices represent events
    // edges with weights (activity durations)
    struct Edge { int from, to, weight; };
    QVector<Edge> edges = {
        {0,1,3},{0,2,2},{0,3,4},
        {1,4,5},{2,4,3},{3,4,4},
        {4,5,2}
    };

    addStep("===== 关键路径(AOE网) =====");
    addStep("活动及持续时间:");
    for(const auto &e:edges)
        addStep(QString("  活动%1→%2: 持续时间=%3").arg(e.from).arg(e.to).arg(e.weight));

    // Build adjacency
    QVector<QVector<Edge>> adj(n);
    QVector<int> indegree(n, 0);
    for(const auto &e:edges){
        adj[e.from].append(e);
        indegree[e.to]++;
    }

    // Forward pass: earliest start time (ve)
    QVector<int> ve(n, 0);
    QQueue<int> q;
    for(int i=0;i<n;i++) if(indegree[i]==0) q.enqueue(i);

    QVector<int> topo;
    while(!q.isEmpty()){
        int v=q.dequeue(); topo.append(v);
        for(const auto &e:adj[v]){
            ve[e.to]=qMax(ve[e.to], ve[v]+e.weight);
            indegree[e.to]--;
            if(indegree[e.to]==0) q.enqueue(e.to);
        }
    }

    addStep("事件最早发生时间 ve:");
    for(int i=0;i<n;i++)
        addStep(QString("  ve[%1] = %2").arg(i).arg(ve[i]));

    // Backward pass: latest start time (vl)
    QVector<int> vl(n, ve[n-1]);
    for(int i=topo.size()-2;i>=0;i--){
        int v=topo[i];
        for(const auto &e:adj[v]){
            vl[v]=qMin(vl[v], vl[e.to]-e.weight);
        }
    }

    addStep("事件最晚发生时间 vl:");
    for(int i=0;i<n;i++)
        addStep(QString("  vl[%1] = %2").arg(i).arg(vl[i]));

    // Critical activities
    addStep("关键活动 (e==l，时间余量为0):");
    QString criticalActs;
    for(const auto &e:edges){
        int ee=ve[e.from];
        int el=vl[e.to]-e.weight;
        int slack=el-ee;
        if(slack==0){
            criticalActs+=QString(" %1→%2").arg(e.from).arg(e.to);
            addStep(QString("  活动%1→%2: e=%3, l=%4, 关键活动!").arg(e.from).arg(e.to).arg(ee).arg(el));
        } else {
            addStep(QString("  活动%1→%2: e=%3, l=%4, 松弛时间=%5").arg(e.from).arg(e.to).arg(ee).arg(el).arg(slack));
        }
    }

    m_resultText=QString("关键路径长度: %1\n关键活动:%2").arg(ve[n-1]).arg(criticalActs);

    QVariantMap viz;viz["type"]="graph";viz["title"]="AOE网关键路径";viz["directed"]=true;
    QVariantList nodes,vedges;
    for(int i=0;i<n;i++){QVariantMap nd;nd["id"]=i;nd["label"]=QString("V%1").arg(i);nodes.append(nd);}
    for(const auto &e:edges){
        QVariantMap ed;ed["from"]=e.from;ed["to"]=e.to;ed["weight"]=e.weight;
        // Check if critical
        if(ve[e.from]==vl[e.to]-e.weight) ed["highlight"]=true;
        vedges.append(ed);
    }
    viz["nodes"]=nodes;viz["edges"]=vedges;m_vizData=viz;
    m_running=false;
}

void CriticalPath::step() { if(m_stepIndex==0)run(); }
void CriticalPath::reset() { m_steps.clear();m_stepIndex=0;m_resultText.clear();m_vizData.clear(); }
