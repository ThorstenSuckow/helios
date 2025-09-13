file(MAKE_DIRECTORY ${CMAKE_IFC_OUTPUT_DIRECTORY})

file(GLOB_RECURSE CPP_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/**/*.cpp")
file(GLOB_RECURSE MODULE_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/**/*.ixx")

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

if(CPP_FILES)
    target_sources(helios PRIVATE ${CPP_FILES})
endif()

if(MODULE_FILES)
    target_sources(helios PUBLIC
        FILE_SET modules TYPE CXX_MODULES
        FILES ${MODULE_FILES}
    )
endif()

target_include_directories(helios PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

