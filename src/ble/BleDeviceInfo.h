#pragma once

#include <QString>

namespace armble {

/**
 * @brief BLE 设备扫描结果
 *
 * 纯数据结构，不含 Qt 对象语义（非 QObject）。
 * 用于在扫描列表和连接逻辑之间传递设备信息。
 *
 * 注意：不持有 QBluetoothDeviceInfo，因为该类型未注册为 Qt 元类型，
 * 会导致跨线程信号/槽传递失败。连接时用 MAC 地址重建 QBluetoothDeviceInfo。
 */
struct BleDeviceInfo {
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
