add_library(psm_core)
target_sources(psm_core PRIVATE
    zone.cpp controller.cpp plant.cpp diverter.cpp belt_motor.cpp engine.cpp
    estop_latch.cpp mode.cpp scenario_input.cpp safety_supervisor.cpp
    scripted_fault.cpp presence_sensor.cpp weight_sensor.cpp reading_status.cpp)
target_include_directories(psm_core PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_compile_features(psm_core PUBLIC cxx_std_20)
