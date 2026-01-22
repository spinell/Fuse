set(CMAKE_FOLDER "tests")

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

fuse_target_set_compiler_warnings(TestFuseCore)

target_link_libraries(TestFuseCore
    PRIVATE
        GTest::gmock_main
        Fuse::Fuse
)

add_test(NAME Fuse::lib COMMAND TestFuseCore)
