#include "Texture.h"

#include "TextureGenerator.h"

#include <cmath>
#include <vector>


Texture Texture::Create(unsigned width, unsigned height, unsigned mipmap) {
    Texture tex;
    glCreateTextures(GL_TEXTURE_2D, 1, &tex.mId);
    glTextureParameteri(tex.mId, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(tex.mId, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(tex.mId, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(tex.mId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Anisotropic filtering
    GLint maxAniso = 0;
    glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAniso);
    glTextureParameteri(tex.mId, GL_TEXTURE_MAX_ANISOTROPY, maxAniso);

    // compute the number of level in the texture (nbMipmap + 1).
    const unsigned level = [mipmap, width, height]() {
        if (mipmap == static_cast<unsigned>(-1)) {
            return static_cast<unsigned>(std::log2(std::max(width, height))) + 1;
        }
        return mipmap + 1;
    }();

    glTextureStorage2D(tex.mId, (GLsizei)level, GL_RGBA8, (GLsizei)width, (GLsizei)height);

    return tex;
}

Texture::Texture(unsigned width, unsigned height, void* data) {
    glCreateTextures(GL_TEXTURE_2D, 1, &mId);
    glTextureParameteri(mId, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(mId, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(mId, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(mId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Anisotropic filtering
    GLint maxAniso = 0;
    glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAniso);
    glTextureParameteri(mId, GL_TEXTURE_MAX_ANISOTROPY, maxAniso);

    const auto max = std::max(width, height);
    const auto nbLevel = GLsizei(std::log2(max)) + 1;
    glTextureStorage2D(mId, (GLsizei)nbLevel, GL_RGBA8, (GLsizei)width, (GLsizei)height);


    glTextureSubImage2D(mId, 0, 0, 0, (GLsizei)width, (GLsizei)height, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateTextureMipmap(mId);


    // create a new immutable data store
    // The size of the data store is specified by size
    // If an initial data is available, its address may be supplied in data.
    // Otherwise, to create an uninitialized data store, data should be NULL.
    // The flags parameters specifies the intended usage of the buffer's data store.
    //glNamedBufferStorage(mId, size, data, flags);
}

Texture::~Texture() { glDeleteTextures(1, &mId); }


void Texture::upload(unsigned mipmap, unsigned width, unsigned height, void* data) {
    glTextureSubImage2D(mId, (GLsizei)mipmap, 0, 0, (GLsizei )width, (GLsizei)height, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

void Texture::bind() {}
void Texture::unbind() {}


// ======================================================
//              Build texture generation
// ======================================================

Texture Texture::Create(const TextureGenerator::ImageData& data) {
    Texture texture(data.width, data.height, (void*)data.pixels.data());
    glTextureParameteri(texture.mId, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(texture.mId, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(texture.mId, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(texture.mId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(texture.mId, GL_TEXTURE_MAX_ANISOTROPY, 1);
    return texture;
}

Texture Texture::CreateBrick1() {
    return Create(TextureGenerator::generateBrickTexture1(1024, 1024));
}

Texture Texture::CreateBrick2() {
    return Create(TextureGenerator::generateBrickTexture2(1024, 1024));
}

Texture Texture::CreateBrick3() {
    return Create(TextureGenerator::generateBrickTexture3(1024, 1024));
}

Texture Texture::CreateBrick4(unsigned width, unsigned height, unsigned brickWidth,
                              unsigned brickHeight, unsigned mortarThickness) {
    return Create(TextureGenerator::generateBrickTexture4(width,
                                                        height,
                                                        brickWidth,
                                                        brickHeight,
                                                        mortarThickness));
}

Texture Texture::CreateGrass(unsigned width, unsigned height) {
    return Create(TextureGenerator::generateGrass(width, height));
}

Texture Texture::CreateCheckerboard(unsigned width, unsigned height, const Color color1,
                                    const Color color2, unsigned int squareSize) {
    const auto textureData =
      TextureGenerator::generateCheckerboard(width,
                                             height,
                                             {color1.r, color1.g, color1.b, color1.a},
                                             {color2.r, color2.g, color2.b, color2.a},
                                             squareSize);
    Texture texture = Create(textureData);
    glTextureParameteri(texture.mId, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(texture.mId, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(texture.mId, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(texture.mId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(texture.mId, GL_TEXTURE_MAX_ANISOTROPY, 1);
    return texture;
}


Texture Texture::CreateDebugWithMipmap() {
    auto texture = Texture::Create(1024, 1024, 5);

    // red
    auto mipmap0 = TextureGenerator::generateFlatImage(1024,1024, {225, 0, 0});
    texture.upload(0, mipmap0.width, mipmap0.height, (void*)mipmap0.pixels.data());

    // blue
    auto mipmap1 = TextureGenerator::generateFlatImage(512, 512, {0, 225, 0});
    texture.upload(1, mipmap1.width, mipmap1.height, (void*)mipmap1.pixels.data());

    // green
    auto mipmap2 = TextureGenerator::generateFlatImage(256, 256, {0, 0, 225});
    texture.upload(2, mipmap2.width, mipmap2.height, (void*)mipmap2.pixels.data());

    // Purple
    auto mipmap3 = TextureGenerator::generateFlatImage(128, 128, {255, 0, 255});
    texture.upload(3, mipmap3.width, mipmap3.height, (void*)mipmap3.pixels.data());

    // Cyan
    auto mipmap4 = TextureGenerator::generateFlatImage(64, 64, {0, 255, 255});
    texture.upload(4, mipmap4.width, mipmap4.height, (void*)mipmap4.pixels.data());

    // Yellow
    auto mipmap5 = TextureGenerator::generateFlatImage(32, 32, {255, 255, 0});
    texture.upload(5, mipmap5.width, mipmap5.height, (void*)mipmap5.pixels.data());

    return texture;
}
