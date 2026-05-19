#include "mainwindow.h"
#include "algorithmfactory.h"
#include <QApplication>
#include <QHeaderView>
#include <QScrollArea>
#include <QFrame>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("数据结构考研核心算法演示系统");
    resize(1400, 900);

    m_animTimer = new QTimer(this);
    m_animTimer->setInterval(300);
    connect(m_animTimer, &QTimer::timeout, this, &MainWindow::onAnimTimerTick);

    setupUI();
    setupAlgorithmTree();
}

MainWindow::~MainWindow()
{
    m_animTimer->stop();
    if (m_worker && m_worker->isRunning()) {
        m_worker->requestStop();
        m_worker->wait(3000);
    }
}

void MainWindow::setupUI()
{
    auto *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    auto *mainLayout = new QHBoxLayout(centralWidget);

    auto *splitter = new QSplitter(Qt::Horizontal);

    // Left panel: algorithm tree
    auto *leftPanel = new QWidget;
    auto *leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(4, 4, 4, 4);
    auto *treeLabel = new QLabel("<b>算法列表</b>");
    m_treeWidget = new QTreeWidget;
    m_treeWidget->setHeaderHidden(true);
    m_treeWidget->setMinimumWidth(260);
    leftLayout->addWidget(treeLabel);
    leftLayout->addWidget(m_treeWidget);
    splitter->addWidget(leftPanel);

    // Right panel: demo area
    auto *rightPanel = new QWidget;
    auto *rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(4, 4, 4, 4);

    // Top: parameter area
    auto *paramGroup = new QGroupBox("参数设置");
    m_paramScroll = new QScrollArea;
    m_paramScroll->setWidgetResizable(true);
    m_paramScroll->setMaximumHeight(200);
    m_paramPanel = new QWidget;
    m_paramLayout = new QVBoxLayout(m_paramPanel);
    m_paramLayout->setContentsMargins(8, 8, 8, 8);
    m_paramScroll->setWidget(m_paramPanel);
    auto *paramGroupLayout = new QVBoxLayout(paramGroup);
    paramGroupLayout->addWidget(m_paramScroll);
    rightLayout->addWidget(paramGroup);

    // Control buttons
    auto *btnLayout = new QHBoxLayout;
    m_runBtn = new QPushButton("开始演示");
    m_stepBtn = new QPushButton("逐步执行");
    m_resetBtn = new QPushButton("重置");
    m_statusLabel = new QLabel("就绪");
    m_timeLabel = new QLabel("耗时: --");
    m_runBtn->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; padding: 6px 16px; font-weight: bold; }");
    m_stepBtn->setStyleSheet("QPushButton { background-color: #2196F3; color: white; padding: 6px 16px; }");
    m_resetBtn->setStyleSheet("QPushButton { background-color: #f44336; color: white; padding: 6px 16px; }");
    btnLayout->addWidget(m_runBtn);
    btnLayout->addWidget(m_stepBtn);
    btnLayout->addWidget(m_resetBtn);
    btnLayout->addStretch();
    btnLayout->addWidget(m_timeLabel);
    btnLayout->addWidget(m_statusLabel);
    rightLayout->addLayout(btnLayout);

    // Middle: visualization + log
    auto *vizLogSplitter = new QSplitter(Qt::Vertical);

    auto *vizGroup = new QGroupBox("可视化区域");
    auto *vizLayout = new QVBoxLayout(vizGroup);
    m_vizStack = new QStackedWidget;

    m_defaultViz = new QWidget;
    m_defaultViz->setAutoFillBackground(true);
    QPalette defPal = m_defaultViz->palette();
    defPal.setColor(QPalette::Window, QColor("#fafafa"));
    m_defaultViz->setPalette(defPal);
    auto *defaultVizLayout = new QVBoxLayout(m_defaultViz);
    auto *placeholder = new QLabel("请在左侧选择算法，设置参数后点击\"开始演示\"");
    placeholder->setAlignment(Qt::AlignCenter);
    placeholder->setStyleSheet("color: #888; font-size: 16px;");
    defaultVizLayout->addWidget(placeholder);

    m_barChart = new BarChartWidget;
    m_treeViz = new TreeWidget;
    m_graphViz = new GraphWidget;

    m_vizStack->addWidget(m_defaultViz);
    m_vizStack->addWidget(m_barChart);
    m_vizStack->addWidget(m_treeViz);
    m_vizStack->addWidget(m_graphViz);
    m_vizStack->setCurrentIndex(0);

    vizLayout->addWidget(m_vizStack);
    vizLogSplitter->addWidget(vizGroup);

    auto *logGroup = new QGroupBox("运行日志");
    auto *logLayout = new QVBoxLayout(logGroup);
    m_logBrowser = new QTextBrowser;
    m_logBrowser->setMinimumHeight(150);
    m_logBrowser->setStyleSheet("font-family: Consolas, 'Microsoft YaHei'; font-size: 13px;");
    logLayout->addWidget(m_logBrowser);
    vizLogSplitter->addWidget(logGroup);

    vizLogSplitter->setStretchFactor(0, 2);
    vizLogSplitter->setStretchFactor(1, 1);

    rightLayout->addWidget(vizLogSplitter);
    splitter->addWidget(rightPanel);

    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);
    splitter->setSizes({280, 1100});

    mainLayout->addWidget(splitter);

    connect(m_treeWidget, &QTreeWidget::itemClicked, this, &MainWindow::onAlgorithmSelected);
    connect(m_runBtn, &QPushButton::clicked, this, &MainWindow::onRunClicked);
    connect(m_stepBtn, &QPushButton::clicked, this, &MainWindow::onStepClicked);
    connect(m_resetBtn, &QPushButton::clicked, this, &MainWindow::onResetClicked);

    setControlsEnabled(false);
}

