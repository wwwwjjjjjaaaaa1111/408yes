#ifndef HASHTABLE_H
#define HASHTABLE_H
#include "algorithmbase.h"

class HashTable : public AlgorithmBase {
    Q_OBJECT
public:
    explicit HashTable(QObject *parent = nullptr);
    QString name() const override { return "散列表(Hash)"; }
    QString chapter() const override { return "第7章 查找"; }
    QString description() const override { return "开放定址法(线性探测/二次探测)和链地址法, 可调参数"; }

    void run() override; void step() override; void reset() override;
    QVector<QString> parameterNames() const override;
    QVector<QString> parameterDescriptions() const override;
    QVector<QVariant> parameterDefaults() const override;

private:
    void runLinearProbing();
    void runQuadraticProbing();
    void runChaining();
    QString m_method = "线性探测";
    int m_size = 11;
    int m_stepIndex = 0;
};

#endif
