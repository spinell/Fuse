find_package(GTest 1.17.0 EXACT CONFIG REQUIRED)

add_executable(TestFuseCore
    TestMat2.cpp
    TestMat3.cpp
    TestMat4.cpp
)

target_link_libraries(TestFuseCore
    PRIVATE
        GTest::gmock_main
        Fuse::Lib
)

add_test(NAME Fuse::lib COMMAND TestFuseCore)
