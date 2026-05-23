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
        tn.isBTreeNode = nm.value("isBTreeNode", false).toBool();

        if (nm.contains("keys")) {
            QVariantList kl = nm.value("keys").toList();
            for (const auto &k : kl)
                tn.keys.append(k.toString());
        }

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

    QMap<int, QVector<int>> children;
    int rootId = -1;
    for (const auto &n : m_nodes) {
        if (n.parentId < 0) rootId = n.id;
        else children[n.parentId].append(n.id);
    }
    if (rootId < 0 && !m_nodes.isEmpty()) rootId = m_nodes[0].id;

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
        // B-tree nodes need more horizontal space per node
        double extraPerNode = 0;
        for (int nid : nodes) {
            for (const auto &nd : m_nodes) {
                if (nd.id == nid && nd.isBTreeNode) {
                    extraPerNode = qMax(extraPerNode, (double)(nd.keys.size() * 12));
                }
            }
        }
        double hSpacing = (double)(width() - 40 - extraPerNode * nodes.size()) / (nodes.size() + 1);
        hSpacing = qMax(hSpacing, 20.0);
        for (int j = 0; j < nodes.size(); j++) {
            xCoord[nodes[j]] = 20 + (j + 1) * (hSpacing + extraPerNode);
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

    QFont font = painter.font();
    font.setPointSize(9);
    painter.setFont(font);

    for (int i = 0; i < m_nodes.size(); i++) {
        QPointF pos = m_positions[i];

        if (m_nodes[i].isBTreeNode) {
            // Draw rounded rectangle for B-tree node
            double rw = m_nodeRadius * 0.9 + m_nodes[i].keys.size() * 12;
            double rh = m_nodeRadius * 1.6;
            QRectF nodeRect(pos.x() - rw, pos.y() - rh, rw * 2, rh * 2);

            painter.setBrush(m_nodes[i].color);
            painter.setPen(QPen(Qt::black, 1.5));
            painter.drawRoundedRect(nodeRect, 6, 6);

            // Draw key compartments separated by vertical lines
            if (m_nodes[i].keys.size() > 1) {
                double compartmentW = nodeRect.width() / m_nodes[i].keys.size();
                painter.setPen(QPen(QColor("#ccc"), 1));
                for (int k = 1; k < m_nodes[i].keys.size(); k++) {
                    double lx = nodeRect.left() + k * compartmentW;
                    painter.drawLine(QPointF(lx, nodeRect.top() + 3),
                                     QPointF(lx, nodeRect.bottom() - 3));
                }
            }

            // Draw key texts
            if (!m_nodes[i].keys.isEmpty()) {
                double cw = nodeRect.width() / m_nodes[i].keys.size();
                painter.setPen(Qt::black);
                for (int k = 0; k < m_nodes[i].keys.size(); k++) {
                    QRectF keyRect(nodeRect.left() + k * cw, nodeRect.top(),
                                   cw, nodeRect.height());
                    painter.drawText(keyRect, Qt::AlignCenter, m_nodes[i].keys[k]);
                }
            }
        } else {
            // Draw circle (original behavior)
            QRectF nodeRect(pos.x() - m_nodeRadius, pos.y() - m_nodeRadius,
                            m_nodeRadius * 2, m_nodeRadius * 2);

            painter.setBrush(m_nodes[i].color);
            painter.setPen(QPen(Qt::black, 1.5));
            painter.drawEllipse(nodeRect);
            painter.drawText(nodeRect, Qt::AlignCenter, m_nodes[i].label);
        }
    }

    painter.setPen(Qt::black);
    painter.drawText(QRect(0, 0, width(), 20), Qt::AlignCenter, m_title);
}
