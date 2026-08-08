add_executable(zone_test zone_test.cpp)
target_link_libraries(zone_test PRIVATE psm_core)
add_test(NAME zone_test COMMAND zone_test)
set_tests_properties(zone_test PROPERTIES LABELS "misja-1")

add_executable(item_motion_test item_motion_test.cpp)
target_link_libraries(item_motion_test PRIVATE psm_core)
add_test(NAME item_motion_test COMMAND item_motion_test)
set_tests_properties(item_motion_test PROPERTIES LABELS "misja-2")

add_executable(plant_test plant_test.cpp)
target_link_libraries(plant_test PRIVATE psm_core)
add_test(NAME plant_test COMMAND plant_test)
set_tests_properties(plant_test PROPERTIES LABELS "misja-3")

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
set_tests_properties(plant_diverter_test PROPERTIES LABELS "misja-9")

add_executable(tick_result_test tick_result_test.cpp)
target_link_libraries(tick_result_test PRIVATE psm_core)
add_test(NAME tick_result_test COMMAND tick_result_test)
set_tests_properties(tick_result_test PROPERTIES LABELS "misja-10")

add_executable(engine_test engine_test.cpp)
target_link_libraries(engine_test PRIVATE psm_core)
add_test(NAME engine_test COMMAND engine_test)
set_tests_properties(engine_test PROPERTIES LABELS "misja-11")

add_test(NAME simulator_cli_smoke COMMAND simulator_cli)
set_tests_properties(simulator_cli_smoke PROPERTIES LABELS "misja-6")

add_test(NAME toolchain_check_smoke COMMAND toolchain_check)
