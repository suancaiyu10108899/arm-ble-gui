#include "MainWindow.h"
#include "DeviceScanPanel.h"
#include "PresetPanel.h"
#include "SendPanel.h"
#include "LogPanel.h"
#include "BleScanner.h"
#include "BleConnection.h"
#include "StyleConstants.h"

#include <QSplitter>
#include <QMenuBar>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QMessageBox>

MainWindow::MainWindow(armble::BleScanner    *scanner,
                       armble::BleConnection *connection,
                       QWidget *parent)
    : QMainWindow(parent)
    , m_scanner(scanner)
    , m_connection(connection)
{
    setupUI();
    setupMenuBar();
    setupStatusBar();
    setupConnections();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    setWindowTitle(QStringLiteral("ARM-BLE 调试器"));
    resize(Style::kWindowWidth, Style::kWindowHeight);

    // ── 左侧面板：设备扫描 ──
    m_deviceScanPanel = new DeviceScanPanel(this);
    m_deviceScanPanel->setFixedWidth(Style::kLeftPanelWidth);
    // 注入依赖
    m_deviceScanPanel->setScanner(m_scanner);
    m_deviceScanPanel->setConnection(m_connection);

    // ── 右侧面板：垂直布局 ──
    auto *rightWidget = new QWidget(this);
    auto *rightLayout = new QVBoxLayout(rightWidget);

    // 上部：预设指令 + 发送面板
    auto *topWidget = new QWidget(this);
    auto *topLayout = new QHBoxLayout(topWidget);

    m_presetPanel = new PresetPanel(this);
    topLayout->addWidget(m_presetPanel);

    m_sendPanel = new SendPanel(this);
    m_sendPanel->setConnection(m_connection);
    topLayout->addWidget(m_sendPanel, 1);  // 发送面板占更多空间

    rightLayout->addWidget(topWidget);

    // 下部：接收日志
    m_logPanel = new LogPanel(this);
    rightLayout->addWidget(m_logPanel, 1);  // 日志占剩余空间

    // ── 水平分割器 ──
    m_mainSplitter = new QSplitter(Qt::Horizontal, this);
    m_mainSplitter->addWidget(m_deviceScanPanel);
    m_mainSplitter->addWidget(rightWidget);
    m_mainSplitter->setSizes({Style::kLeftPanelWidth, Style::kRightPanelWidth});
    m_mainSplitter->setStretchFactor(0, 0);
    m_mainSplitter->setStretchFactor(1, 1);

    setCentralWidget(m_mainSplitter);
}

void MainWindow::setupMenuBar()
{
    // 文件菜单
    QMenu *fileMenu = menuBar()->addMenu(QStringLiteral("文件(&F)"));

    QAction *exitAction = fileMenu->addAction(QStringLiteral("退出(&Q)"));
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    // 工具菜单
    QMenu *toolsMenu = menuBar()->addMenu(QStringLiteral("工具(&T)"));

    QAction *clearLogAction = toolsMenu->addAction(QStringLiteral("清空日志"));
    connect(clearLogAction, &QAction::triggered, this, [this]() {
        m_logPanel->clearLog();
    });

    // 帮助菜单
    QMenu *helpMenu = menuBar()->addMenu(QStringLiteral("帮助(&H)"));
    QAction *aboutAction = helpMenu->addAction(QStringLiteral("关于(&A)"));
    connect(aboutAction, &QAction::triggered, this, [this]() {
        QMessageBox::about(this,
            QStringLiteral("关于 ARM-BLE 调试器"),
            QStringLiteral("ARM-BLE 调试器 v0.1.0\n\n"
                           "假肢机械臂 BLE 蓝牙调试验证工具\n\n"
                           "目标板：Adafruit Feather nRF52840 Express\n"
                           "协议：Nordic UART Service (NUS)"));
    });
}

void MainWindow::setupStatusBar()
{
    statusBar()->showMessage(QStringLiteral("就绪 — 请扫描设备"), 0);
}

void MainWindow::setupConnections()
{
    // ── 扫描面板 → MainWindow → Connection ──
    connect(m_deviceScanPanel, &DeviceScanPanel::connectRequested,
            this, &MainWindow::onConnectRequested);
    connect(m_deviceScanPanel, &DeviceScanPanel::disconnectRequested,
            this, &MainWindow::onDisconnectRequested);

    // ── 预设面板 → Connection ──
    connect(m_presetPanel, &PresetPanel::presetCommand,
            this, [this](const QString &cmd) {
                if (m_connection && m_connection->state() == armble::BleConnection::State::Ready) {
                    m_connection->sendText(cmd);
                    m_logPanel->appendLog(QStringLiteral("发送: %1").arg(cmd), "#0066cc");
                }
            });

    // ── 发送面板 → Connection ──
    connect(m_sendPanel, &SendPanel::sendRequested,
            this, [this](const QString &text) {
                if (m_connection && m_connection->state() == armble::BleConnection::State::Ready) {
                    m_connection->sendText(text);
                    m_logPanel->appendLog(QStringLiteral("发送: %1").arg(text), "#0066cc");
                }
            });

    // ── Connection 数据 → 日志面板 ──
    connect(m_connection, &armble::BleConnection::textReceived,
            this, [this](const QString &text) {
                m_logPanel->appendLog(QStringLiteral("接收: %1").arg(text), "#2e7d32");
            });

    // ── Connection 状态 → 状态栏 ──
    connect(m_connection, &armble::BleConnection::stateChanged,
            this, [this](int state) {
                using State = armble::BleConnection::State;
                switch (static_cast<State>(state)) {
                case State::Disconnected:
                    statusBar()->showMessage(QStringLiteral("已断开"), 0);
                    break;
                case State::Connecting:
                    statusBar()->showMessage(QStringLiteral("连接中..."), 0);
                    break;
                case State::DiscoveringServices:
                    statusBar()->showMessage(QStringLiteral("发现服务..."), 0);
                    break;
                case State::Ready:
                    statusBar()->showMessage(
                        QStringLiteral("已连接 — %1").arg(m_connection->deviceAddress()), 0);
                    break;
                case State::Error:
                    statusBar()->showMessage(QStringLiteral("连接错误"), 0);
                    break;
                }
            });

    connect(m_connection, &armble::BleConnection::errorOccurred,
            this, [this](const QString &msg) {
                m_logPanel->appendLog(QStringLiteral("错误: %1").arg(msg), "#c62828");
            });
}

void MainWindow::onConnectRequested(const QString &address)
{
    m_logPanel->appendLog(QStringLiteral("正在连接 %1...").arg(address), "#1565c0");
    m_connection->connectToAddress(address);
}

void MainWindow::onDisconnectRequested()
{
    m_logPanel->appendLog(QStringLiteral("断开连接"), "#666666");
    m_connection->disconnect();
}
