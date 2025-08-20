include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0

    CMAKE_ARGS -DCMAKE_CXX_SCAN_FOR_MODULES=OFF
)

FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_LIST_DIR}/**/*.test.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/*.test.cpp"
)

add_executable(helios_tests
    ${TEST_SOURCES}
)

target_link_libraries(helios_tests PRIVATE
    helios
    GTest::gtest_main
)

target_compile_features(helios_tests PRIVATE cxx_std_20)

include(GoogleTest)
gtest_discover_tests(helios_tests
    DISCOVERY_TIMEOUT 30
)