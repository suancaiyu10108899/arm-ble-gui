#pragma once

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>

#include "BleDeviceInfo.h"

namespace armble {
class BleScanner;
class BleConnection;
}

/**
 * @brief 设备扫描面板（左侧）
 *
 * 功能：
 * - 扫描/停止按钮
 * - 设备列表（ListWidget）
 * - 连接/断开按钮
 * - 状态标签（已断开 / 扫描中 / 已连接）
 */
class DeviceScanPanel : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceScanPanel(QWidget *parent = nullptr);

    /// 依赖注入：主窗口注入 scanner 和 connection 引用
    void setScanner(armble::BleScanner *scanner);
    void setConnection(armble::BleConnection *connection);

signals:
    /// 用户选中设备后点击"连接"
    void connectRequested(const QString &address);

    /// 用户点击"断开"
    void disconnectRequested();

private slots:
    void onScanClicked();
    void onConnectClicked();
    void onDisconnectClicked();
    void onDeviceDiscovered(const armble::BleDeviceInfo &info);
    void onScanFinished();
    void onScanError(const QString &msg);
    void onConnectionStateChanged(int state);

private:
    void setupUI();
    void updateButtonStates();

    armble::BleScanner    *m_scanner    = nullptr;
    armble::BleConnection *m_connection = nullptr;

    QLabel      *m_statusLabel    = nullptr;
    QListWidget *m_deviceList     = nullptr;
    QPushButton *m_scanBtn        = nullptr;
    QPushButton *m_connectBtn     = nullptr;
    QPushButton *m_disconnectBtn  = nullptr;

    bool m_isConnected = false;
};
