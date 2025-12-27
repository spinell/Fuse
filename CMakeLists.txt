cmake_minimum_required(VERSION 4.0.0)

# Ensure non-empty default build type for single-config
get_property(isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(NOT isMultiConfig AND NOT CMAKE_BUILD_TYPE)
    message(FATAL_ERROR "CMAKE_BUILD_TYPE is empty.")
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)
include(PreventInSourceBuild)

project(Fuse VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Print information (Can be useful for debugging)
include(FusePrintUtils)
fuse_print_system_info()
fuse_print_generator_info()
fuse_print_compiler_info()
fuse_print_compiler_flags()
fuse_print_linker_flags()

add_subdirectory(externals)
add_subdirectory(src)

set_directory_properties(PROPERTIES
    VS_STARTUP_PROJECT Fuse
)
