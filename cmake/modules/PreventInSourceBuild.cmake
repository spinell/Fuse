#
# Prevent in source build
#

# Raise a error if the build directory is the same has the source directory.
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "In-source builds are discouraged and therefore prevented here!.\n"
                        " CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}\n"
                        " CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}\n"
                        " * Delete ${CMAKE_SOURCE_DIR}/CMakeCache.txt\n"
                        " * Delete ${CMAKE_SOURCE_DIR}/CMakeFiles/ directory.\n")
endif()

# Raise a error if the install directory is the same has the source directory.
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_INSTALL_PREFIX)
    message(FATAL_ERROR "The current CMAKE_INSTALL_PREFIX points at the build tree:\n"
                        " * ${CMAKE_INSTALL_PREFIX}\n"
                        " Chose another install directory.")
endif()
