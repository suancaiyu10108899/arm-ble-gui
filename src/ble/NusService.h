#pragma once

#include <QString>
#include <QLowEnergyService>
#include <QLowEnergyCharacteristic>

namespace armble {

/**
 * @brief Nordic UART Service (NUS) 常量与工具方法
 *
 * NUS 是一个非官方的 BLE 串口仿真服务，广泛用于 Nordic 芯片。
 * 我们的 nRF52840 板子验证时使用了此服务（见 2026-06-11 BLE 验证日志）。
 *
 * UUID 来源：6月11日 BLE 验证日志中 nRF Connect 确认的值。
 */
class NusService
{
public:
    /// Nordic UART Service UUID（服务级别）
    static const QString SERVICE_UUID;

    /// TX 特征 UUID — 手机/App → 板子（写）
    static const QString TX_CHAR_UUID;

    /// RX 特征 UUID — 板子 → 手机/App（读/通知）
    static const QString RX_CHAR_UUID;

    /**
     * @brief 检查一个服务是否是 NUS
     * @param serviceUuid 服务 UUID 字符串
     * @return true 如果 UUID 匹配 NUS
     */
    static bool isNusService(const QString &serviceUuid);

    /**
     * @brief 获取特征的可读名称（用于调试日志）
     * @param characteristic 特征对象引用
     * @return "TX" / "RX" / "Unknown"
     */
    static QString characteristicName(const QLowEnergyCharacteristic &ch);
};

} // namespace armble
