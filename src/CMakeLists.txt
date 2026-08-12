add_library(psm_core)
target_sources(psm_core PRIVATE
    zone.cpp plant.cpp controller.cpp diverter.cpp
    tick_result.cpp engine.cpp belt_motor.cpp mode.cpp
    estop_latch.cpp safety_supervisor.cpp
    presence_sensor.cpp weight_sensor.cpp scenario.cpp)
target_include_directories(psm_core PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_compile_features(psm_core PUBLIC cxx_std_20)
