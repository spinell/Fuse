

add_library(glad2 STATIC)
add_library(glad2::glad2 ALIAS glad2)

# TODO: Don't install glad headers when OpenGL will be abstracted
target_sources(glad2
    PRIVATE
        src/gl.c
    PUBLIC
        FILE_SET HEADERS
        TYPE HEADERS
        BASE_DIRS
            ${CMAKE_CURRENT_SOURCE_DIR}/include
        FILES
            include/glad/gl.h
            include/KHR/khrplatform.h
)
