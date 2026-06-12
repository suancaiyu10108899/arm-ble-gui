#pragma once

#include <QString>
#include <QBluetoothDeviceInfo>

namespace armble {

/**
 * @brief BLE 设备扫描结果
 *
 * 包含 Qt 原生的 QBluetoothDeviceInfo（用于后续连接）
 * 以及便于 UI 显示的解析字段。
 */
struct BleDeviceInfo {
    QBluetoothDeviceInfo qtInfo;   ///< Qt 原生设备信息（连接时必需）
    QString  name;                  ///< 设备名称（可能为空/N/A）
    QString  address;               ///< MAC 地址，格式如 "D3:52:88:3A:06:27"
    int      rssi = 0;             ///< 信号强度 (dBm)，越接近 0 越强

    /** 是否有有效名称 */
    bool hasName() const { return !name.isEmpty() && name != "N/A"; }

    /** 用于 UI 显示 */
    QString displayText() const
    {
        if (hasName())
            return QString("%1 (%2)").arg(name, address);
        return address;
    }
};

} // namespace armble
