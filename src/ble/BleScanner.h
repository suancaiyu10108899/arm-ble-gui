#pragma once

#include <QObject>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QList>

#include "BleDeviceInfo.h"

namespace armble {

/**
 * @brief BLE 设备扫描器
 *
 * 封装 QBluetoothDeviceDiscoveryAgent，提供：
 * - startScan() / stopScan() 控制扫描生命周期
 * - deviceDiscovered() 信号发射解析后的 BleDeviceInfo
 * - scanFinished() 信号通知扫描完成
 *
 * 为什么独立封装而不是直接在 UI 里用？
 * 1. UI 层只关心 BleDeviceInfo 列表，不应知道 QBluetooth* 内部细节
 * 2. 方便测试（可以 mock 这个类）
 * 3. 未来切换平台（Android/iOS）只改这一层
 */
class BleScanner : public QObject
{
    Q_OBJECT

public:
    explicit BleScanner(QObject *parent = nullptr);
    ~BleScanner();

    /// 开始扫描（默认无限扫描，直到手动 stop）
    void startScan();

    /// 停止扫描
    void stopScan();

    /// 是否正在扫描
    bool isScanning() const;

    /// 当前扫描到的设备列表（每次 startScan 后清空重建）
    QList<BleDeviceInfo> devices() const;

signals:
    /// 发现一个新设备（值传递 — 跨线程 queued connection 要求可复制）
    void deviceDiscovered(const BleDeviceInfo info);

    /// 扫描完成（手动停止 或 超时）
    void scanFinished();

    /// 扫描出错
    void scanError(const QString &errorMessage);

private slots:
    void onDeviceDiscovered(const QBluetoothDeviceInfo &qtInfo);
    void onScanFinished();
    void onError(QBluetoothDeviceDiscoveryAgent::Error error);

private:
    QBluetoothDeviceDiscoveryAgent *m_agent = nullptr;
    QList<BleDeviceInfo> m_devices;
};

} // namespace armble
