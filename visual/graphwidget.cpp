#include "graphwidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QtMath>

GraphWidget::GraphWidget(QWidget *parent) : QWidget(parent)
{
    setMinimumSize(350, 280);
    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor("#fafafa"));
    setPalette(pal);
}

void GraphWidget::clearData()
{
    m_nodes.clear();
    m_edges.clear();
    m_title.clear();
    update();
}

void GraphWidget::setData(const QVariantMap &data)
{
    m_title = data.value("title", "图结构").toString();
    m_directed = data.value("directed", false).toBool();
    m_nodes.clear();
    m_edges.clear();

    QVariantList nodeList = data.value("nodes").toList();
    int w = width(), h = height();
    double cx = w / 2.0, cy = h / 2.0;
    double r = qMin(w, h) / 2.5;

    for (int i = 0; i < nodeList.size(); i++) {
        QVariantMap nm = nodeList[i].toMap();
        GraphNode gn;
        gn.id = nm.value("id").toInt();
        gn.label = nm.value("label").toString();
        double angle = 2 * M_PI * i / nodeList.size() - M_PI / 2;
        gn.position = QPointF(cx + r * cos(angle), cy + r * sin(angle));
        if (nm.contains("color")) gn.color = QColor(nm.value("color").toString());
        m_nodes.append(gn);
    }

    QVariantList edgeList = data.value("edges").toList();
    for (const auto &e : edgeList) {
        QVariantMap em = e.toMap();
        GraphEdge ge;
        ge.from = em.value("from").toInt();
        ge.to = em.value("to").toInt();
        ge.weight = em.value("weight", 1).toInt();
        ge.highlight = em.value("highlight", false).toBool();
        ge.color = ge.highlight ? QColor("#FF5722") : QColor("#666");
        m_edges.append(ge);
    }

    update();
}

void GraphWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), QColor("#fafafa"));

    if (m_nodes.isEmpty()) {
        painter.drawText(rect(), Qt::AlignCenter, "无图数据");
        return;
    }

    // Draw edges
    for (const auto &edge : m_edges) {
        QPointF pFrom, pTo;
        for (const auto &n : m_nodes) {
            if (n.id == edge.from) pFrom = n.position;
            if (n.id == edge.to) pTo = n.position;
        }
        painter.setPen(QPen(edge.color, edge.highlight ? 2.5 : 1.5));
        painter.drawLine(pFrom, pTo);

        // Arrow for directed
        if (m_directed) {
            double angle = atan2(pTo.y() - pFrom.y(), pTo.x() - pFrom.x());
            QPointF arrowBase = pTo - QPointF(NODE_R * 1.1 * cos(angle), NODE_R * 1.1 * sin(angle));
            QPointF arrowP1 = arrowBase - QPointF(10 * cos(angle - 0.5), 10 * sin(angle - 0.5));
            QPointF arrowP2 = arrowBase - QPointF(10 * cos(angle + 0.5), 10 * sin(angle + 0.5));
            QPolygonF arrow;
            arrow << pTo - QPointF(NODE_R * cos(angle), NODE_R * sin(angle)) << arrowP1 << arrowP2;
            painter.setBrush(edge.color);
            painter.drawPolygon(arrow);
        }

        // Weight label
        if (edge.weight != 1 || m_directed) {
            QPointF mid = (pFrom + pTo) / 2;
            painter.setPen(Qt::blue);
            QFont f = painter.font();
            f.setPointSize(9);
            painter.setFont(f);
            painter.drawText(QRectF(mid.x() - 15, mid.y() - 15, 30, 20),
                             Qt::AlignCenter, QString::number(edge.weight));
        }
    }

    // Draw nodes
    QFont font = painter.font();
    font.setPointSize(10);
    painter.setFont(font);
    for (const auto &n : m_nodes) {
        QRectF nr(n.position.x() - NODE_R, n.position.y() - NODE_R, NODE_R * 2, NODE_R * 2);
        painter.setBrush(n.color);
        painter.setPen(QPen(Qt::black, 1.5));
        painter.drawEllipse(nr);
        painter.drawText(nr, Qt::AlignCenter, n.label);
    }

    painter.setPen(Qt::black);
    painter.drawText(QRect(0, 0, width(), 20), Qt::AlignCenter, m_title);
}

void GraphWidget::mousePressEvent(QMouseEvent *event)
{
    for (int i = 0; i < m_nodes.size(); i++) {
        QPointF d = event->pos() - m_nodes[i].position;
        if (sqrt(d.x() * d.x() + d.y() * d.y()) < NODE_R + 5) {
            m_dragIdx = i;
            break;
        }
    }
}

void GraphWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragIdx >= 0) {
        m_nodes[m_dragIdx].position = event->pos();
        update();
    }
}

void GraphWidget::mouseReleaseEvent(QMouseEvent *)
{
    m_dragIdx = -1;
}
