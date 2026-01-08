#pragma once
#include <glad/gl.h>

#include <algorithm>
#include <utility>


/// @brief Thing wrapper around OpenGL Texture.
class Texture {
public:
    Texture() = default;
    Texture(GLsizei width, GLsizei height, void* data);
    ~Texture();

    Texture(const Texture&)            = delete;
    Texture& operator=(const Texture&) = delete;

    Texture(Texture&& o) { o.mId = std::exchange(mId, o.mId); }

    Texture& operator=(Texture&& o) {
        o.mId = std::exchange(mId, o.mId);
        return *this;
    }

    void bind();
    void unbind();

    GLuint getId() const { return mId; }

private:
    GLuint mId{0};
};
