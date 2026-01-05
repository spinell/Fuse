
add_library(FuseLib
    math/Angle.h
    math/Angle.cpp
    math/Vec2.h
    math/Vec3.h
    math/Vec4.h
    math/Mat2.h
    math/Mat2.cpp
    math/Mat4.h
    math/Mat4.cpp
    math/Mat3.h
    math/Mat3.cpp
    math/Mat4.h
    math/Mat4.cpp
    math/Quaternion.cpp
    math/Quaternion.h

)
add_library(Fuse::Lib ALIAS FuseLib)

fuse_target_set_compiler_warnings(FuseLib)

target_include_directories(FuseLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
