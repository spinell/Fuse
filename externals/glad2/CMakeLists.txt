

add_library(glad2 STATIC)
add_library(glad2::glad2 ALIAS glad2)

target_sources(
    glad2
    PRIVATE
        include/glad/gl.h
        include/KHR/khrplatform.h
        src/gl.c
)

target_include_directories(glad2 PUBLIC include)
