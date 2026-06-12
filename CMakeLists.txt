cmake_minimum_required(VERSION 3.25)

project(arm-ble-gui
    VERSION 0.1.0
    DESCRIPTION "ARM-BLE Debug Tool - BLE 蓝牙调试验证工具"
    LANGUAGES CXX
)

# ---- C++ 标准 ----
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

# ---- CTest ----
include(CTest)

# ---- 查找 Qt ----
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Widgets
    Bluetooth
    Test
)

# ---- 进入子目录 ----
add_subdirectory(src)
add_subdirectory(tests)
