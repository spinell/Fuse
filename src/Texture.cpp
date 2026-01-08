#include "Texture.h"


Texture::Texture(GLsizei /*width*/, GLsizei /*height*/, void* /*data*/) {
    glCreateTextures(GL_TEXTURE_2D, 1, &mId);
    glTextureParameteri(mId, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(mId, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(mId, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(mId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureStorage2D(mId, 1, GL_RGBA8, 2, 2);

    unsigned char pixels[2][2][4];
    pixels[0][0][0] = 0xFF;
    pixels[0][0][1] = 0xFF;
    pixels[0][0][2] = 0xFF;
    pixels[0][0][3] = 0xFF;

    pixels[0][1][0] = 0x00;
    pixels[0][1][1] = 0x00;
    pixels[0][1][2] = 0x00;
    pixels[0][1][3] = 0xFF;


    pixels[1][0][0] = 0x00;
    pixels[1][0][1] = 0x0;
    pixels[1][0][2] = 0x00;
    pixels[1][0][3] = 0xFF;

    pixels[1][1][0] = 0xFF;
    pixels[1][1][1] = 0xFF;
    pixels[1][1][2] = 0xFF;
    pixels[1][1][3] = 0xFF;
    glTextureSubImage2D(mId, 0, 0, 0, 2, 2, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glGenerateTextureMipmap(mId);


    // create a new immutable data store
    // The size of the data store is specified by size
    // If an initial data is available, its address may be supplied in data.
    // Otherwise, to create an uninitialized data store, data should be NULL.
    // The flags parameters specifies the intended usage of the buffer's data store.
    //glNamedBufferStorage(mId, size, data, flags);
}

Texture::~Texture() { glDeleteTextures(1, &mId); }


void Texture::bind() {}
void Texture::unbind() {}
