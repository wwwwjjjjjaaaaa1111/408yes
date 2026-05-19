#ifndef BUFFERQUEUE_H
#define BUFFERQUEUE_H

#include "algorithmbase.h"

class BufferQueue : public AlgorithmBase {
    Q_OBJECT
public:
    explicit BufferQueue(QObject *parent = nullptr);
    QString name() const override { return "队列在计算机系统中的应用"; }
    QString chapter() const override { return "第3章 栈、队列和数组"; }
    QString description() const override { return "模拟操作系统中的缓冲队列(生产者-消费者)"; }

    void run() override;
    void step() override;
    void reset() override;

    QVector<QString> parameterNames() const override;
    QVector<QString> parameterDescriptions() const override;
    QVector<QVariant> parameterDefaults() const override;

private:
    int m_bufferSize = 5;
    int m_produceCount = 8;
    int m_stepIndex = 0;
};

#endif // BUFFERQUEUE_H
