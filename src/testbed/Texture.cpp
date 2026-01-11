#include "Texture.h"

#include "stb_image.h"
#include "TextureGenerator.h"
#include "Log.h"

#include <cassert>
#include <cmath>
#include <utility>
#include <vector>

namespace {

struct GLFormatDesc {
    GLenum internalFormat;
    GLenum format;
    GLenum type;
};

GLFormatDesc getGLFormat(PixelFormat format) {
    using PF = PixelFormat;
    switch (format) {
            // clang-format off
        case PF::R8_UNORM:         return { GL_R8,           GL_RED,  GL_UNSIGNED_BYTE };
        case PF::RG8_UNORM:        return { GL_RG8,          GL_RG,   GL_UNSIGNED_BYTE };
        case PF::RGB8_UNORM:       return { GL_RGB8,         GL_RGB,  GL_UNSIGNED_BYTE };
        case PF::RGBA8_UNORM:      return { GL_RGBA8,        GL_RGBA, GL_UNSIGNED_BYTE };
        case PF::R16_UNORM:        return { GL_R16,          GL_RED,  GL_UNSIGNED_SHORT };
        case PF::RG16_UNORM:       return { GL_RG16,         GL_RG,   GL_UNSIGNED_SHORT };
        case PF::RGB16_UNORM:      return { GL_RGB16,        GL_RGB,  GL_UNSIGNED_SHORT };
        case PF::RGBA16_UNORM:     return { GL_RGBA16,       GL_RGBA, GL_UNSIGNED_SHORT };
        case PF::RGB8_UNORM_SRGB:  return { GL_SRGB8,        GL_RGB,  GL_UNSIGNED_BYTE };
        case PF::RGBA8_UNORM_SRGB: return { GL_SRGB8_ALPHA8, GL_RGBA, GL_UNSIGNED_BYTE };
        case PF::R32F:             return { GL_R32F,         GL_RED,  GL_FLOAT };
        case PF::RG32F:            return { GL_RG32F,        GL_RG,   GL_FLOAT };
        case PF::RGB32F:           return { GL_RGB32F,       GL_RGB,  GL_FLOAT };
        case PF::RGBA32F:          return { GL_RGBA32F,      GL_RGBA, GL_FLOAT };
        case PF::Count:
        default: std::unreachable();
            // clang-format on
    }
}

} // namespace


Texture Texture::Create(const Texture2DCreateInfo& createInfo) {
    assert(createInfo.width >= 1);
    assert(createInfo.height >= 1);

    Texture texture{};
    texture.mCreateInfo = createInfo;
    glCreateTextures(GL_TEXTURE_2D, 1, &texture.mId);
    glObjectLabel(GL_TEXTURE, texture.mId, -1, createInfo.debugName);

    glTextureParameteri(texture.mId, GL_TEXTURE_WRAP_S, GL_REPEAT);     // default value ?
    glTextureParameteri(texture.mId, GL_TEXTURE_WRAP_T, GL_REPEAT);     // default value ?
    glTextureParameteri(texture.mId, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // default value ?
    glTextureParameteri(texture.mId,
                        GL_TEXTURE_MIN_FILTER,
                        GL_NEAREST_MIPMAP_LINEAR); // default value ?

    // Anisotropic filtering
    GLint maxAniso = 0;
    glGetIntegerv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAniso);
    glTextureParameteri(texture.mId, GL_TEXTURE_MAX_ANISOTROPY, maxAniso);

    // compute the number of level in the texture (nbMipmap + 1).
    const unsigned level = [&createInfo]() {
        if (createInfo.mipmap == 0) {
            return static_cast<unsigned>(std::log2(std::max(createInfo.width, createInfo.height))) +
                   1;
        }
        return createInfo.mipmap;
    }();

    const GLenum format = getGLFormat(createInfo.format).internalFormat;
    glTextureStorage2D(texture.mId,
                       (GLsizei)level,
                       format,
                       (GLsizei)createInfo.width,
                       (GLsizei)createInfo.height);

    return texture;
}

