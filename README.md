# arm-ble-gui — BLE 蓝牙调试验证工具

> 项目：假肢机械臂 — 上位机调试工具
> 技术栈：Qt 6.11.0 + C++17 + CMake + MSVC 2022
> 目标板：Adafruit Feather nRF52840 Express

## 功能

- 扫描附近 BLE 设备
- 连接 nRF52840 蓝牙板（Nordic UART Service 协议）
- 双向收发数据
- 预设指令按钮（舵机控制占位）
- 接收日志（时间戳 + 自动滚动）

## 环境依赖

| 工具 | 版本 | 备注 |
|------|------|------|
| CMake | 3.25+ | |
| Qt | 6.11.0 msvc2022_64 | **必须勾选 Qt Connectivity 组件** |
| MSVC | VS 2022 Community | |
| Ninja | VS 内嵌 | 通过 VsDevShell 激活 |
| 蓝牙 | BLE 4.0+ | 笔记本内置 或 USB 适配器 |

> ⚠️ **Qt Connectivity 改名**：Qt 6.8+ 中，`Qt Bluetooth` 在 Maintenance Tool 中已重命名为 `Qt Connectivity`（包含 Bluetooth + NFC）。CMake 中仍用 `find_package(Qt6 Bluetooth)` ，不受影响。

## 构建

```powershell
# 每次新开终端先激活 MSVC 环境
. .\scripts\dev-shell.ps1

# 配置
cmake --preset win-msvc-debug

# 编译
cmake --build --preset win-msvc-debug

# 测试
cd build\win-msvc-debug
ctest --output-on-failure
```

## 运行

```powershell
# 方式一：在 VsDevShell 环境下（Qt DLL 已在 PATH）
.\build\win-msvc-debug\src\app\arm-ble-gui.exe

# 方式二：部署 DLL 后独立运行
D:\Qt\6.11.0\msvc2022_64\bin\windeployqt.exe .\build\win-msvc-debug\src\app\arm-ble-gui.exe
```

## 架构

```
src/
├── ble/          BLE 层（Qt6::Bluetooth，不依赖 Widgets）
│   ├── BleDeviceInfo   设备信息结构
│   ├── BleScanner      设备扫描器
│   ├── BleConnection   BLE 连接管理（NUS 协议）
│   └── NusService      NUS UUID 常量 + 工具方法
├── ui/           UI 层（Qt6::Widgets）
│   ├── MainWindow      主窗口（编排者角色）
│   ├── DeviceScanPanel 设备扫描面板
│   ├── PresetPanel     预设指令按钮
│   ├── SendPanel       发送面板
│   └── LogPanel        接收日志
└── app/          入口（依赖注入组装）
    └── main.cpp
```

## 已验证

- CMake configure ✅ / build ✅ (19/19)
- 单元测试 ✅ (1/1: tst_nus_protocol)
- BLE 硬件环境 ✅ (Intel Wireless Bluetooth + BLE Enumerator)

## 相关项目

- 固件仓库：[arm-ble](https://github.com/suancaiyu10108899/arm-ble)
- 文档仓库：[prosthetic-robotic-arm](https://github.com/suancaiyu10108899/prosthetic-robotic-arm)
