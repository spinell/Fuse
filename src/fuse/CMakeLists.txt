
add_library(Fuse)
add_library(Fuse::Fuse ALIAS Fuse)

fuse_target_set_compiler_warnings(Fuse)

# The SDL INTERFACE_SYSTEM_INCLUDE_DIRECTORIES is not propagated ....
get_target_property(sdl_include_dir SDL3::Headers INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(Fuse SYSTEM PRIVATE ${sdl_include_dir})

target_sources(Fuse
    PRIVATE
        src/Assert.cpp
        src/Application.cpp
        src/Timer.cpp
        src/LayerStack.cpp
        src/math/Angle.cpp
        src/math/Mat2.cpp
        src/math/Mat3.cpp
        src/math/Mat4.cpp
        src/math/Quaternion.cpp
    PUBLIC
        FILE_SET HEADERS
        TYPE HEADERS
        BASE_DIRS
            ${CMAKE_CURRENT_BINARY_DIR}/generated
            ${CMAKE_CURRENT_SOURCE_DIR}/include
        FILES
            include/fuse/Assert.h
            include/fuse/Application.h
            include/fuse/Layer.h
            include/fuse/LayerStack.h
            include/fuse/Time.h
            include/fuse/Timer.h
            include/fuse/math/Angle.h
            include/fuse/math/Vec2.h
            include/fuse/math/Vec3.h
            include/fuse/math/Vec4.h
            include/fuse/math/Mat2.h
            include/fuse/math/Mat3.h
            include/fuse/math/Mat4.h
            include/fuse/math/Quaternion.h
)

target_link_libraries(Fuse
    PRIVATE
        SDL3::SDL3
        glad2::glad2
        ImGui::ImGui
)


# Group source/headers files for IDE to match the file system.
get_target_property(sources Fuse SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "Sources" FILES ${sources})
get_target_property(sources Fuse HEADER_SET)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/include/fuse PREFIX "Header Files" FILES ${sources})

#
# Install targets
#
include(GNUInstallDirs)

# Install fuse and it's dependencies.
# This is required for when fuse is build as a static lib.
# TODO: Find a way to not have to install dependencies when fuse is build as a static lib.
#       May use OBJECT library for the dependencies or
#       merge all static library in fuse.
install(
    TARGETS Fuse glad2 ImGui
    EXPORT FuseTargets
    RUNTIME
        DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FILE_SET HEADERS
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# install SDL3 config files
install(FILES $<TARGET_IMPORT_FILE:SDL3::SDL3> DESTINATION lib/)

# install SDL3 lib

# Install exported target
install(
    EXPORT FuseTargets
    NAMESPACE fuse::
    DESTINATION cmake
)

#
# Write config file
#
include(CMakePackageConfigHelpers)
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/FuseConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/FuseConfig.cmake
    INSTALL_DESTINATION cmake
)

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/FuseConfig.cmake"
        DESTINATION cmake
)


#
# Export build tree
#
export(
    EXPORT FuseTargets
    NAMESPACE fuse::
    FILE ${PROJECT_BINARY_DIR}/FuseTargets-build.cmake
)
