#include "BleConnection.h"
#include "NusService.h"

#include <QDebug>

namespace armble {

BleConnection::BleConnection(QObject *parent)
    : QObject(parent)
{
}

BleConnection::~BleConnection()
{
    disconnect();
}

void BleConnection::connectToAddress(const QString &address)
{
    if (m_state != State::Disconnected) {
        qWarning() << "[BleConnection] Already connected/connecting, disconnect first.";
        return;
    }

    m_deviceAddress = address;
    setState(State::Connecting);

    // 创建 BLE 控制器（Qt 6.11 需要 QBluetoothDeviceInfo 而非 QBluetoothAddress）
    QBluetoothDeviceInfo deviceInfo(QBluetoothAddress(address), QString(), 0);
    m_controller = QLowEnergyController::createCentral(deviceInfo, this);

    connect(m_controller, &QLowEnergyController::connected,
            this, &BleConnection::onConnected);
    connect(m_controller, &QLowEnergyController::disconnected,
            this, &BleConnection::onDisconnected);
    connect(m_controller, &QLowEnergyController::errorOccurred,
            this, &BleConnection::onError);
    connect(m_controller, &QLowEnergyController::serviceDiscovered,
            this, &BleConnection::onServiceDiscovered);
    connect(m_controller, &QLowEnergyController::discoveryFinished,
            this, &BleConnection::onServiceDiscoveryFinished);

    qDebug() << "[BleConnection] Connecting to" << address << "...";
    m_controller->connectToDevice();
}

void BleConnection::disconnect()
{
    if (m_nusService) {
        m_nusService->deleteLater();
        m_nusService = nullptr;
    }
    if (m_controller) {
        m_controller->disconnectFromDevice();
        m_controller->deleteLater();
        m_controller = nullptr;
    }
    m_txChar = QLowEnergyCharacteristic();
    m_rxChar = QLowEnergyCharacteristic();
    setState(State::Disconnected);
}

void BleConnection::sendData(const QByteArray &data)
{
    if (m_state != State::Ready) {
        qWarning() << "[BleConnection] Not ready, cannot send.";
        emit errorOccurred(QStringLiteral("设备未就绪，无法发送"));
        return;
    }

    if (!m_txChar.isValid()) {
        qWarning() << "[BleConnection] TX characteristic not found.";
        emit errorOccurred(QStringLiteral("TX 特征未找到"));
        return;
    }

    m_nusService->writeCharacteristic(m_txChar, data);
}

void BleConnection::sendText(const QString &text)
{
    sendData(text.toUtf8());
}

void BleConnection::setState(State newState)
{
    if (m_state != newState) {
        m_state = newState;
        const char *stateNames[] = {"Disconnected", "Connecting",
                                     "DiscoveringServices", "Ready", "Error"};
        qDebug() << "[BleConnection] State ->" << stateNames[static_cast<int>(newState)];
        emit stateChanged(static_cast<int>(newState));
    }
}

// ── Slots ──

void BleConnection::onConnected()
{
    qDebug() << "[BleConnection] Connected, discovering services...";
    setState(State::DiscoveringServices);
    m_controller->discoverServices();
}

void BleConnection::onDisconnected()
{
    qDebug() << "[BleConnection] Disconnected.";
    if (m_nusService) {
        m_nusService->deleteLater();
        m_nusService = nullptr;
    }
    if (m_controller) {
        m_controller->deleteLater();
        m_controller = nullptr;
    }
    setState(State::Disconnected);
}

void BleConnection::onError(QLowEnergyController::Error error)
{
    QString msg;
    switch (error) {
    case QLowEnergyController::ConnectionError:
        msg = QStringLiteral("连接失败，可能是设备不在范围内或已断开");
        break;
    case QLowEnergyController::RemoteHostClosedError:
        msg = QStringLiteral("设备主动断开连接");
        break;
    default:
        msg = QStringLiteral("BLE 错误: %1").arg(m_controller->errorString());
        break;
    }
    qWarning() << "[BleConnection]" << msg;
    setState(State::Error);
    emit errorOccurred(msg);
}

void BleConnection::onServiceDiscovered(const QBluetoothUuid &serviceUuid)
{
    const QString uuidStr = serviceUuid.toString();
    qDebug() << "[BleConnection] Service discovered:" << uuidStr;

    // 只关心 NUS 服务
    if (NusService::isNusService(uuidStr)) {
        qDebug() << "[BleConnection] Found NUS service, creating service object...";
        m_nusService = m_controller->createServiceObject(serviceUuid, this);

        connect(m_nusService, &QLowEnergyService::stateChanged,
                this, &BleConnection::onServiceStateChanged);
        connect(m_nusService, &QLowEnergyService::characteristicChanged,
                this, &BleConnection::onCharacteristicChanged);
        connect(m_nusService, &QLowEnergyService::characteristicWritten,
                this, &BleConnection::onCharacteristicWritten);
    }
}

void BleConnection::onServiceDiscoveryFinished()
{
    qDebug() << "[BleConnection] Service discovery finished.";
    if (!m_nusService) {
        setState(State::Error);
        emit errorOccurred(QStringLiteral("未找到 NUS 服务，请确认板子已烧录蓝牙固件"));
        return;
    }
    // 需要手动调用 discoverDetails 来获取特征信息
    m_nusService->discoverDetails();
}

void BleConnection::onServiceStateChanged(QLowEnergyService::ServiceState newState)
{
    if (newState == QLowEnergyService::RemoteServiceDiscovered) {
        qDebug() << "[BleConnection] NUS service details discovered.";
        discoverNusCharacteristics();

        if (m_txChar.isValid() && m_rxChar.isValid()) {
            // 启用 RX 特征的通知（板子 → App）
            m_nusService->writeDescriptor(
                m_rxChar.descriptor(QBluetoothUuid::DescriptorType::ClientCharacteristicConfiguration),
                QByteArray::fromHex("0100"));  // 0x01,0x00 = enable notification

            setState(State::Ready);
            qDebug() << "[BleConnection] NUS ready. TX & RX characteristics configured.";
        } else {
            setState(State::Error);
            emit errorOccurred(QStringLiteral("NUS 特征不完整（缺少 TX 或 RX）"));
        }
    }
}

void BleConnection::onCharacteristicChanged(const QLowEnergyCharacteristic &ch,
                                              const QByteArray &value)
{
    Q_UNUSED(ch)
    emit dataReceived(value);
    QString text = QString::fromUtf8(value);
    if (!text.isEmpty()) {
        emit textReceived(text);
    }
}

void BleConnection::onCharacteristicWritten(const QLowEnergyCharacteristic &ch,
                                              const QByteArray &value)
{
    Q_UNUSED(ch)
    Q_UNUSED(value)
    qDebug() << "[BleConnection] Data written successfully (" << value.size() << "bytes).";
}

void BleConnection::discoverNusCharacteristics()
{
    const auto chars = m_nusService->characteristics();
    for (const auto &ch : chars) {
        const QString uuid = ch.uuid().toString();
        qDebug() << "[BleConnection] Characteristic:" << NusService::characteristicName(ch)
                 << "UUID:" << uuid;

        if (uuid.compare(NusService::TX_CHAR_UUID, Qt::CaseInsensitive) == 0) {
            m_txChar = ch;
        } else if (uuid.compare(NusService::RX_CHAR_UUID, Qt::CaseInsensitive) == 0) {
            m_rxChar = ch;
        }
    }
}

} // namespace armble
