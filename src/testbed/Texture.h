#pragma once
#include <glad/gl.h>

#include <algorithm>
#include <utility>


namespace TextureGenerator {
struct ImageData;
} // namespace TextureGenerator

enum class PixelFormat {
    // 8-bit unsigned-normalized
    R8_UNORM,
    RG8_UNORM,
    RGB8_UNORM,
    RGBA8_UNORM,
    // 16-bit unsigned-normalized
    R16_UNORM,
    RG16_UNORM,
    RGB16_UNORM,
    RGBA16_UNORM,
    // unsigned-normalized integer sRGB
    RGB8_UNORM_SRGB,
    RGBA8_UNORM_SRGB,
    // float
    R32F,
    RG32F,
    RGB32F,
    RGBA32F,
    Count
};

/// @brief Describe a Texture 2D creation.
struct Texture2DCreateInfo {
    const char* debugName;
    unsigned    width; ///! Texture width
    unsigned    height;///! Texture height

    /// The number of mipmap
    /// If 0, the number of mipmap will be auto calculated.
    /// If 1, no mipmap, only the base image.
    /// If 2, base image + 1 mipmap....
    unsigned    mipmap;
    /// The texture pixel format
    PixelFormat format;
    //unsigned  arrayCount = 0;
    //unsigned  msaa = 0
};

/// @brief Thing wrapper around OpenGL Texture.
class Texture {
public:
    struct Color {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a = 255;
    };

    Texture() = default;
    ~Texture();
    Texture(const Texture&)            = delete;
    Texture& operator=(const Texture&) = delete;

    Texture(Texture&& o) {
        o.mId = std::exchange(mId, o.mId);
        o.mCreateInfo = std::exchange(mCreateInfo, o.mCreateInfo);
    }

    Texture& operator=(Texture&& o) {
        o.mId = std::exchange(mId, o.mId);
        o.mCreateInfo = std::exchange(mCreateInfo, o.mCreateInfo);
        return *this;
    }

    void upload(unsigned mipmap, unsigned width, unsigned height, void* data);
    void generateMipmap();
    void bind();
    void unbind();

    GLuint getId() const { return mId; }

    static Texture Create(const Texture2DCreateInfo& createInfo);
    static Texture CreateFromFile(const char* path, bool srgb, bool generateMipmap);
    static Texture Create(const TextureGenerator::ImageData&);
    static Texture CreateCheckBoard();
    static Texture CreateBrick1();
    static Texture CreateBrick2();
    static Texture CreateBrick3();
    static Texture CreateBrick4(unsigned width, unsigned height, unsigned brickWidth,
                                unsigned brickHeight, unsigned mortarThickness);
    static Texture CreateGrass(unsigned width, unsigned height);

    /// @copydoc TextureGenerator::generateCheckerboard()
    static Texture CreateCheckerboard(unsigned width, unsigned height, const Color color1,
                                      const Color color2, unsigned int squareSize);

    /// @brief Create a texture with flat color and mipmap
    ///
    /// Each mipmap have a different color.
    /// This can be usefull for debugging.
    static Texture CreateDebugWithMipmap();

private:
    Texture2DCreateInfo mCreateInfo{};
    GLuint mId{0};
};
