include(FetchContent)
FetchContent_Declare(
    benchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.9.4

    CMAKE_ARGS -DCMAKE_CXX_SCAN_FOR_MODULES=OFF
)
FetchContent_MakeAvailable(benchmark)

file(GLOB_RECURSE BENCHMARK_FILES "*.benchmark.cpp")

foreach(BENCHMARK_FILE ${BENCHMARK_FILES})
    file(RELATIVE_PATH REL_PATH ${CMAKE_CURRENT_SOURCE_DIR} ${BENCHMARK_FILE})
    get_filename_component(SUBDIR ${REL_PATH} DIRECTORY)
    get_filename_component(TARGET_NAME ${BENCHMARK_FILE} NAME_WE)

    string(REPLACE "/" "" UNIQUE_TARGET ${REL_PATH})
    string(REPLACE ".benchmark.cpp" "" UNIQUE_TARGET ${UNIQUE_TARGET})

    add_executable(${UNIQUE_TARGET} ${BENCHMARK_FILE})

    target_link_libraries(${UNIQUE_TARGET} PRIVATE helios benchmark::benchmark)

    set_target_properties(${UNIQUE_TARGET} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/benchmarks/${SUBDIR}
        OUTPUT_NAME ${TARGET_NAME}
    )
endforeach()