void MainWindow::setupAlgorithmTree()
{
    auto categories = AlgorithmFactory::categories();
    for (const auto &cat : categories) {
        auto *catItem = new QTreeWidgetItem(m_treeWidget);
        catItem->setText(0, cat.name);
        catItem->setFlags(catItem->flags() & ~Qt::ItemIsSelectable);
        QFont catFont = catItem->font(0);
        catFont.setBold(true);
        catFont.setPointSize(10);
        catItem->setFont(0, catFont);

        for (const auto &algoName : cat.algorithmNames) {
            auto *algoItem = new QTreeWidgetItem(catItem);
            algoItem->setText(0, algoName);
        }
    }
    m_treeWidget->expandAll();
}

void MainWindow::onAlgorithmSelected(QTreeWidgetItem *item, int)
{
    if (!item || item->parent() == nullptr) return;

    m_animTimer->stop();
    QString algoName = item->text(0);
    if (m_currentAlgo) {
        if (m_worker && m_worker->isRunning()) {
            m_worker->requestStop();
            m_worker->wait(3000);
        }
        delete m_currentAlgo;
        m_currentAlgo = nullptr;
    }

    m_currentAlgo = AlgorithmFactory::create(algoName, this);
    if (!m_currentAlgo) return;

    m_barChart->clearData();
    m_treeViz->clearData();
    m_graphViz->clearData();
    m_vizStack->setCurrentWidget(m_defaultViz);

    m_statusLabel->setText("已选择: " + algoName);
    m_logBrowser->clear();
    m_logBrowser->append("<b>算法:</b> " + m_currentAlgo->name());
    m_logBrowser->append("<b>分类:</b> " + m_currentAlgo->chapter());
    m_logBrowser->append("<b>简介:</b> " + m_currentAlgo->description());
    m_logBrowser->append("");

    buildParameterPanel(m_currentAlgo);
    connect(m_currentAlgo, &AlgorithmBase::stepCompleted, this, &MainWindow::onStepCompleted);
    connect(m_currentAlgo, &AlgorithmBase::finished, this, &MainWindow::onAlgorithmFinished);

    setControlsEnabled(true);
    m_stepBtn->setEnabled(true);
}

void MainWindow::clearParameterPanel()
{
    // Delete the old panel entirely and create a new one to avoid widget overlap
    delete m_paramPanel;
    m_paramPanel = new QWidget;
    m_paramLayout = new QVBoxLayout(m_paramPanel);
    m_paramLayout->setContentsMargins(8, 8, 8, 8);
    m_paramScroll->setWidget(m_paramPanel);
    m_paramWidgets.clear();
}

