add_library(psm_core)
target_sources(psm_core PRIVATE zone.cpp)
target_include_directories(psm_core PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_compile_features(psm_core PUBLIC cxx_std_20)
