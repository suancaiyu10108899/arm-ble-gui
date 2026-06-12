#include "BleScanner.h"
#include "NusService.h"

#include <QDebug>

namespace armble {

BleScanner::BleScanner(QObject *parent)
    : QObject(parent)
    , m_agent(new QBluetoothDeviceDiscoveryAgent(this))
{
    connect(m_agent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &BleScanner::onDeviceDiscovered);
    connect(m_agent, &QBluetoothDeviceDiscoveryAgent::finished,
            this, &BleScanner::onScanFinished);
    connect(m_agent, &QBluetoothDeviceDiscoveryAgent::errorOccurred,
            this, &BleScanner::onError);
}

BleScanner::~BleScanner()
{
    if (m_agent->isActive()) {
        m_agent->stop();
    }
}

void BleScanner::startScan()
{
    m_devices.clear();
    qDebug() << "[BleScanner] Starting BLE scan...";
    m_agent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
}

void BleScanner::stopScan()
{
    if (m_agent->isActive()) {
        m_agent->stop();
        qDebug() << "[BleScanner] Scan stopped.";
    }
}

bool BleScanner::isScanning() const
{
    return m_agent->isActive();
}

QList<BleDeviceInfo> BleScanner::devices() const
{
    return m_devices;
}

void BleScanner::onDeviceDiscovered(const QBluetoothDeviceInfo &qtInfo)
{
    BleDeviceInfo info;
    info.name    = qtInfo.name();
    info.address = qtInfo.address().toString();
    info.rssi    = qtInfo.rssi();

    // 检查是否已存在（按 MAC 地址去重）
    for (const auto &existing : m_devices) {
        if (existing.address == info.address) {
            return;  // 重复的设备，忽略
        }
    }

    m_devices.append(info);
    emit deviceDiscovered(info);
}

void BleScanner::onScanFinished()
{
    qDebug() << "[BleScanner] Scan finished. Found" << m_devices.size() << "device(s).";
    emit scanFinished();
}

void BleScanner::onError(QBluetoothDeviceDiscoveryAgent::Error error)
{
    QString msg;
    switch (error) {
    case QBluetoothDeviceDiscoveryAgent::PoweredOffError:
        msg = QStringLiteral("蓝牙未开启，请打开电脑蓝牙");
        break;
    case QBluetoothDeviceDiscoveryAgent::InputOutputError:
        msg = QStringLiteral("蓝牙 IO 错误");
        break;
    default:
        msg = QStringLiteral("扫描失败: %1").arg(m_agent->errorString());
        break;
    }
    qWarning() << "[BleScanner]" << msg;
    emit scanError(msg);
}

} // namespace armble
