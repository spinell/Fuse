#pragma once
#include "Vec2.h"

namespace fuse {

/// @brief 2D matrix (column-major memory layout).
class Mat2 {
public:
    /// @brief Default constructor, does not initialise the matrix members.
    Mat2() = default;

    /// @brief Constructor which allow specifying which member.
    ///
    /// Each paramater represent an element at position (i,j) where <br>
    ///  - @b i is the row
    ///  - @b j is the column
    ///
    // clang-format off
    Mat2(float m00, float m01,
         float m10, float m11) noexcept {
            mData[0][0] = m00; mData[0][1] = m10;
            mData[1][0] = m01; mData[1][1] = m11;
         }

    // clang-format on

    /// @brief Construct a 2x2 matrix from 2 column vectors.
    /// @param col0 The first column of the matrix.
    /// @param col1 The second column of the matrix.
    constexpr Mat2(const Vec2& col0,
                   const Vec2& col1) noexcept {
        // clang-format off
        mData[0][0] = col0.x; mData[0][1] = col0.y;
        mData[1][0] = col1.x; mData[1][1] = col1.y;
        // clang-format on
    }

    /// @brief Direct access to elements.
    float& operator()(int row, int col) noexcept { return mData[col][row]; }

    /// @copydoc operator()(int,int)
    float operator()(int row, int col) const noexcept { return mData[col][row]; }

    /// @brief Matrix multiplication.
    Mat2& operator*=(const Mat2&) noexcept;

    /// @brief Matrix multiplication.
    [[nodiscard]] Mat2 operator*(const Mat2&) const noexcept;

    /// @brief Direct access to the underlying data.
    [[nodiscard]] float* data() noexcept { return reinterpret_cast<float*>(mData); }

    /// @copydoc data()
    [[nodiscard]] const float* data() const noexcept {
        return reinterpret_cast<const float*>(mData);
    }

private:
    float mData[2][2];
};

/// @relates Mat2
/// @brief Multiply the column vector @b v by the matrix @b m
[[nodiscard]] Vec2 operator*(const Mat2& m, const Vec2& v) noexcept;

} // namespace fuse
