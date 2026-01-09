#pragma once
#include <vector>

/// @brief Namespace which contains function to generate images.
/// @todo Review brick generation texture for similitude/duplicate
namespace TextureGenerator {
struct Color {
    unsigned char r{};
    unsigned char g{};
    unsigned char b{};
    unsigned char a{255};
};

struct ImageData {
    ImageData(unsigned width, unsigned height)
        : width(width)
        , height(height) {
        pixels.resize(width * height);
    }

    Color& operator()(unsigned row, unsigned col) {
        return pixels[row * width + col];
    }

    std::vector<Color> pixels;
    unsigned           width;
    unsigned           height;
};

/// @brief Generate an image with a single color.
/// @param width The width of the image in pixel.
/// @param height The height of the image in pixel.
/// @param color The color of each pixel of the image image.
ImageData generateFlatImage(unsigned width, unsigned height, Color color);

/// @brief Generate an image with a brick pattern.
/// @param width The width of the image in pixel.
/// @param height The height of the image in pixel.
ImageData generateBrickTexture1(unsigned width, unsigned height);

/// @brief Generate an image with a brick pattern.
/// @param width The width of the image in pixel.
/// @param height The height of the image in pixel.
ImageData generateBrickTexture2(unsigned width, unsigned height);

/// @brief Generate an image with a brick pattern.
/// @param width The width of the image in pixel.
/// @param height The height of the image in pixel.
ImageData generateBrickTexture3(unsigned width, unsigned height);

/// @brief Generate an image with a brick pattern.
/// @param width The width of the image in pixel.
/// @param height The height of the image in pixel.
ImageData generateBrickTexture4(unsigned width, unsigned height, unsigned brickWidth = 40,
                              unsigned brickHeight = 20, unsigned mortarThickness = 5);

/// @brief Generate an image with a brick pattern.
/// @param width The width of the image in pixel.
/// @param height The height of the image in pixel.
ImageData generateBrickTexture5(unsigned width, unsigned height);


/// @brief Generate an image with a brick pattern.
/// @param width The width of the image in pixel.
/// @param height The height of the image in pixel.
ImageData generateBrickTexture6(unsigned width, unsigned height);

/// @brief
/// @param width The width of the image in pixel.
/// @param height The height of the image in pixel.
ImageData generateGrass(unsigned width, unsigned height);

/// @brief
/// @param width The width of the image in pixel.
/// @param height The height of the image in pixel.
ImageData generateGrass2(unsigned width, unsigned height);

/// @brief Generates an image with a checkerboard pattern.
/// @param width The width of the image in pixel.
/// @param height The height of the image in pixel.
/// @param color1 The first color.
/// @param color2 The second color.
/// @param squareSize The size of each checkerboard square in pixels.
ImageData generateCheckerboard(unsigned int width, unsigned int height, Color color1, Color color2,
                               unsigned int squareSize);

/// @brief Generates an black and white image with XOR pattern.
/// @param width The width of the image in pixel.
/// @param height The height of the image in pixel.
ImageData generateXor(unsigned int width, unsigned int height);

} // namespace TextureGenerator
