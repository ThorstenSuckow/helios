project(demo LANGUAGES C CXX)

file(GLOB_RECURSE EXAMPLE_FILES "*.cpp")

file(GLOB_RECURSE EXT_CPP_FILES CONFIGURE_DEPENDS
        # helios base impl
        "${CMAKE_CURRENT_SOURCE_DIR}/../ext/src/**/*.cpp")
file(GLOB_RECURSE EXT_MODULE_FILES CONFIGURE_DEPENDS
        # helios base impl
        "${CMAKE_CURRENT_SOURCE_DIR}/../ext/src/**/*.ixx")

list(FILTER CPP_FILES EXCLUDE REGEX "/CMakeFiles/|/CMakeTmp/|/cmake_build_|/\\.build/")
list(FILTER MODULE_INTERFACE_FILES EXCLUDE REGEX "/CMakeFiles/|/CMakeTmp/|/cmake_build_|/\\.build/")

foreach(EXAMPLE_FILE ${EXAMPLE_FILES})
    file(RELATIVE_PATH REL_PATH ${CMAKE_CURRENT_SOURCE_DIR} ${EXAMPLE_FILE})
    get_filename_component(SUBDIR ${REL_PATH} DIRECTORY)
    get_filename_component(TARGET_NAME ${EXAMPLE_FILE} NAME_WE)

    string(REPLACE "/" "" UNIQUE_TARGET ${REL_PATH})
    string(REPLACE ".cpp" "" UNIQUE_TARGET ${UNIQUE_TARGET})

    add_executable(${UNIQUE_TARGET} ${EXAMPLE_FILE})

    target_sources(${UNIQUE_TARGET} PRIVATE ${EXT_CPP_FILES})

    if(EXT_MODULE_FILES)
        target_sources(${UNIQUE_TARGET} PUBLIC
                FILE_SET modules TYPE CXX_MODULES
                BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../ext"
                FILES ${EXT_MODULE_FILES}
        )
    endif()


    target_link_libraries(${UNIQUE_TARGET} PRIVATE
            helios
            thirdparty::opengl
    )

    target_compile_definitions(${UNIQUE_TARGET} PRIVATE GLFW_INCLUDE_NONE)

    set_target_properties(${UNIQUE_TARGET} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples/${SUBDIR}
        OUTPUT_NAME ${TARGET_NAME}
    )

    set(RESOURCES_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${SUBDIR}/resources")
    set(RESOURCES_DEST_DIR "${CMAKE_BINARY_DIR}/examples/${SUBDIR}/resources")

    if(EXISTS "${RESOURCES_SOURCE_DIR}")
        add_custom_command(
                TARGET ${UNIQUE_TARGET}
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                    "${RESOURCES_SOURCE_DIR}"
                    "${RESOURCES_DEST_DIR}"
        )
    endif()

endforeach()


