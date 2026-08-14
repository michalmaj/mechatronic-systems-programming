add_executable(zone_test zone_test.cpp)
target_link_libraries(zone_test PRIVATE psm_core)
add_test(NAME zone_test COMMAND zone_test)
set_tests_properties(zone_test PROPERTIES LABELS "misja-1")

add_executable(plant_test plant_test.cpp)
target_link_libraries(plant_test PRIVATE psm_core)
add_test(NAME plant_test COMMAND plant_test)
set_tests_properties(plant_test PROPERTIES LABELS "misja-3;misja-29")

add_executable(controller_test controller_test.cpp)
target_link_libraries(controller_test PRIVATE psm_core)
add_test(NAME controller_test COMMAND controller_test)
set_tests_properties(controller_test PROPERTIES LABELS "misja-4")

add_executable(diverter_command_test diverter_command_test.cpp)
target_link_libraries(diverter_command_test PRIVATE psm_core)
add_test(NAME diverter_command_test COMMAND diverter_command_test)
set_tests_properties(diverter_command_test PROPERTIES LABELS "misja-7")

add_executable(diverter_test diverter_test.cpp)
target_link_libraries(diverter_test PRIVATE psm_core)
add_test(NAME diverter_test COMMAND diverter_test)
set_tests_properties(diverter_test PROPERTIES LABELS "misja-8")

add_executable(plant_diverter_test plant_diverter_test.cpp)
target_link_libraries(plant_diverter_test PRIVATE psm_core)
add_test(NAME plant_diverter_test COMMAND plant_diverter_test)
set_tests_properties(plant_diverter_test PROPERTIES LABELS "misja-9;misja-30")

add_executable(tick_result_test tick_result_test.cpp)
target_link_libraries(tick_result_test PRIVATE psm_core)
add_test(NAME tick_result_test COMMAND tick_result_test)
set_tests_properties(tick_result_test PROPERTIES LABELS "misja-10;misja-32")

add_executable(engine_test engine_test.cpp)
target_link_libraries(engine_test PRIVATE psm_core)
add_test(NAME engine_test COMMAND engine_test)
set_tests_properties(engine_test PROPERTIES LABELS "misja-11;misja-32")

add_executable(belt_motor_test belt_motor_test.cpp)
target_link_libraries(belt_motor_test PRIVATE psm_core)
add_test(NAME belt_motor_test COMMAND belt_motor_test)
set_tests_properties(belt_motor_test PROPERTIES LABELS "misja-13")

add_executable(mode_test mode_test.cpp)
target_link_libraries(mode_test PRIVATE psm_core)
add_test(NAME mode_test COMMAND mode_test)
set_tests_properties(mode_test PROPERTIES LABELS "misja-14")

add_executable(belt_gated_engine_test belt_gated_engine_test.cpp)
target_link_libraries(belt_gated_engine_test PRIVATE psm_core)
add_test(NAME belt_gated_engine_test COMMAND belt_gated_engine_test)
set_tests_properties(belt_gated_engine_test PROPERTIES LABELS "misja-15;misja-32")

add_executable(estop_latch_test estop_latch_test.cpp)
target_link_libraries(estop_latch_test PRIVATE psm_core)
add_test(NAME estop_latch_test COMMAND estop_latch_test)
set_tests_properties(estop_latch_test PROPERTIES LABELS "misja-16")

add_executable(estop_mode_test estop_mode_test.cpp)
target_link_libraries(estop_mode_test PRIVATE psm_core)
add_test(NAME estop_mode_test COMMAND estop_mode_test)
set_tests_properties(estop_mode_test PROPERTIES LABELS "misja-17")

add_executable(safety_supervisor_test safety_supervisor_test.cpp)
target_link_libraries(safety_supervisor_test PRIVATE psm_core)
add_test(NAME safety_supervisor_test COMMAND safety_supervisor_test)
set_tests_properties(safety_supervisor_test PROPERTIES LABELS "misja-18")

add_executable(estop_engine_test estop_engine_test.cpp)
target_link_libraries(estop_engine_test PRIVATE psm_core)
add_test(NAME estop_engine_test COMMAND estop_engine_test)
set_tests_properties(estop_engine_test PROPERTIES LABELS "misja-19;misja-32")

