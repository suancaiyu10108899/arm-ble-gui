# arm-ble-gui 调试日志

## 2026-06-12 — BLE 设备列表不显示问题

### 现象
- 终端日志: `[BleScanner] Scan finished. Found 57 device(s).`（扫描正常）
- UI: 设备列表空白，状态未更新

### 尝试的修复

| # | 尝试 | 结果 |
|---|------|:--:|
| 1 | 删除 `BleDeviceInfo.qtInfo`（QBluetoothDeviceInfo 未注册元类型） | ❌ |
| 2 | `Q_DECLARE_METATYPE` + `qRegisterMetaType<>()` | ❌ |
| 3 | 信号改为值传递 `const BleDeviceInfo` | ❌ |
| 4 | slot 对齐值传递 + clean rebuild | ❌ |

### 根因分析
`QBluetoothDeviceDiscoveryAgent` 在 Windows 上通过 WinRT 后台线程回调。
BleScanner 的 emit 在蓝牙线程执行，DeviceScanPanel 在主线程。
跨线程 signal/slot 需要 queued connection，但 Qt 6.11 + MSVC 下
`BleDeviceInfo` 的跨线程拷贝行为尚未完全诊断清楚。

### 待尝试的方案
- 使用 `QMetaObject::invokeMethod` 手动调度到主线程
- 用 `Qt::DirectConnection` 测试是否是连接类型问题
- 在 BleScanner 内部用 `QTimer::singleShot(0, ...)` 强制回到主线程 emit
- 需要板子在手里时配合终端日志调试

### 当前状态
arm-ble-gui 骨架完成（编译/测试通过），BLE 扫描功能存在跨线程信号传递 bug。
暂停调试，等待手柄协议文档接手后一并修复。
