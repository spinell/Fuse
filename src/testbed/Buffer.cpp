#include "Buffer.h"


Buffer::Buffer(GLsizeiptr size, void* data) {
    glCreateBuffers(1, &mId);

    glNamedBufferData(mId, size, data, GL_STATIC_DRAW);
}

Buffer::~Buffer() { glDeleteBuffers(1, &mId); }


void Buffer::bind() {}
void Buffer::unbind() {}
