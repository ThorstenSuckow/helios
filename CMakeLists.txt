cmake_minimum_required(VERSION 4.0)
project(helios LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG        3.4
)

FetchContent_Declare(glad
        GIT_REPOSITORY https://github.com/Dav1dde/glad.git
        GIT_TAG        v2.0.8
        SOURCE_SUBDIR  cmake
)
FetchContent_MakeAvailable(glfw glad)

# GLAD v2: Core GL 4.6
glad_add_library(glad_gl_core_46 STATIC REPRODUCIBLE LOADER API gl:core=4.6)
add_library(thirdparty::glad ALIAS glad_gl_core_46)
add_library(thirdparty::glfw ALIAS glfw)
add_library(thirdparty_opengl INTERFACE)
target_link_libraries(thirdparty_opengl INTERFACE thirdparty::glad thirdparty::glfw)
add_library(thirdparty::opengl ALIAS thirdparty_opengl)


add_subdirectory(src)

option(BUILD_TESTING "Build tests" ON)
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

option(BUILD_BENCHMARKS "Build benchmarks" ON)
if(BUILD_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()

option(BUILD_EXAMPLES "Build examples" ON)
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
