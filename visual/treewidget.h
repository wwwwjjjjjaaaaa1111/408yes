#ifndef TREEWIDGET_H
#define TREEWIDGET_H

#include <QWidget>
#include <QVariantMap>
#include <QVector>
#include <QPointF>
#include <QColor>
#include <QStringList>

struct TreeNodeData {
    int id;
    QString label;
    int parentId;
    QColor color = Qt::white;
    bool isRed = false;
    QStringList keys;
    bool isBTreeNode = false;
};

class TreeWidget : public QWidget {
    Q_OBJECT
public:
    explicit TreeWidget(QWidget *parent = nullptr);
    void setData(const QVariantMap &data);
    void clearData();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void layoutTree();
    QVector<TreeNodeData> m_nodes;
    QVector<QPointF> m_positions;
    QVector<QPair<int,int>> m_edges;
    QString m_title;
    double m_nodeRadius = 20;
};

#endif // TREEWIDGET_H
