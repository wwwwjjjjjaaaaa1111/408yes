#ifndef ALGORITHMBASE_H
#define ALGORITHMBASE_H

#include <QString>
#include <QVector>
#include <QVariantMap>
#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>

struct StepInfo {
    QString description;
    QVariantMap data;
    qint64 timestamp;
};

class AlgorithmBase : public QObject {
    Q_OBJECT
public:
    explicit AlgorithmBase(QObject *parent = nullptr) : QObject(parent), m_running(false) {}
    virtual ~AlgorithmBase() {}

    virtual QString name() const = 0;
    virtual QString chapter() const = 0;
    virtual QString description() const = 0;

    virtual void setParams(const QVariantMap &params) { m_params = params; }
    virtual QVariantMap params() const { return m_params; }

    virtual void run() = 0;
    virtual void step() = 0;
    virtual void reset() = 0;

    virtual QVector<StepInfo> steps() const { return m_steps; }
    virtual QString resultText() const { return m_resultText; }
    virtual QVariantMap visualizationData() const { return m_vizData; }

    // Per-step visualization snapshots for step-by-step animation
    int snapshotCount() const { return m_snapshots.size(); }
    QVariantMap snapshotAt(int index) const {
        if (index < 0 || index >= m_snapshots.size()) return {};
        return m_snapshots[index];
    }

    bool isRunning() const { return m_running; }

    virtual QVector<QString> parameterNames() const = 0;
    virtual QVector<QString> parameterDescriptions() const = 0;
    virtual QVector<QVariant> parameterDefaults() const = 0;

signals:
    void stepCompleted(int stepIndex);
    void finished();
    void progressUpdated(int percent);

protected:
    QVariantMap m_params;
    QVector<StepInfo> m_steps;
    QVector<QVariantMap> m_snapshots; // per-step visualization data
    QString m_resultText;
    QVariantMap m_vizData;
    bool m_running = false;

    void addStep(const QString &desc, const QVariantMap &data = {}) {
        StepInfo step;
        step.description = desc;
        step.data = data;
        step.timestamp = m_steps.size();
        m_steps.append(step);
    }

    void recordSnapshot(const QVariantMap &data) {
        m_snapshots.append(data);
    }

    static QVariantMap makeBarchartSnapshot(const QVector<int> &arr, const QString &title,
                                             const QVector<int> &highlights = {}) {
        QVariantMap snap;
        snap["type"] = "barchart";
        snap["title"] = title;
        QVariantList vals, colors, hlList;
        for (int i = 0; i < arr.size(); i++) {
            vals.append(arr[i]);
            colors.append(highlights.contains(i) ? "#FF5722" : "#4CAF50");
        }
        for (int h : highlights) hlList.append(h);
        snap["values"] = vals;
        snap["colors"] = colors;
        snap["highlights"] = hlList;
        return snap;
    }
};

class AlgorithmWorker : public QThread {
    Q_OBJECT
public:
    explicit AlgorithmWorker(AlgorithmBase *algo, QObject *parent = nullptr)
        : QThread(parent), m_algo(algo) {}

    void run() override {
        if (m_algo) {
            m_algo->run();
            emit finished();
        }
    }

    void requestStop() { m_stopRequested = true; }

signals:
    void finished();

private:
    AlgorithmBase *m_algo;
    bool m_stopRequested = false;
};

#endif // ALGORITHMBASE_H
