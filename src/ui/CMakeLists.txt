add_library(arm_ble_gui_ui STATIC
    StyleConstants.h
    MainWindow.h
    MainWindow.cpp
    DeviceScanPanel.h
    DeviceScanPanel.cpp
    PresetPanel.h
    PresetPanel.cpp
    SendPanel.h
    SendPanel.cpp
    LogPanel.h
    LogPanel.cpp
)

target_link_libraries(arm_ble_gui_ui
    PUBLIC
        Qt6::Widgets
    PRIVATE
        arm_ble_gui_ble
)

target_include_directories(arm_ble_gui_ui
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
