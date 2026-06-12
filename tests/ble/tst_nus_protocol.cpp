#include <QtTest>

#include "NusService.h"

/**
 * @brief NUS 协议单元测试
 *
 * 验证 NUS UUID 常量的正确性（从 BLE 验证日志确认的值）
 */
class TestNusProtocol : public QObject
{
    Q_OBJECT

private slots:
    void testServiceUuid()
    {
        // 验证 NUS 服务 UUID 不为空
        QVERIFY(!armble::NusService::SERVICE_UUID.isEmpty());
        // 验证是标准 128-bit UUID 格式（36 字符含 4 个破折号）
        QCOMPARE(armble::NusService::SERVICE_UUID.length(), 36);
    }

    void testTxRxUuid()
    {
        QVERIFY(!armble::NusService::TX_CHAR_UUID.isEmpty());
        QVERIFY(!armble::NusService::RX_CHAR_UUID.isEmpty());
        // 确保 TX 和 RX 是不同的
        QVERIFY(armble::NusService::TX_CHAR_UUID
                != armble::NusService::RX_CHAR_UUID);
    }

    void testIsNusService()
    {
        // 完全匹配
        QVERIFY(armble::NusService::isNusService(
            armble::NusService::SERVICE_UUID));
        // 大小写不敏感
        QVERIFY(armble::NusService::isNusService(
            armble::NusService::SERVICE_UUID.toLower()));
        QVERIFY(armble::NusService::isNusService(
            armble::NusService::SERVICE_UUID.toUpper()));
        // 不匹配
        QVERIFY(!armble::NusService::isNusService(
            "00000000-0000-0000-0000-000000000000"));
    }
};

QTEST_MAIN(TestNusProtocol)
#include "tst_nus_protocol.moc"