add_executable(presence_sensor_test presence_sensor_test.cpp)
target_link_libraries(presence_sensor_test PRIVATE psm_core)
add_test(NAME presence_sensor_test COMMAND presence_sensor_test)
set_tests_properties(presence_sensor_test PROPERTIES LABELS "misja-20;misja-31")

add_executable(weight_sensor_test weight_sensor_test.cpp)
target_link_libraries(weight_sensor_test PRIVATE psm_core)
add_test(NAME weight_sensor_test COMMAND weight_sensor_test)
set_tests_properties(weight_sensor_test PROPERTIES LABELS "misja-21;misja-31")

add_executable(decide_classification_test decide_classification_test.cpp)
target_link_libraries(decide_classification_test PRIVATE psm_core)
add_test(NAME decide_classification_test COMMAND decide_classification_test)
set_tests_properties(decide_classification_test PROPERTIES LABELS "misja-22")

add_executable(sensors_engine_test sensors_engine_test.cpp)
target_link_libraries(sensors_engine_test PRIVATE psm_core)
add_test(NAME sensors_engine_test COMMAND sensors_engine_test)
set_tests_properties(sensors_engine_test PROPERTIES LABELS "misja-24;misja-32")

add_executable(diverter_fault_test diverter_fault_test.cpp)
target_link_libraries(diverter_fault_test PRIVATE psm_core)
add_test(NAME diverter_fault_test COMMAND diverter_fault_test)
set_tests_properties(diverter_fault_test PROPERTIES LABELS "misja-25")

add_executable(plant_deadline_test plant_deadline_test.cpp)
target_link_libraries(plant_deadline_test PRIVATE psm_core)
add_test(NAME plant_deadline_test COMMAND plant_deadline_test)
set_tests_properties(plant_deadline_test PROPERTIES LABELS "misja-26;misja-30")

add_executable(mode_fault_test mode_fault_test.cpp)
target_link_libraries(mode_fault_test PRIVATE psm_core)
add_test(NAME mode_fault_test COMMAND mode_fault_test)
set_tests_properties(mode_fault_test PROPERTIES LABELS "misja-27")

add_executable(fault_engine_test fault_engine_test.cpp)
target_link_libraries(fault_engine_test PRIVATE psm_core)
add_test(NAME fault_engine_test COMMAND fault_engine_test)
set_tests_properties(fault_engine_test PROPERTIES LABELS "misja-28;misja-32")

add_executable(scenario_validation_test scenario_validation_test.cpp)
target_link_libraries(scenario_validation_test PRIVATE psm_core)
add_test(NAME scenario_validation_test COMMAND scenario_validation_test)
set_tests_properties(scenario_validation_test PROPERTIES LABELS "misja-33")

add_executable(run_scenario_test run_scenario_test.cpp)
target_link_libraries(run_scenario_test PRIVATE psm_core)
add_test(NAME run_scenario_test COMMAND run_scenario_test)
set_tests_properties(run_scenario_test PROPERTIES LABELS "misja-34")

add_executable(item_correlation_test item_correlation_test.cpp)
target_link_libraries(item_correlation_test PRIVATE psm_core)
add_test(NAME item_correlation_test COMMAND item_correlation_test)
set_tests_properties(item_correlation_test PROPERTIES LABELS "misja-31")

add_executable(multiple_items_engine_test multiple_items_engine_test.cpp)
target_link_libraries(multiple_items_engine_test PRIVATE psm_core)
add_test(NAME multiple_items_engine_test COMMAND multiple_items_engine_test)
set_tests_properties(multiple_items_engine_test PROPERTIES LABELS "misja-32")

add_test(NAME simulator_cli_smoke COMMAND simulator_cli)
set_tests_properties(simulator_cli_smoke PROPERTIES LABELS "misja-6")

add_executable(simulator_cli_scenario_test simulator_cli_scenario_test.cpp)
target_link_libraries(simulator_cli_scenario_test PRIVATE simulator_cli_scenarios)
add_test(NAME simulator_cli_scenario_test COMMAND simulator_cli_scenario_test)
set_tests_properties(simulator_cli_scenario_test PROPERTIES LABELS "misja-35")

add_test(NAME toolchain_check_smoke COMMAND toolchain_check)
