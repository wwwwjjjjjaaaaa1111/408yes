#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QStackedWidget>
#include <QSplitter>
#include <QLabel>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>
#include <QSlider>
#include <QLineEdit>
#include <QGroupBox>
#include <QTableWidget>
#include <QScrollArea>
#include <QTimer>
#include <QElapsedTimer>

#include "algorithmbase.h"
#include "visual/barchartwidget.h"
#include "visual/treewidget.h"
#include "visual/graphwidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAlgorithmSelected(QTreeWidgetItem *item, int column);
    void onRunClicked();
    void onStepClicked();
    void onResetClicked();
    void onParamChanged();
    void onStepCompleted(int stepIndex);
    void onAlgorithmFinished();
    void onAnimTimerTick();

private:
    void setupUI();
    void setupAlgorithmTree();
    void clearParameterPanel();
    void buildParameterPanel(AlgorithmBase *algo);
    void updateLog();
    void updateVisualization();
    void showSnapshot(int index);
    void setControlsEnabled(bool enabled);

    QTreeWidget *m_treeWidget;
    QStackedWidget *m_vizStack;
    QScrollArea *m_paramScroll;
    QWidget *m_paramPanel;
    QVBoxLayout *m_paramLayout;
    QTextBrowser *m_logBrowser;
    QPushButton *m_runBtn;
    QPushButton *m_stepBtn;
    QPushButton *m_resetBtn;
    QLabel *m_statusLabel;
    QLabel *m_timeLabel;

    // Visualization widgets
    BarChartWidget *m_barChart;
    TreeWidget *m_treeViz;
    GraphWidget *m_graphViz;
    QWidget *m_defaultViz;

    AlgorithmBase *m_currentAlgo = nullptr;
    AlgorithmWorker *m_worker = nullptr;
    int m_currentStep = 0;
    QElapsedTimer m_timer;

    // Animation
    QTimer *m_animTimer;
    bool m_stepMode = false; // true = user clicking step, false = auto animation

    // Parameter widgets mapping
    QVector<QWidget*> m_paramWidgets;
};

#endif // MAINWINDOW_H
