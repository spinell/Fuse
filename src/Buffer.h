#pragma once
#include <glad/gl.h>

#include <algorithm>


/// @brief Thing wrapper around OpenGL Buffer.
class Buffer {
public:
    Buffer() = default;
    Buffer(GLsizeiptr size, void* data);
    ~Buffer();

    Buffer(const Buffer&)            = delete;
    Buffer& operator=(const Buffer&) = delete;

    Buffer(Buffer&& o) { o.mId = std::exchange(mId, o.mId); }

    Buffer& operator=(Buffer&& o) {
        o.mId = std::exchange(mId, o.mId);
        return *this;
    }

    void bind();
    void unbind();

    GLuint getId() const { return mId; }

private:
    GLuint mId{0};
};
