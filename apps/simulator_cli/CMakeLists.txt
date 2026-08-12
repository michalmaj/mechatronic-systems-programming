add_executable(simulator_cli main.cpp scenario_demos.cpp)
target_link_libraries(simulator_cli PRIVATE psm_core)