void MainWindow::buildParameterPanel(AlgorithmBase *algo)
{
    clearParameterPanel();
    auto names = algo->parameterNames();
    auto descs = algo->parameterDescriptions();
    auto defaults = algo->parameterDefaults();

    QVariantMap params;
    for (int i = 0; i < names.size(); i++) {
        auto *row = new QHBoxLayout;
        auto *label = new QLabel(descs[i] + ":");
        label->setMinimumWidth(120);
        row->addWidget(label);

        QVariant defVal = (i < defaults.size()) ? defaults[i] : QVariant();

        if (defVal.type() == QVariant::Int || defVal.type() == QVariant::Double) {
            auto *spin = new QSpinBox;
            spin->setRange(1, 10000);
            spin->setValue(defVal.toInt());
            spin->setObjectName(names[i]);
            connect(spin, QOverload<int>::of(&QSpinBox::valueChanged),
                    this, &MainWindow::onParamChanged);
            row->addWidget(spin);
            m_paramWidgets.append(spin);
            params[names[i]] = defVal;
        } else if (defVal.type() == QVariant::String) {
            auto *edit = new QLineEdit;
            edit->setText(defVal.toString());
            edit->setObjectName(names[i]);
            connect(edit, &QLineEdit::textChanged, this, &MainWindow::onParamChanged);
            row->addWidget(edit);
            m_paramWidgets.append(edit);
            params[names[i]] = defVal;
        } else if (defVal.type() == QVariant::List) {
            auto *combo = new QComboBox;
            auto list = defVal.toList();
            for (const auto &v : list) {
                combo->addItem(v.toString());
            }
            combo->setObjectName(names[i]);
            connect(combo, QOverload<int>::of(&QComboBox::currentIndexChanged),
                    this, &MainWindow::onParamChanged);
            row->addWidget(combo);
            m_paramWidgets.append(combo);
            params[names[i]] = list.isEmpty() ? QVariant() : list[0];
        }

        row->addStretch();
        m_paramLayout->addLayout(row);
    }

    if (!names.isEmpty()) {
        algo->setParams(params);
    }
    m_paramLayout->addStretch();
}

void MainWindow::onParamChanged()
{
    if (!m_currentAlgo) return;
    QVariantMap params;
    for (auto *w : m_paramWidgets) {
        QString name = w->objectName();
        if (auto *spin = qobject_cast<QSpinBox*>(w)) {
            params[name] = spin->value();
        } else if (auto *edit = qobject_cast<QLineEdit*>(w)) {
            params[name] = edit->text();
        } else if (auto *combo = qobject_cast<QComboBox*>(w)) {
            params[name] = combo->currentText();
        }
    }
    m_currentAlgo->setParams(params);
}

void MainWindow::onRunClicked()
{
    if (!m_currentAlgo) return;
    m_animTimer->stop();

    m_currentAlgo->reset();
    m_currentStep = 0;
    m_stepMode = false;
    m_logBrowser->clear();
    m_logBrowser->append("<b>开始演示:</b> " + m_currentAlgo->name());
    m_logBrowser->append("");

    setControlsEnabled(false);
    m_timer.start();

    if (m_worker && m_worker->isRunning()) {
        m_worker->requestStop();
        m_worker->wait(3000);
    }

    m_worker = new AlgorithmWorker(m_currentAlgo, this);
    connect(m_worker, &AlgorithmWorker::finished, this, [this]() {
        qint64 elapsed = m_timer.elapsed();
        m_timeLabel->setText(QString("耗时: %1 ms").arg(elapsed));
        m_statusLabel->setText("演示完成");
        updateLog();
        updateVisualization();

        // Start animation through snapshots if available
        int snapCount = m_currentAlgo->snapshotCount();
        if (snapCount > 0) {
            m_currentStep = 0;
            showSnapshot(0);
            m_animTimer->start();
        } else {
            setControlsEnabled(true);
            m_runBtn->setEnabled(true);
        }
    });
    m_worker->start();
}

void MainWindow::onStepClicked()
{
    if (!m_currentAlgo) return;
    m_animTimer->stop();

    if (m_currentStep == 0) {
        m_stepMode = true;
        m_currentAlgo->reset();
        // Run synchronously in main thread for step mode
        m_currentAlgo->run();
        m_logBrowser->clear();
        m_logBrowser->append("<b>逐步演示:</b> " + m_currentAlgo->name());
        m_logBrowser->append("");
        m_timer.start();
    }

    int snapCount = m_currentAlgo->snapshotCount();
    int totalSteps = qMax(snapCount, m_currentAlgo->steps().size());

    if (m_currentStep < totalSteps) {
        if (m_currentStep < snapCount) {
            showSnapshot(m_currentStep);
        }
        // Show corresponding log step
        auto steps = m_currentAlgo->steps();
        if (m_currentStep < steps.size()) {
            m_logBrowser->append(QString("[步骤 %1] %2").arg(m_currentStep + 1).arg(steps[m_currentStep].description));
        }
        m_currentStep++;
        m_statusLabel->setText(QString("步骤 %1/%2").arg(m_currentStep).arg(totalSteps));
    }

    if (m_currentStep >= totalSteps) {
        qint64 elapsed = m_timer.elapsed();
        m_timeLabel->setText(QString("耗时: %1 ms").arg(elapsed));
        m_statusLabel->setText("演示完成");
        if (!m_currentAlgo->resultText().isEmpty()) {
            m_logBrowser->append("");
            m_logBrowser->append("<b>结果:</b> " + m_currentAlgo->resultText());
        }
        setControlsEnabled(true);
    }
}

