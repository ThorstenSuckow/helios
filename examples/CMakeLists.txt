project(demo LANGUAGES C CXX)

include(FetchContent)

FetchContent_Declare(glad
        GIT_REPOSITORY https://github.com/Dav1dde/glad.git
        GIT_TAG        v2.0.8
        SOURCE_SUBDIR  cmake
)
FetchContent_Declare(glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG        3.4
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glad glfw)

glad_add_library(glad_gl_core_46 STATIC REPRODUCIBLE LOADER API gl:core=4.6)
target_link_libraries(glad_gl_core_46 glfw)

file(GLOB_RECURSE EXAMPLE_FILES "*.cpp")

foreach(EXAMPLE_FILE ${EXAMPLE_FILES})
    file(RELATIVE_PATH REL_PATH ${CMAKE_CURRENT_SOURCE_DIR} ${EXAMPLE_FILE})
    get_filename_component(SUBDIR ${REL_PATH} DIRECTORY)
    get_filename_component(TARGET_NAME ${EXAMPLE_FILE} NAME_WE)

    string(REPLACE "/" "" UNIQUE_TARGET ${REL_PATH})
    string(REPLACE ".cpp" "" UNIQUE_TARGET ${UNIQUE_TARGET})

    add_executable(${UNIQUE_TARGET} ${EXAMPLE_FILE})

    target_link_libraries(${UNIQUE_TARGET} PRIVATE
            helios
            benchmark::benchmark
            glad_gl_core_46
            glfw
    )

    target_compile_definitions(${UNIQUE_TARGET} PRIVATE GLFW_INCLUDE_NONE)

    set_target_properties(${UNIQUE_TARGET} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/demos/${SUBDIR}
        OUTPUT_NAME ${TARGET_NAME}
    )
endforeach()


