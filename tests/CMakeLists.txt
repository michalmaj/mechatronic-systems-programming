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

add_executable(engine_test engine_test.cpp)
target_link_libraries(engine_test PRIVATE psm_core)
add_test(NAME engine_test COMMAND engine_test)

add_executable(estop_latch_test estop_latch_test.cpp)
target_link_libraries(estop_latch_test PRIVATE psm_core)
add_test(NAME estop_latch_test COMMAND estop_latch_test)

add_executable(mode_test mode_test.cpp)
target_link_libraries(mode_test PRIVATE psm_core)
add_test(NAME mode_test COMMAND mode_test)

add_executable(scenario_input_test scenario_input_test.cpp)
target_link_libraries(scenario_input_test PRIVATE psm_core)
add_test(NAME scenario_input_test COMMAND scenario_input_test)

add_executable(safety_supervisor_test safety_supervisor_test.cpp)
target_link_libraries(safety_supervisor_test PRIVATE psm_core)
add_test(NAME safety_supervisor_test COMMAND safety_supervisor_test)

add_executable(engine_estop_test engine_estop_test.cpp)
target_link_libraries(engine_estop_test PRIVATE psm_core)
add_test(NAME engine_estop_test COMMAND engine_estop_test)

add_executable(sensor_snapshot_test sensor_snapshot_test.cpp)
target_link_libraries(sensor_snapshot_test PRIVATE psm_core)
add_test(NAME sensor_snapshot_test COMMAND sensor_snapshot_test)

add_executable(scripted_fault_test scripted_fault_test.cpp)
target_link_libraries(scripted_fault_test PRIVATE psm_core)
add_test(NAME scripted_fault_test COMMAND scripted_fault_test)

add_executable(presence_sensor_test presence_sensor_test.cpp)
target_link_libraries(presence_sensor_test PRIVATE psm_core)
add_test(NAME presence_sensor_test COMMAND presence_sensor_test)

add_executable(weight_sensor_test weight_sensor_test.cpp)
target_link_libraries(weight_sensor_test PRIVATE psm_core)
add_test(NAME weight_sensor_test COMMAND weight_sensor_test)

add_test(NAME simulator_cli_smoke COMMAND simulator_cli)
