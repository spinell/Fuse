
add_subdirectory(lib)

find_package(SDL3 3.2.28 CONFIG REQUIRED)
message(STATUS "SDL3_DIR     : " ${SDL3_DIR})
message(STATUS "SDL3_VERSION : " ${SDL3_VERSION})


add_executable(Fuse main.cpp)
fuse_target_set_compiler_flags(Fuse)

target_link_libraries(Fuse
    PRIVATE
        SDL3::SDL3
        glad2::glad2
        ImGui::ImGui
        Fuse::Lib
)
install(TARGETS Fuse DESTINATION .)
