cmake_minimum_required(VERSION 4.0.0)

# Ensure non-empty default build type for single-config
get_property(isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(NOT isMultiConfig AND NOT CMAKE_BUILD_TYPE)
    message(FATAL_ERROR "CMAKE_BUILD_TYPE is empty.")
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)
include(PreventInSourceBuild)

project(Fuse VERSION 0.1.0 LANGUAGES C CXX)

###############################################
#               Project setup
###############################################
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CMakeDependentOption)
cmake_dependent_option(FUSE_BUILD_DOC     "Build doxygen documentation" ON PROJECT_IS_TOP_LEVEL OFF)
cmake_dependent_option(FUSE_BUILD_TESTS   "Build tests" ON PROJECT_IS_TOP_LEVEL OFF)

include(FusePrintUtils)
include(CompilerWarning)

# Print information (Can be useful for debugging)
fuse_print_system_info()
fuse_print_generator_info()
fuse_print_compiler_info()
fuse_print_compiler_flags()
fuse_print_linker_flags()


################################################
#               Dependencies
################################################
find_package(SDL3 3.2.28 CONFIG REQUIRED)
message(STATUS "SDL3_DIR     : " ${SDL3_DIR})
message(STATUS "SDL3_VERSION : " ${SDL3_VERSION})

################################################
#               Targets
################################################
add_subdirectory(externals SYSTEM)
add_subdirectory(src/fuse)
add_subdirectory(src/testbed)

set_directory_properties(PROPERTIES VS_STARTUP_PROJECT Fuse)

if(FUSE_BUILD_TESTS)
    find_package(GTest 1.17.0 EXACT CONFIG REQUIRED)

    enable_testing()
    add_subdirectory(tests)
endif()

if(FUSE_BUILD_DOC)
    message(STATUS "Building documentation.")
    add_subdirectory(docs)
else()
    message(STATUS "Skipping documentation")
endif()


# Add a custom target to run clang-format on all source files
add_custom_target(
    ClangFormat
    COMMAND ${CMAKE_COMMAND} -DCLANG_FORMAT_EXECUTABLE=${CLANG_FORMAT_EXECUTABLE} -P ${CMAKE_SOURCE_DIR}/scripts/RunClangFormat.cmake
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    VERBATIM
    COMMENT "Running clang-format on source files"
)
