#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QWidget>
#include <QVariantMap>
#include <QVector>
#include <QPointF>
#include <QColor>

struct GraphEdge {
    int from;
    int to;
    int weight;
    QColor color = Qt::darkGray;
    bool highlight = false;
};

struct GraphNode {
    int id;
    QString label;
    QPointF position;
    QColor color = QColor("#64B5F6");
};

class GraphWidget : public QWidget {
    Q_OBJECT
public:
    explicit GraphWidget(QWidget *parent = nullptr);
    void setData(const QVariantMap &data);
    void clearData();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QVector<GraphNode> m_nodes;
    QVector<GraphEdge> m_edges;
    QString m_title;
    bool m_directed = false;
    int m_dragIdx = -1;
    static constexpr double NODE_R = 22;
};

#endif // GRAPHWIDGET_H
