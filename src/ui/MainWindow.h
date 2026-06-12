#pragma once

#include <QMainWindow>

class DeviceScanPanel;
class PresetPanel;
class SendPanel;
class LogPanel;
class QSplitter;

namespace armble {
class BleScanner;
class BleConnection;
}

/**
 * @brief 主窗口 — BLE 调试器
 *
 * 四区域布局：
 * ┌──────────┬─────────────────────┐
 * │ 左侧面板  │  右侧上部            │
 * │          │  ├ 预设指令          │
 * │ 设备扫描  │  ├ 发送面板          │
 * │          │  右侧下部            │
 * │          │  └ 接收日志          │
 * └──────────┴─────────────────────┘
 *
 * 架构角色：
 * - "编排者"（Orchestrator）—— 负责把 Scanner / Connection 的信号
 *   连接到各个 UI 面板，自己不操作 BLE socket。
 * - 依赖注入：Scanner 和 Connection 由 app 层创建并注入。
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(armble::BleScanner    *scanner,
                        armble::BleConnection *connection,
                        QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onConnectRequested(const QString &address);
    void onDisconnectRequested();

private:
    void setupUI();
    void setupMenuBar();
    void setupStatusBar();
    void setupConnections();

    // 核心依赖（由 app 层注入，不拥有所有权）
    armble::BleScanner    *m_scanner    = nullptr;
    armble::BleConnection *m_connection = nullptr;

    // UI 组件
    DeviceScanPanel *m_deviceScanPanel = nullptr;
    PresetPanel     *m_presetPanel     = nullptr;
    SendPanel       *m_sendPanel       = nullptr;
    LogPanel        *m_logPanel        = nullptr;
    QSplitter       *m_mainSplitter    = nullptr;
};
