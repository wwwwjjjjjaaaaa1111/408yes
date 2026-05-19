#ifndef CRITICALPATH_H
#define CRITICALPATH_H

#include "algorithmbase.h"

class CriticalPath : public AlgorithmBase {
    Q_OBJECT
public:
    explicit CriticalPath(QObject *parent = nullptr);
    QString name() const override { return "关键路径"; }
    QString chapter() const override { return "第6章 图"; }
    QString description() const override { return "AOE网的关键路径，显示事件最早/最晚发生时间及关键活动"; }

    void run() override;
    void step() override;
    void reset() override;
    QVector<QString> parameterNames() const override { return {}; }
    QVector<QString> parameterDescriptions() const override { return {}; }
    QVector<QVariant> parameterDefaults() const override { return {}; }
private:
    int m_stepIndex = 0;
};

#endif
