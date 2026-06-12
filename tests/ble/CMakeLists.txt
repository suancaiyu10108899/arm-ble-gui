# tests/ble/CMakeLists.txt

add_executable(tst_nus_protocol
    tst_nus_protocol.cpp
)

target_link_libraries(tst_nus_protocol
    PRIVATE
        arm_ble_gui_ble
        Qt6::Test
)

add_test(
    NAME tst_nus_protocol
    COMMAND tst_nus_protocol
)
