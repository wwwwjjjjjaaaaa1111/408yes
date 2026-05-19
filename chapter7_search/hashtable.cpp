#include "hashtable.h"

HashTable::HashTable(QObject *parent) : AlgorithmBase(parent) {}

QVector<QString> HashTable::parameterNames() const { return {"method"}; }
QVector<QString> HashTable::parameterDescriptions() const { return {"冲突处理方法"}; }
QVector<QVariant> HashTable::parameterDefaults() const {
    return {QVariant(QVariantList{"线性探测", "二次探测", "链地址法"})};
}

void HashTable::runLinearProbing() {
    m_size = 11;
    QVector<int> table(m_size, -1);
    QVector<int> keys = {47, 7, 29, 11, 16, 92, 22, 8, 3};

    addStep(QString("线性探测法 (表长=%1). 哈希函数: h(k)=k mod %1").arg(m_size));
    for (int k : keys) {
        int h0 = k % m_size;
        int pos = h0, probe = 0;
        while (table[pos] != -1 && probe < m_size) {
            addStep(QString("插入%1: h(%1)=%2 冲突, 位置%3已占，线性探测位置%4").arg(k).arg(h0).arg(pos).arg((pos+1)%m_size));
            probe++;
            pos = (h0 + probe) % m_size;
        }
        if (probe < m_size) {
            table[pos] = k;
            addStep(QString("插入%1: 最终位置=%2 (探测%3次)").arg(k).arg(pos).arg(probe));
        } else {
            addStep(QString("插入%1: 表已满!").arg(k));
        }
    }

    QString tblStr = "散列表: ";
    for (int v : table) tblStr += (v >= 0 ? QString::number(v) : "_") + " ";
    addStep(tblStr);
    m_resultText = "线性探测完成\n" + tblStr;

    QVariantMap viz; viz["type"]="barchart"; viz["title"]="散列表(线性探测)";
    QVariantList vals, colors;
    for (int i = 0; i < m_size; i++) {
        vals.append(table[i] >= 0 ? table[i] : 0);
        colors.append(table[i] >= 0 ? "#64B5F6" : "#E0E0E0");
    }
    viz["values"]=vals; viz["colors"]=colors; m_vizData=viz;
}

void HashTable::runQuadraticProbing() {
    m_size = 11;
    QVector<int> table(m_size, -1);
    QVector<int> keys = {47, 7, 29, 11, 16, 92};

    addStep(QString("二次探测法 (表长=%1). h(k,i)=(h(k)+i²) mod %1").arg(m_size));
    for (int k : keys) {
        int h0 = k % m_size;
        int pos = h0, probe = 0;
        while (table[pos] != -1) {
            probe++;
            pos = (h0 + probe * probe) % m_size;
            addStep(QString("插入%1: h0=%2 冲突, 二次探测i=%3 → pos=%4").arg(k).arg(h0).arg(probe).arg(pos));
        }
        table[pos] = k;
        addStep(QString("插入%1: 最终位置=%2").arg(k).arg(pos));
    }

    QString tblStr = "散列表: ";
    for (int v : table) tblStr += (v >= 0 ? QString::number(v) : "_") + " ";
    addStep(tblStr);
    m_resultText = "二次探测完成\n" + tblStr;
}

void HashTable::runChaining() {
    m_size = 7;
    QVector<QVector<int>> chains(m_size);
    QVector<int> keys = {15, 22, 37, 44, 51, 66, 73};

    addStep(QString("链地址法 (表长=%1). h(k)=k mod %1").arg(m_size));
    for (int k : keys) {
        int h = k % m_size;
        chains[h].append(k);
        addStep(QString("插入%1: h(%1)=%2, 链入位置%2 (链长=%3)").arg(k).arg(h).arg(chains[h].size()));
    }

    QString tblStr = "散列表(链地址):\n";
    for (int i = 0; i < m_size; i++) {
        tblStr += QString("  [%1] → ").arg(i);
        for (int v : chains[i]) tblStr += QString::number(v) + " → ";
        tblStr += chains[i].isEmpty() ? "NULL\n" : "NULL\n";
    }
    addStep(tblStr);
    m_resultText = "链地址法完成\n" + tblStr;
}

void HashTable::run() {
    reset();
    m_method = m_params.value("method", "线性探测").toString();

    if (m_method == "线性探测") runLinearProbing();
    else if (m_method == "二次探测") runQuadraticProbing();
    else runChaining();

    m_running=false;
}

void HashTable::step() { if(m_stepIndex==0)run(); }
void HashTable::reset() { m_steps.clear();m_stepIndex=0;m_resultText.clear();m_vizData.clear(); }
