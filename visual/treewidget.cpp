#include "treewidget.h"
#include <QPainter>
#include <QToolTip>
#include <QtMath>

TreeWidget::TreeWidget(QWidget *parent) : QWidget(parent)
{
    setMinimumSize(300, 250);
    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor("#fafafa"));
    setPalette(pal);
}

void TreeWidget::clearData()
{
    m_nodes.clear();
    m_edges.clear();
    m_positions.clear();
    m_title.clear();
    update();
}

void TreeWidget::setData(const QVariantMap &data)
{
    m_title = data.value("title", "树形结构").toString();
    m_nodes.clear();
    m_edges.clear();

    QVariantList nodeList = data.value("nodes").toList();
    for (const auto &n : nodeList) {
        QVariantMap nm = n.toMap();
        TreeNodeData tn;
        tn.id = nm.value("id").toInt();
        tn.label = nm.value("label").toString();
        tn.parentId = nm.value("parentId", -1).toInt();
        tn.isRed = nm.value("isRed", false).toBool();
        tn.color = tn.isRed ? QColor("#ff6b6b") : QColor("#e8f5e9");
        if (nm.contains("color")) {
            tn.color = QColor(nm.value("color").toString());
        }
        m_nodes.append(tn);
        if (tn.parentId >= 0) {
            m_edges.append({tn.parentId, tn.id});
        }
    }

    layoutTree();
    update();
}

void TreeWidget::layoutTree()
{
    m_positions.clear();
    if (m_nodes.isEmpty()) return;

    // Build adjacency from edges
    QMap<int, QVector<int>> children;
    int rootId = -1;
    for (const auto &n : m_nodes) {
        if (n.parentId < 0) rootId = n.id;
        else children[n.parentId].append(n.id);
    }
    if (rootId < 0 && !m_nodes.isEmpty()) rootId = m_nodes[0].id;

    // BFS level order, then spread horizontally
    QMap<int, int> depth;
    QMap<int, double> xCoord;
    QVector<int> bfs = {rootId};
    depth[rootId] = 0;
    QMap<int, QVector<int>> levelNodes;

    for (int i = 0; i < bfs.size(); i++) {
        int cur = bfs[i];
        levelNodes[depth[cur]].append(cur);
        for (int child : children[cur]) {
            depth[child] = depth[cur] + 1;
            bfs.append(child);
        }
    }

    int maxDepth = levelNodes.keys().isEmpty() ? 0 : levelNodes.keys().last();
    double vSpacing = (double)(height() - 60) / qMax(maxDepth + 1, 1);
    double nodeRadius = vSpacing * 0.28;
    m_nodeRadius = qMax(15.0, qMin(nodeRadius, 28.0));

    for (auto it = levelNodes.begin(); it != levelNodes.end(); ++it) {
        int d = it.key();
        auto &nodes = it.value();
        double hSpacing = (double)(width() - 40) / (nodes.size() + 1);
        for (int j = 0; j < nodes.size(); j++) {
            xCoord[nodes[j]] = 20 + (j + 1) * hSpacing;
        }
    }

    m_positions.resize(m_nodes.size());
    for (int i = 0; i < m_nodes.size(); i++) {
        int id = m_nodes[i].id;
        double x = xCoord.value(id, width() / 2.0);
        double y = 35 + depth.value(id, 0) * vSpacing;
        m_positions[i] = QPointF(x, y);
    }
}

void TreeWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), QColor("#fafafa"));

    if (m_nodes.isEmpty()) {
        painter.drawText(rect(), Qt::AlignCenter, "无树形数据");
        return;
    }

    // Draw edges
    painter.setPen(QPen(Qt::darkGray, 2));
    for (const auto &edge : m_edges) {
        QPointF p1, p2;
        for (int i = 0; i < m_nodes.size(); i++) {
            if (m_nodes[i].id == edge.first) p1 = m_positions[i];
            if (m_nodes[i].id == edge.second) p2 = m_positions[i];
        }
        painter.drawLine(p1, p2);
    }

    // Draw nodes
    QFont font = painter.font();
    font.setPointSize(9);
    painter.setFont(font);

    for (int i = 0; i < m_nodes.size(); i++) {
        QPointF pos = m_positions[i];
        QRectF nodeRect(pos.x() - m_nodeRadius, pos.y() - m_nodeRadius,
                        m_nodeRadius * 2, m_nodeRadius * 2);

        painter.setBrush(m_nodes[i].color);
        painter.setPen(QPen(Qt::black, 1.5));
        painter.drawEllipse(nodeRect);
        painter.drawText(nodeRect, Qt::AlignCenter, m_nodes[i].label);
    }

    painter.setPen(Qt::black);
    painter.drawText(QRect(0, 0, width(), 20), Qt::AlignCenter, m_title);
}
