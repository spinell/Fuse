#pragma once
#include <glad/gl.h>

namespace fuse {
class Mat4;
struct Vec4;
} // namespace fuse

struct Vec4;

/// @brief Thing wrapper around OpenGL Shader.
class Shader {
public:
    Shader();
    ~Shader();

    Shader(const Shader&)            = delete;
    Shader& operator=(const Shader&) = delete;

    void bind();
    void unbind();

    void setVector(const char* name, const fuse::Vec4&);
    void setMatrix(const char* name, const fuse::Mat4&);

private:
    GLint getUniformLocation(const char* uniformName) const;

    GLuint mProgram{0};
};
