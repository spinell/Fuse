#pragma once
#include <glad/gl.h>

#include <algorithm>
#include <utility>


namespace TextureGenerator
{
    struct ImageData;
} // namespace TextureGenerator


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
    Texture(unsigned width, unsigned height, void* data);
    ~Texture();

    Texture(const Texture&)            = delete;
    Texture& operator=(const Texture&) = delete;

    Texture(Texture&& o) { o.mId = std::exchange(mId, o.mId); }

    Texture& operator=(Texture&& o) {
        o.mId = std::exchange(mId, o.mId);
        return *this;
    }

    void upload(unsigned mipmap, unsigned width, unsigned height, void* data);

    void bind();
    void unbind();

    GLuint getId() const { return mId; }

    static Texture Create(unsigned width, unsigned height, unsigned mipmap);

    static Texture Create(const TextureGenerator::ImageData&);

    static Texture CreateCheckBoard();
    static Texture CreateBrick1();
    static Texture CreateBrick2();
    static Texture CreateBrick3();
    static Texture CreateBrick4(unsigned width, unsigned height, unsigned brickWidth, unsigned brickHeight, unsigned mortarThickness);
    static Texture CreateGrass(unsigned width, unsigned height);

    /// @copydoc TextureGenerator::generateCheckerboard()
    static Texture CreateCheckerboard(unsigned width, unsigned height, const Color color1, const Color color2, unsigned int squareSize);

    /// @brief Create a texture with flat color and mipmap
    ///
    /// Each mipmap have a different color.
    /// This can be usefull for debugging.
    static Texture CreateDebugWithMipmap();

private:
    GLuint mId{0};
};
