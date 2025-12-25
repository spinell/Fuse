#
# This file contains utility functions.
#
include_guard()


#
# Compute the maximum length of multiple string.
#  - result : Will contains the length of the greater string.
# Example:
#
# fuse_get_greater_string_length(max_length "Hello" "Cat") will return 5
#
function(fuse_get_greater_string_length result)
    set(MAX_VARIABLE_LENGTH 0)
    foreach(var ${ARGN})
        string(LENGTH ${var} VAR_NAME_LENGTH)
        if(VAR_NAME_LENGTH GREATER MAX_VARIABLE_LENGTH)
            set(MAX_VARIABLE_LENGTH ${VAR_NAME_LENGTH})
        endif()
    endforeach()

    set(${result} ${MAX_VARIABLE_LENGTH})
    return(PROPAGATE ${result})
endfunction(fuse_get_greater_string_length)
