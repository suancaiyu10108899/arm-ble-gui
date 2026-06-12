add_executable(arm-ble-gui
    main.cpp
)

target_link_libraries(arm-ble-gui
    PRIVATE
        arm_ble_gui_ui
        arm_ble_gui_ble
        Qt6::Widgets
)
