
add_executable(FuseTestbed
    main.cpp
    Buffer.cpp
    Buffer.h
    Texture.cpp
    Texture.h
    Shader.cpp
    Shader.h
    Camera.h
    Camera.cpp
    ImGui.h
    ImGui.cpp
    GeometryGenerator.h
    GeometryGenerator.cpp
    TextureGenerator.h
    TextureGenerator.cpp
    Log.h
    Log.cpp
    Mesh.h
    Mesh.cpp
    Layers/TestLayer.h
    Layers/TestLayer.cpp
)
fuse_target_set_compiler_warnings(FuseTestbed)

target_link_libraries(FuseTestbed
    PRIVATE
        SDL3::SDL3
        glad2::glad2
        ImGui::ImGui
        Fuse::Fuse
        stb::image
)

# The SDL INTERFACE_SYSTEM_INCLUDE_DIRECTORIES is not propagated ....
get_target_property(sdl_include_dir SDL3::Headers INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(FuseTestbed SYSTEM PRIVATE ${sdl_include_dir})

install(TARGETS FuseTestbed DESTINATION .)
