find_package(GTest 1.17.0 EXACT CONFIG REQUIRED)

add_executable(TestFuseCore
    GTestUtils.h
    GTestUtils.cpp
    TestAngle.cpp
    TestVec2.cpp
    TestVec3.cpp
    TestVec4.cpp
    TestMat2.cpp
    TestMat3.cpp
    TestMat4.cpp
    TestQuaternion.cpp
)

fuse_target_set_compiler_flags(TestFuseCore)

target_link_libraries(TestFuseCore
    PRIVATE
        GTest::gmock_main
        Fuse::Lib
)

add_test(NAME Fuse::lib COMMAND TestFuseCore)
