add_executable(zone_test zone_test.cpp)
target_link_libraries(zone_test PRIVATE psm_core)
add_test(NAME zone_test COMMAND zone_test)

add_executable(value_types_test value_types_test.cpp)
target_link_libraries(value_types_test PRIVATE psm_core)
add_test(NAME value_types_test COMMAND value_types_test)

add_executable(controller_test controller_test.cpp)
target_link_libraries(controller_test PRIVATE psm_core)
add_test(NAME controller_test COMMAND controller_test)

add_executable(plant_test plant_test.cpp)
target_link_libraries(plant_test PRIVATE psm_core)
add_test(NAME plant_test COMMAND plant_test)

add_executable(diverter_test diverter_test.cpp)
target_link_libraries(diverter_test PRIVATE psm_core)
add_test(NAME diverter_test COMMAND diverter_test)

add_executable(belt_motor_test belt_motor_test.cpp)
target_link_libraries(belt_motor_test PRIVATE psm_core)
add_test(NAME belt_motor_test COMMAND belt_motor_test)

add_executable(tick_result_test tick_result_test.cpp)
target_link_libraries(tick_result_test PRIVATE psm_core)
add_test(NAME tick_result_test COMMAND tick_result_test)

add_test(NAME simulator_cli_smoke COMMAND simulator_cli)
