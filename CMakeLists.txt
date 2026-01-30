cmake_minimum_required(VERSION 4.0)
project(helios LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# glfw
set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG        3.4
)

# Freetype
set(FT_DISABLE_ZLIB        ON  CACHE BOOL "" FORCE)
set(FT_DISABLE_PNG         ON  CACHE BOOL "" FORCE)
set(FT_DISABLE_BZIP2       ON  CACHE BOOL "" FORCE)
set(FT_DISABLE_BROTLI      ON  CACHE BOOL "" FORCE)
set(FT_DISABLE_HARFBUZZ    ON  CACHE BOOL "" FORCE)
set(FT_DISABLE_FREETYPE_DEBUGGER ON CACHE BOOL "" FORCE)

FetchContent_Declare(freetype
        GIT_REPOSITORY https://github.com/freetype/freetype.git
        GIT_TAG        VER-2-14-1
)


# glad
FetchContent_Declare(glad
        GIT_REPOSITORY https://github.com/Dav1dde/glad.git
        GIT_TAG        v2.0.8
        SOURCE_SUBDIR  cmake
)

#imgui
FetchContent_Declare(imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG        docking
)
FetchContent_MakeAvailable(freetype glfw glad imgui)

# GLAD v2: Core GL 4.6
glad_add_library(glad_gl_core_46 STATIC REPRODUCIBLE LOADER API gl:core=4.6)
add_library(thirdparty::glad ALIAS glad_gl_core_46)
add_library(thirdparty::freetype ALIAS freetype)
add_library(thirdparty::glfw ALIAS glfw)
add_library(thirdparty_opengl INTERFACE)
target_link_libraries(thirdparty_opengl INTERFACE thirdparty::glad thirdparty::glfw)
add_library(thirdparty::opengl ALIAS thirdparty_opengl)

# ImGui library
add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)
target_link_libraries(imgui PUBLIC thirdparty::glfw thirdparty::glad)
add_library(thirdparty::imgui ALIAS imgui)


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
