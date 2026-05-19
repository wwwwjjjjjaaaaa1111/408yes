#ifndef MST_H
#define MST_H

#include "algorithmbase.h"

class MST : public AlgorithmBase {
    Q_OBJECT
public:
    explicit MST(QObject *parent = nullptr);
    QString name() const override { return "最小生成树(Prim & Kruskal)"; }
    QString chapter() const override { return "第6章 图"; }
    QString description() const override { return "Prim算法和Kruskal算法求最小生成树，可切换"; }

    void run() override;
    void step() override;
    void reset() override;
    QVector<QString> parameterNames() const override;
    QVector<QString> parameterDescriptions() const override;
    QVector<QVariant> parameterDefaults() const override;

private:
    void runPrim();
    void runKruskal();
    QString m_mode = "Prim";
    int m_stepIndex = 0;
};

#endif
