#
# This file contains utility functions for printing.
#
include_guard()

include(FuseStringUtils)

####################################################################
# Print multiple variables aligned
#
# Example:
#   set(VAR "Hello")
#   set(SUPER_LONG_VAR "Hello World")
#   set(ANOTHER_VAR "Toto")
#   fuse_print_aligned_variables(VAR SUPER_LONG_VAR ANOTHER_VAR NOT_EXIST_VAR)
#
# Result:
#     VAR            : Hello
#     SUPER_LONG_VAR : Hello World
#     ANOTHER_VAR    : Toto
#     NOT_EXIST_VAR  : <not defined>
#
function(fuse_print_aligned_variables)
    # compute the max length
    fuse_get_greater_string_length(MAX_VARIABLE_LENGTH ${ARGN})

    # print all variables
    foreach(var ${ARGN})
        string(LENGTH ${var} VAR_NAME_LENGTH)

        math(EXPR PADDING_LENGTH "${MAX_VARIABLE_LENGTH} - ${VAR_NAME_LENGTH}" OUTPUT_FORMAT DECIMAL)
        string(REPEAT "." ${PADDING_LENGTH} MSG2)

        set(MSG ${var})
        string(APPEND MSG ${MSG2})

        if(DEFINED ${var})
            message(STATUS "${MSG} : ${${var}}")
        else()
            message(STATUS "${MSG} : <not defined>")
        endif()

    endforeach()

endfunction(fuse_print_aligned_variables)

# Print CMake variable related to the generator used.
function(fuse_print_system_info)
    message(STATUS "System")
    list(APPEND CMAKE_MESSAGE_INDENT "  ")
    fuse_print_aligned_variables(
        CMAKE_HOST_SYSTEM
        CMAKE_HOST_SYSTEM_NAME
        CMAKE_HOST_SYSTEM_PROCESSOR
        CMAKE_HOST_SYSTEM_VERSION
        CMAKE_HOST_APPLE
        CMAKE_HOST_LINUX
        CMAKE_HOST_UNIX
        CMAKE_HOST_WIN32
        CMAKE_SYSTEM
        CMAKE_SYSTEM_NAME
        CMAKE_SYSTEM_PROCESSOR
        CMAKE_SYSTEM_VERSION
    )
endfunction(fuse_print_system_info)

# Print CMake variable related to the generator used.
function(fuse_print_generator_info)
    message(STATUS "Generator")
    list(APPEND CMAKE_MESSAGE_INDENT "  ")
    fuse_print_aligned_variables(
        CMAKE_GENERATOR
        CMAKE_GENERATOR_INSTANCE
        CMAKE_GENERATOR_PLATFORM
        CMAKE_GENERATOR_TOOLSET
    )
endfunction(fuse_print_generator_info)


# Print CMake variable related to the compiler used.
function(fuse_print_compiler_info)
    message(STATUS "Compiler")

    list(APPEND CMAKE_MESSAGE_INDENT "  ")

    fuse_print_aligned_variables(
        CMAKE_CXX_COMPILER
        CMAKE_CXX_COMPILER_VERSION
        CMAKE_CXX_COMPILER_ID
        # "simulated" compiler (this variable describes the ABI compatibility of the generated code).
        CMAKE_CXX_SIMULATE_ID
        CMAKE_CXX_SIMULATE_VERSION
        # this variable describes what command line options and language extensions the compiler frontend expects.
        CMAKE_CXX_COMPILER_FRONTEND_VARIANT
        # target architecture(s) of the compiler
        CMAKE_CXX_COMPILER_ARCHITECTURE_ID
    )

    list(POP_BACK  CMAKE_MESSAGE_INDENT)
    message(STATUS "Linker")
    list(APPEND CMAKE_MESSAGE_INDENT "  ")

    fuse_print_aligned_variables(
        CMAKE_CXX_COMPILER_LINKER
        CMAKE_CXX_COMPILER_LINKER_VERSION
        CMAKE_CXX_COMPILER_LINKER_ID
        CMAKE_CXX_COMPILER_LINKER_FRONTEND_VARIANT
        CMAKE_CXX_COMPILER_RANLIB
        CMAKE_LINKER_TYPE
    )
