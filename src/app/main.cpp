#include <QApplication>
#include <QDebug>

#include "MainWindow.h"
#include "BleScanner.h"
#include "BleConnection.h"
#include "BleDeviceInfo.h"

// 注册自定义类型到 Qt 元对象系统 — 使 BleDeviceInfo 可跨线程通过信号/槽传递
Q_DECLARE_OPAQUE_POINTER(armble::BleScanner*)
Q_DECLARE_OPAQUE_POINTER(armble::BleConnection*)

/**
 * @brief ARM-BLE 调试器 — 应用入口
 *
 * 依赖组装流程（与 MindPath 一致）：
 * 1. 创建 QApplication
 * 2. 创建核心依赖（BleScanner / BleConnection）
 * 3. 注入到 MainWindow
 * 4. 显示窗口，进入事件循环
 *
 * 为什么 Scanner 和 Connection 在 main() 创建而不是在 MainWindow 内部创建？
 * - 这是"依赖注入"模式：
 *   MainWindow 不关心 Scanner 是怎么创建的，它只需要一个可用的引用。
 * - 好处：
 *   1. 未来测试时可以注入 Mock 对象
 *   2. Scanner 的生命周期由 app 层统一管理
 *   3. 多个窗口可以共享同一个 Scanner/Connection
 */
int main(int argc, char *argv[])
{
    // 必须先于任何信号/槽连接之前注册自定义类型
    qRegisterMetaType<armble::BleDeviceInfo>("armble::BleDeviceInfo");

    QApplication app(argc, argv);

    app.setApplicationName(QStringLiteral("ARM-BLE-GUI"));
    app.setOrganizationName(QStringLiteral("ProstheticArm"));

    // ── 创建核心依赖 ──
    auto *scanner    = new armble::BleScanner(&app);
    auto *connection = new armble::BleConnection(&app);

    // ── 组装 + 显示 ──
    MainWindow window(scanner, connection);
    window.show();

    qDebug() << "ARM-BLE 调试器 启动完成";
    qDebug() << "  Qt版本:" << qVersion();
    qDebug() << "  目标板: Adafruit Feather nRF52840 Express (NUS)";

    return app.exec();
}
