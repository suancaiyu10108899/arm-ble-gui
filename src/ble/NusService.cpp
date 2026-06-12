#include "NusService.h"

namespace armble {

// UUID 常量从 2026-06-11 BLE 验证日志确认
// nRF Connect 扫到的 NUS 服务 UID: 6E400001-B5A3-F393-E0A9-E50E24DCCA9E
const QString NusService::SERVICE_UUID =
    QStringLiteral("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");
const QString NusService::TX_CHAR_UUID =
    QStringLiteral("6E400002-B5A3-F393-E0A9-E50E24DCCA9E");
const QString NusService::RX_CHAR_UUID =
    QStringLiteral("6E400003-B5A3-F393-E0A9-E50E24DCCA9E");

bool NusService::isNusService(const QString &serviceUuid)
{
    // 比较 UUID 时忽略大小写
    return serviceUuid.compare(SERVICE_UUID, Qt::CaseInsensitive) == 0;
}

QString NusService::characteristicName(const QLowEnergyCharacteristic &ch)
{
    const QString uuid = ch.uuid().toString();
    if (uuid.compare(TX_CHAR_UUID, Qt::CaseInsensitive) == 0)
        return QStringLiteral("TX");
    if (uuid.compare(RX_CHAR_UUID, Qt::CaseInsensitive) == 0)
        return QStringLiteral("RX");
    return QStringLiteral("Unknown");
}

} // namespace armble
