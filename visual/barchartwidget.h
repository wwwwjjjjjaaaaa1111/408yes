#ifndef BARCHARTWIDGET_H
#define BARCHARTWIDGET_H

#include <QWidget>
#include <QVariantMap>
#include <QVector>

class BarChartWidget : public QWidget {
    Q_OBJECT
public:
    explicit BarChartWidget(QWidget *parent = nullptr);
    void setData(const QVariantMap &data);
    void clearData();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<int> m_values;
    QVector<int> m_highlights; // indices to highlight
    QVector<QColor> m_colors;
    QString m_title;
};

#endif // BARCHARTWIDGET_H
