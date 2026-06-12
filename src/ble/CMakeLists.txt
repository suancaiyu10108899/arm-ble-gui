# BLE 层：蓝牙通信封装
#
# 设计原则：
# - 只依赖 Qt6::Core 和 Qt6::Bluetooth（不依赖 Widgets）
# - 不依赖 ui 层的任何东西
# - 可以独立编译和测试

add_library(arm_ble_gui_ble STATIC
    BleDeviceInfo.h
    BleScanner.h
    BleScanner.cpp
    BleConnection.h
    BleConnection.cpp
    NusService.h
    NusService.cpp
)

target_link_libraries(arm_ble_gui_ble
    PUBLIC
        Qt6::Core
        Qt6::Bluetooth
)

target_include_directories(arm_ble_gui_ble
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