Texture Texture::CreateFromFile(const char* path, bool srgb, bool generateMipmap) {
    int   texWidth{};
    int   texHeight{};
    int   texChannels{}; // the number of channel in the image.
    void* pixels{};

    if (stbi_info(path, &texWidth, &texHeight, &texChannels) == 0) {
        fuse::logError("Fail to load texture: {}\n {}", path, stbi_failure_reason());
        return {};
    }

    Texture2DCreateInfo createInfo{};
    createInfo.debugName = path;
    createInfo.width     = (unsigned)texWidth;
    createInfo.height    = (unsigned)texHeight;
    createInfo.mipmap    = generateMipmap ? 0u : 1u;

    stbi_set_flip_vertically_on_load(1);

    Texture texture;
    if (stbi_is_hdr(path)) {
        if (texChannels == 1) {
            createInfo.format = PixelFormat::R32F;
        }
        if (texChannels == 2) {
            createInfo.format = PixelFormat::RG32F;
        }
        if (texChannels == 3) {
            createInfo.format = PixelFormat::RGB32F;
        }
        if (texChannels == 4) {
            createInfo.format = PixelFormat::RGBA32F;
        }
        pixels = stbi_loadf(path, &texWidth, &texHeight, &texChannels, 0);
    } else if (stbi_is_16_bit(path)) {
        if (texChannels == 1) {
            createInfo.format = PixelFormat::R16_UNORM;
        }
        if (texChannels == 2) {
            createInfo.format = PixelFormat::RG16_UNORM;
        }
        if (texChannels == 3) {
            createInfo.format = PixelFormat::RGB16_UNORM;
        }
        if (texChannels == 4) {
            createInfo.format = PixelFormat::RGBA16_UNORM;
        }
        pixels = stbi_load_16(path, &texWidth, &texHeight, &texChannels, 0);
    } else {
        if (texChannels == 1) {
            createInfo.format = PixelFormat::R8_UNORM;
        }
        if (texChannels == 2) {
            createInfo.format = PixelFormat::RG8_UNORM;
        }
        if (texChannels == 3) {
            createInfo.format = srgb ? PixelFormat::RGB8_UNORM_SRGB : PixelFormat::RGB8_UNORM;
        }
        if (texChannels == 4) {
            createInfo.format = srgb ? PixelFormat::RGBA8_UNORM_SRGB : PixelFormat::RGBA8_UNORM;
        }
        pixels = stbi_load(path, &texWidth, &texHeight, &texChannels, 0);
    }

    if (!pixels) {
        fuse::logError("Fail to load texture: {}", path);
        return {};
    }

    texture = Texture::Create(createInfo);
    texture.upload(0, (unsigned)texWidth, (unsigned)texHeight, pixels);
    if (generateMipmap) {
        texture.generateMipmap();
    }

    stbi_image_free(pixels);

    fuse::logError("Loading texture {} {}x{}x{}", path, texWidth, texHeight, texChannels);

    return texture;
}

Texture::~Texture() { glDeleteTextures(1, &mId); }


void Texture::upload(unsigned mipmap, unsigned width, unsigned height, void* data) {
    glTextureSubImage2D(mId,
                        (GLsizei)mipmap,
                        0,
                        0,
                        (GLsizei)width,
                        (GLsizei)height,
                        getGLFormat(mCreateInfo.format).format,
                        getGLFormat(mCreateInfo.format).type,
                        data);
}

void Texture::generateMipmap() { glGenerateTextureMipmap(mId); }

void Texture::bind() {}
void Texture::unbind() {}


// ======================================================
//              Build texture generation
// ======================================================

Texture Texture::Create(const TextureGenerator::ImageData& data) {
    Texture2DCreateInfo createInfo{
      .debugName = "",
      .width     = data.width,
      .height    = data.height,
      .mipmap    = 0,
      .format    = PixelFormat::RGBA8_UNORM_SRGB,
    };
    auto texture = Create(createInfo);
    texture.upload(0, data.width, data.height, (void*)data.pixels.data());
    texture.generateMipmap();
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
    Texture2DCreateInfo createInfo{
      .debugName = "DebugWithMipmap",
      .width     = 1024,
      .height    = 1024,
      .mipmap    = 6,
      .format    = PixelFormat::RGBA8_UNORM,
    };
    auto texture = Texture::Create(createInfo);

    // red
    auto mipmap0 = TextureGenerator::generateFlatImage(1024, 1024, {225, 0, 0});
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
