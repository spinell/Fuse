################################################################################
# Run Clang-format on the sources files
#
# Call this script cmake in script mode (cmake -P)
#
# By default the script will find clang-format with find_program()
# you can specify a full path to use with -DCLANG_FORMAT_EXECUTABLE=...
#
# Warning: This script must be called from the root of the repository
#
################################################################################

SET(CLANG_FORMAT_VERSION_REQUIRED 19) # The clang-format version required

# Make sure we are in cmake script mode
get_cmake_property(CMAKE_ROLE_VALUE CMAKE_ROLE)
if(NOT ${CMAKE_ROLE_VALUE} STREQUAL "SCRIPT")
    message(FATAL_ERROR "This script must to run in cmake script mode!\n Run this script with cmake -P <script_path>")
endif()

#
# Try to find clang-format
#
if(DEFINED CLANG_FORMAT_EXECUTABLE AND NOT EXISTS ${CLANG_FORMAT_EXECUTABLE})
    message(FATAL_ERROR "CLANG_FORMAT_EXECUTABLE is defined by can't be found '${CLANG_FORMAT_EXECUTABLE}'.")
endif()

# clang-format was not specified with -DCLANG_FORMAT_EXECUTABLE=...
# try to find it ...
if(NOT DEFINED CLANG_FORMAT_EXECUTABLE)
    find_program(CLANG_FORMAT_EXECUTABLE
        NAMES
            clang-format-${CLANG_FORMAT_VERSION_REQUIRED}
            clang-format
    )
    if(CLANG_FORMAT_EXECUTABLE-NOTFOUND)
        message(FATAL_ERROR "clang-format not found!")
    endif()
    message(STATUS "Found clang-format in:  ${CLANG_FORMAT_EXECUTABLE}")
else()
    message(STATUS "Using clang-format in:  ${CLANG_FORMAT_EXECUTABLE}")
endif()


#
# Check clang-format version
#
execute_process(COMMAND ${CLANG_FORMAT_EXECUTABLE} --version OUTPUT_VARIABLE CLANG_FORMAT_VERSION_FOUND)
string(REGEX MATCH "clang-format version ([0-9]+)" CLANG_FORMAT_VERSION_FOUND ${CLANG_FORMAT_VERSION_FOUND})
unset(CLANG_FORMAT_VERSION_FOUND)
if(NOT CMAKE_MATCH_1 EQUAL ${CLANG_FORMAT_VERSION_REQUIRED})
    message(FATAL_ERROR "clang-format version ${CMAKE_MATCH_1} not supported. Must use version ${CLANG_FORMAT_VERSION_REQUIRED}")
endif()


#
# Collect all the source all in SOURCES_FILES
#
set(SOURCES_FILES "")
foreach(FOLDER IN ITEMS src tests sandbox)
    # grab all source in the folder 'FOLDER'
    file(GLOB_RECURSE folder_files
         LIST_DIRECTORIES false
            "${FOLDER}/*.h"
            "${FOLDER}/*.hpp"
            "${FOLDER}/*.c"
            "${FOLDER}/*.cpp"
            "${FOLDER}/*.inl"
            "${FOLDER}/*.mm"
            "${FOLDER}/*.m"
    )
    list(APPEND SOURCES_FILES ${folder_files})
endforeach()

if(NOT SOURCES_FILES)
    message(FATAL_ERROR "No source file found.")
endif()


execute_process(COMMAND ${CLANG_FORMAT_EXECUTABLE}
    -i
    --style=file
    --Werror
    --verbose
    ${SOURCES_FILES}
)
