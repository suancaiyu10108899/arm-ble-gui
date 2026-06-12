#pragma once

#include <QObject>
#include <QLowEnergyController>
#include <QLowEnergyService>
#include <QLowEnergyCharacteristic>

namespace armble {

/**
 * @brief BLE 设备连接管理
 *
 * 封装 QLowEnergyController，管理：
 * - 连接到 BLE 外设
 * - 自动发现 NUS 服务
 * - 双向数据收发（写 TX 特征 / 监听 RX 特征通知）
 *
 * 为什么 NUS 服务发现内置在此类中？
 * 因为我们的板子 100% 用 NUS 协议，不需要通用服务发现。
 * 未来如果需要连接其他非 NUS 设备，再拆分出 ServiceDiscovery 层。
 */
class BleConnection : public QObject
{
    Q_OBJECT

public:
    enum class State {
        Disconnected,
        Connecting,
        DiscoveringServices,
        Ready,      ///< NUS 服务就绪，可以收发
        Error
    };
    Q_ENUM(State)

    explicit BleConnection(QObject *parent = nullptr);
    ~BleConnection();

    /// 连接到指定 MAC 地址的设备
    void connectToAddress(const QString &address);

    /// 断开连接
    void disconnect();

    /// 发送数据到板子（写 TX 特征）
    void sendData(const QByteArray &data);

    /// 发送字符串（UTF-8 编码）
    void sendText(const QString &text);

    /// 当前连接状态
    State state() const { return m_state; }

    /// 已连接设备的 MAC 地址
    QString deviceAddress() const { return m_deviceAddress; }

signals:
    /// 连接状态变化（参数用 int 避免 Qt 6.11 enum class 模板推导问题）
    void stateChanged(int newState);

    /// 收到来自板子的数据（RX 特征通知）
    void dataReceived(const QByteArray &data);

    /// 收到文本数据（UTF-8 解码）
    void textReceived(const QString &text);

    /// 连接或通信出错
    void errorOccurred(const QString &errorMessage);

private slots:
    void onConnected();
    void onDisconnected();
    void onError(QLowEnergyController::Error error);
    void onServiceDiscovered(const QBluetoothUuid &serviceUuid);
    void onServiceDiscoveryFinished();
    void onServiceStateChanged(QLowEnergyService::ServiceState newState);
    void onCharacteristicChanged(const QLowEnergyCharacteristic &ch,
                                  const QByteArray &value);
    void onCharacteristicWritten(const QLowEnergyCharacteristic &ch,
                                  const QByteArray &value);

private:
    void setState(State newState);
    void discoverNusCharacteristics();

    QLowEnergyController *m_controller = nullptr;
    QLowEnergyService     *m_nusService = nullptr;
    QLowEnergyCharacteristic m_txChar;    ///< TX（App → 板子）
    QLowEnergyCharacteristic m_rxChar;    ///< RX（板子 → App）
    State m_state = State::Disconnected;
    QString m_deviceAddress;
};

} // namespace armble
