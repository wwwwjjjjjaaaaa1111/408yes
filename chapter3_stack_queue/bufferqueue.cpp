#include "bufferqueue.h"
#include <QQueue>

BufferQueue::BufferQueue(QObject *parent) : AlgorithmBase(parent) {}

QVector<QString> BufferQueue::parameterNames() const {
    return {"bufferSize", "produceCount"};
}
QVector<QString> BufferQueue::parameterDescriptions() const {
    return {"缓冲区大小", "生产数据量"};
}
QVector<QVariant> BufferQueue::parameterDefaults() const {
    return {QVariant(5), QVariant(10)};
}

void BufferQueue::run() {
    reset();
    m_bufferSize = m_params.value("bufferSize", 5).toInt();
    m_produceCount = m_params.value("produceCount", 10).toInt();

    QQueue<int> buffer;
    addStep(QString("模拟缓冲队列: 缓冲区大小=%1，总数据量=%2").arg(m_bufferSize).arg(m_produceCount));
    addStep("--- 生产者-消费者模型 ---");

    int consumed = 0;
    for (int i = 0; i < m_produceCount; i++) {
        if (buffer.size() >= m_bufferSize) {
            int item = buffer.dequeue();
            consumed++;
            addStep(QString("缓冲区满，消费者取出: %1 (已消费%2个)").arg(item).arg(consumed));
        }
        buffer.enqueue(i + 1);
        addStep(QString("生产者放入: %1，缓冲区占用: %2/%3")
                .arg(i + 1).arg(buffer.size()).arg(m_bufferSize));
    }

    while (!buffer.isEmpty()) {
        int item = buffer.dequeue();
        consumed++;
        addStep(QString("消费者取出剩余: %1 (已消费%2个)").arg(item).arg(consumed));
    }

    m_resultText = QString("模拟完成: 生产%1个，消费%2个，缓冲区最大容量%3")
        .arg(m_produceCount).arg(consumed).arg(m_bufferSize);
    m_running = false;
}

void BufferQueue::step() { if (m_stepIndex == 0) run(); }
void BufferQueue::reset() { m_steps.clear(); m_stepIndex = 0; m_resultText.clear(); }