void MainWindow::onResetClicked()
{
    m_animTimer->stop();
    if (!m_currentAlgo) return;
    if (m_worker && m_worker->isRunning()) {
        m_worker->requestStop();
        m_worker->wait(3000);
    }
    m_currentAlgo->reset();
    m_currentStep = 0;
    m_logBrowser->clear();
    m_timeLabel->setText("耗时: --");
    m_statusLabel->setText("已重置");
    m_barChart->clearData();
    m_treeViz->clearData();
    m_graphViz->clearData();
    m_vizStack->setCurrentWidget(m_defaultViz);
    setControlsEnabled(true);
    m_runBtn->setEnabled(true);
    m_stepBtn->setEnabled(true);
}

void MainWindow::onStepCompleted(int stepIndex)
{
    Q_UNUSED(stepIndex);
}

void MainWindow::onAlgorithmFinished()
{
}

void MainWindow::onAnimTimerTick()
{
    if (!m_currentAlgo || m_stepMode) {
        m_animTimer->stop();
        return;
    }

    int snapCount = m_currentAlgo->snapshotCount();
    if (m_currentStep < snapCount) {
        showSnapshot(m_currentStep);

        // Show corresponding log
        auto steps = m_currentAlgo->steps();
        if (m_currentStep < steps.size()) {
            m_logBrowser->append(QString("[步骤 %1] %2").arg(m_currentStep + 1).arg(steps[m_currentStep].description));
        }

        m_currentStep++;
        m_statusLabel->setText(QString("动画中 步骤 %1/%2").arg(m_currentStep).arg(snapCount));
    } else {
        m_animTimer->stop();
        if (!m_currentAlgo->resultText().isEmpty()) {
            m_logBrowser->append("");
            m_logBrowser->append("<b>结果:</b> " + m_currentAlgo->resultText());
        }
        m_statusLabel->setText("演示完成");
        setControlsEnabled(true);
        m_runBtn->setEnabled(true);
    }
}

void MainWindow::updateLog()
{
    if (!m_currentAlgo) return;
    auto steps = m_currentAlgo->steps();
    for (int i = 0; i < steps.size(); i++) {
        m_logBrowser->append(QString("[步骤 %1] %2").arg(i + 1).arg(steps[i].description));
    }
    if (!m_currentAlgo->resultText().isEmpty()) {
        m_logBrowser->append("");
        m_logBrowser->append("<b>结果:</b> " + m_currentAlgo->resultText());
    }
}

void MainWindow::showSnapshot(int index)
{
    if (!m_currentAlgo) return;
    QVariantMap snap = m_currentAlgo->snapshotAt(index);
    if (snap.isEmpty()) return;

    QString vizType = snap.value("type", "none").toString();

    m_barChart->clearData();
    m_treeViz->clearData();
    m_graphViz->clearData();

    if (vizType == "barchart") {
        m_barChart->setData(snap);
        m_vizStack->setCurrentWidget(m_barChart);
    } else if (vizType == "tree") {
        m_treeViz->setData(snap);
        m_vizStack->setCurrentWidget(m_treeViz);
    } else if (vizType == "graph") {
        m_graphViz->setData(snap);
        m_vizStack->setCurrentWidget(m_graphViz);
    }
}

void MainWindow::updateVisualization()
{
    if (!m_currentAlgo) return;
    QVariantMap vizData = m_currentAlgo->visualizationData();
    QString vizType = vizData.value("type", "none").toString();

    m_barChart->clearData();
    m_treeViz->clearData();
    m_graphViz->clearData();

    if (vizType == "barchart") {
        m_barChart->setData(vizData);
        m_vizStack->setCurrentWidget(m_barChart);
    } else if (vizType == "tree") {
        m_treeViz->setData(vizData);
        m_vizStack->setCurrentWidget(m_treeViz);
    } else if (vizType == "graph") {
        m_graphViz->setData(vizData);
        m_vizStack->setCurrentWidget(m_graphViz);
    } else {
        m_vizStack->setCurrentWidget(m_defaultViz);
    }
}

void MainWindow::setControlsEnabled(bool enabled)
{
    m_runBtn->setEnabled(enabled);
    m_stepBtn->setEnabled(enabled);
    m_resetBtn->setEnabled(enabled);
}