endfunction(fuse_print_compiler_info)


# Print CMake variable related to compiler flags.
function(fuse_print_compiler_flags)
    message(STATUS "Compiler flags")

    list(APPEND CMAKE_MESSAGE_INDENT "  ")

    fuse_print_aligned_variables(
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_INIT
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_DEBUG_INIT
        CMAKE_CXX_FLAGS_RELWITHDEBINFO
        CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT
        CMAKE_CXX_FLAGS_MINSIZEREL
        CMAKE_CXX_FLAGS_MINSIZEREL_INIT
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_RELEASE_INIT
    )
endfunction(fuse_print_compiler_flags)

# Print CMake variable related to linker flags.
function(fuse_print_linker_flags)
    message(STATUS "Linker flags")

    list(APPEND CMAKE_MESSAGE_INDENT "  ")

    fuse_print_aligned_variables(
        # Static
        CMAKE_STATIC_LINKER_FLAGS
        CMAKE_STATIC_LINKER_FLAGS_INIT
        CMAKE_STATIC_LINKER_FLAGS_DEBUG
        CMAKE_STATIC_LINKER_FLAGS_DEBUG_INIT
        CMAKE_STATIC_LINKER_FLAGS_RELWITHDEBINFO
        CMAKE_STATIC_LINKER_FLAGS_RELWITHDEBINFO_INIT
        CMAKE_STATIC_LINKER_FLAGS_MINSIZEREL
        CMAKE_STATIC_LINKER_FLAGS_MINSIZEREL_INIT
        CMAKE_STATIC_LINKER_FLAGS_RELEASE
        CMAKE_STATIC_LINKER_FLAGS_RELEASE_INIT
        # Dynamic
        CMAKE_SHARED_LINKER_FLAGS
        CMAKE_SHARED_LINKER_FLAGS_INIT
        CMAKE_SHARED_LINKER_FLAGS_DEBUG
        CMAKE_SHARED_LINKER_FLAGS_DEBUG_INIT
        CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO
        CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO_INIT
        CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL
        CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL_INIT
        CMAKE_SHARED_LINKER_FLAGS_RELEASE
        CMAKE_SHARED_LINKER_FLAGS_RELEASE_INIT
        # Module
        CMAKE_MODULE_LINKER_FLAGS
        CMAKE_MODULE_LINKER_FLAGS_INIT
        CMAKE_MODULE_LINKER_FLAGS_DEBUG
        CMAKE_MODULE_LINKER_FLAGS_DEBUG_INIT
        CMAKE_MODULE_LINKER_FLAGS_RELWITHDEBINFO
        CMAKE_MODULE_LINKER_FLAGS_RELWITHDEBINFO_INIT
        CMAKE_MODULE_LINKER_FLAGS_MINSIZEREL
        CMAKE_MODULE_LINKER_FLAGS_MINSIZEREL_INIT
        CMAKE_MODULE_LINKER_FLAGS_RELEASE
        CMAKE_MODULE_LINKER_FLAGS_RELEASE_INIT
        # Executable
        CMAKE_EXE_LINKER_FLAGS
        CMAKE_EXE_LINKER_FLAGS_INIT
        CMAKE_EXE_LINKER_FLAGS_DEBUG
        CMAKE_EXE_LINKER_FLAGS_DEBUG_INIT
        CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO
        CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO_INIT
        CMAKE_EXE_LINKER_FLAGS_MINSIZEREL
        CMAKE_EXE_LINKER_FLAGS_MINSIZEREL_INIT
        CMAKE_EXE_LINKER_FLAGS_RELEASE
        CMAKE_EXE_LINKER_FLAGS_RELEASE_INIT
    )

endfunction(fuse_print_linker_flags)
