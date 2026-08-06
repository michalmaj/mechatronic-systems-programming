add_executable(zone_test zone_test.cpp)
target_link_libraries(zone_test PRIVATE psm_core)
add_test(NAME zone_test COMMAND zone_test)

add_executable(value_types_test value_types_test.cpp)
target_link_libraries(value_types_test PRIVATE psm_core)
add_test(NAME value_types_test COMMAND value_types_test)
