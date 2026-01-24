set(CMAKE_FOLDER "external")

add_subdirectory(glad2)

find_package(SDL3 3.2.28 CONFIG REQUIRED)
message(STATUS "SDL3_DIR     : " ${SDL3_DIR})
message(STATUS "SDL3_VERSION : " ${SDL3_VERSION})

add_library(ImGui STATIC)
add_library(ImGui::ImGui ALIAS ImGui)
target_sources(
    ImGui
    PRIVATE
        imgui/imgui.cpp
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_internal.h
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        imgui/imstb_rectpack.h
        imgui/imstb_textedit.h
        imgui/imstb_truetype.h
        imgui/backends/imgui_impl_opengl3.h
        imgui/backends/imgui_impl_opengl3.cpp
        imgui/backends/imgui_impl_sdl3.h
        imgui/backends/imgui_impl_sdl3.cpp
    PUBLIC
        FILE_SET HEADERS
        TYPE HEADERS
        BASE_DIRS
        FILES
            imgui/imgui.h
            imgui/imgui_internal.h
            imgui/imconfig.h
)

target_include_directories(ImGui
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/imgui>
)


target_link_libraries(ImGui PUBLIC SDL3::SDL3)

#
# STB
#
add_library(stb STATIC)
add_library(stb::image ALIAS stb)
target_sources(
    stb
    PRIVATE
        stb_build.cpp
        stb/stb_image.h
        stb/stb_image_write.h
)

target_include_directories(stb
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/stb
)
