file(MAKE_DIRECTORY ${CMAKE_IFC_OUTPUT_DIRECTORY})

file(GLOB_RECURSE CPP_FILES CONFIGURE_DEPENDS
        # extension directory with implementations for glfw and OpenGL >= 4.6,
        # helios base impl
        "${CMAKE_CURRENT_SOURCE_DIR}/**/*.cpp")

file(GLOB_RECURSE MODULE_INTERFACE_FILES CONFIGURE_DEPENDS
        # extension directory with implementations for glfw and OpenGL >= 4.6,
        # helios base src
        "${CMAKE_CURRENT_SOURCE_DIR}/../include/**/*.ixx"
)


list(FILTER CPP_FILES EXCLUDE REGEX "/CMakeFiles/|/CMakeTmp/|/cmake_build_|/\\.build/")
list(FILTER MODULE_INTERFACE_FILES EXCLUDE REGEX "/CMakeFiles/|/CMakeTmp/|/cmake_build_|/\\.build/")

##########################
# create lib target
##########################
add_library(helios STATIC)
###
###


target_link_libraries(
        helios
        PUBLIC
        thirdparty::opengl
)

set_property(TARGET helios PROPERTY CXX_SCAN_FOR_MODULES ON)
target_compile_features(helios PUBLIC cxx_std_20)

target_sources(helios PRIVATE ${CPP_FILES})

if(MODULE_INTERFACE_FILES)
    target_sources(helios PUBLIC
            FILE_SET modules TYPE CXX_MODULES
            BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}"
            BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../include"
            FILES ${MODULE_INTERFACE_FILES}
    )
endif()

target_include_directories(helios PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../ext>
)