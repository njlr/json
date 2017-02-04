cmake_minimum_required(VERSION 3.7)

add_subdirectory(thirdparty/benchmark)

add_executable(json_benchmark "src/json_benchmarks.cpp")
target_link_libraries(json_benchmark benchmark)
target_link_libraries(json_benchmark ${JSON_TARGET_NAME})

# we need extra libraries on Windows
if(WIN32)
  find_library(SHLWAPI Shlwapi.lib) 
  target_link_libraries(benchmark ${SHLWAPI})
endif()

set_target_properties(json_benchmark PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
    COMPILE_DEFINITIONS "$<$<CXX_COMPILER_ID:MSVC>:_SCL_SECURE_NO_WARNINGS>"
    COMPILE_OPTIONS "$<$<CXX_COMPILER_ID:MSVC>:/EHsc;$<$<CONFIG:Release>:/Od>>"
)

add_custom_target(do_benchmark
    COMMAND json_benchmark --benchmark_min_time=2 --benchmark_out=${CMAKE_BINARY_DIR}/benchmarks.csv --benchmark_out_format=csv
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/benchmarks
)
