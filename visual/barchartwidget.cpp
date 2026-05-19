#include "barchartwidget.h"
#include <QPainter>
#include <QFontMetrics>

BarChartWidget::BarChartWidget(QWidget *parent) : QWidget(parent)
{
    setMinimumSize(300, 200);
    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor("#fafafa"));
    setPalette(pal);
}

void BarChartWidget::clearData()
{
    m_values.clear();
    m_highlights.clear();
    m_colors.clear();
    m_title.clear();
    update();
}

void BarChartWidget::setData(const QVariantMap &data)
{
    m_title = data.value("title", "排序可视化").toString();
    QVariantList vals = data.value("values").toList();
    m_values.clear();
    for (const auto &v : vals) m_values.append(v.toInt());

    QVariantList hl = data.value("highlights").toList();
    m_highlights.clear();
    for (const auto &h : hl) m_highlights.append(h.toInt());

    m_colors.clear();
    QVariantList clrs = data.value("colors").toList();
    for (const auto &c : clrs) {
        m_colors.append(QColor(c.toString()));
    }

    update();
}

void BarChartWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int w = width();
    int h = height();
    int margin = 20;
    int barAreaH = h - 2 * margin - 30;

    painter.fillRect(rect(), QColor("#fafafa"));

    if (m_values.isEmpty()) {
        painter.drawText(rect(), Qt::AlignCenter, "无数据");
        return;
    }

    int maxVal = *std::max_element(m_values.begin(), m_values.end());
    if (maxVal == 0) maxVal = 1;
    int n = m_values.size();
    double barWidth = (double)(w - 2 * margin) / n;
    double barGap = barWidth * 0.15;
    double actualBarW = barWidth - barGap;

    for (int i = 0; i < n; i++) {
        double barH = (double)m_values[i] / maxVal * barAreaH;
        double x = margin + i * barWidth + barGap / 2;
        double y = h - margin - 30 - barH;

        QColor color = (i < m_colors.size()) ? m_colors[i] : QColor("#4CAF50");
        if (m_highlights.contains(i)) {
            color = QColor("#FF5722");
        }
        painter.fillRect(QRectF(x, y, actualBarW, barH), color);
        painter.setPen(Qt::black);
        painter.drawRect(QRectF(x, y, actualBarW, barH));

        if (n <= 50) {
            painter.drawText(QRectF(x, h - margin - 30, actualBarW, 25),
                             Qt::AlignCenter, QString::number(m_values[i]));
        }
    }

    painter.setPen(Qt::black);
    painter.drawText(QRect(0, 0, w, margin), Qt::AlignCenter, m_title);
}
