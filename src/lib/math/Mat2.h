#pragma once
#include "Vec2.h"

namespace fuse {

/// @brief 2D matrix (column-major memory layout).
/// @ingroup Math
class Mat2 {
public:
    static const Mat2 kZero;
    static const Mat2 kIdentity;

    /// @brief Default constructor, <b>does not</b> initialise the matrix members.
    Mat2() = default;

    /// @brief Constructor which allow specifying which member.
    ///
    /// Each paramater represent an element at position (i,j) where <br>
    ///  - @b i is the row
    ///  - @b j is the column
    constexpr Mat2(float m00, float m01, float m10, float m11) noexcept {
        // clang-format off
        mData[0][0] = m00; mData[0][1] = m10;
        mData[1][0] = m01; mData[1][1] = m11;
        // clang-format on
    }

    /// @brief Construct a 2x2 matrix from 2 column vectors.
    /// @param[in] col0 The first column of the matrix.
    /// @param[in] col1 The second column of the matrix.
    constexpr Mat2(const Vec2& col0, const Vec2& col1) noexcept {
        // clang-format off
        mData[0][0] = col0.x; mData[0][1] = col0.y;
        mData[1][0] = col1.x; mData[1][1] = col1.y;
        // clang-format on
    }

    /// @brief Strict comparaison component by component.
    [[nodiscard]] constexpr bool operator==(const Mat2&) const noexcept = default;

    /// @{
    /// @brief Direct access to elements @p row / @p col.
    /// @param[in] row The row index
    /// @param[in] col The col index
    [[nodiscard]] constexpr float& operator()(unsigned row, unsigned col) noexcept {
        return mData[col][row];
    }

    /// @copydoc operator()(int, int)
    [[nodiscard]] constexpr float operator()(unsigned row, unsigned col) const noexcept {
        return mData[col][row];
    }

    /// @brief Direct access to the underlying data.
    [[nodiscard]] float* data() noexcept { return reinterpret_cast<float*>(mData); }

    /// @copydoc data()
    [[nodiscard]] const float* data() const noexcept {
        return reinterpret_cast<const float*>(mData);
    }

    /// @}

    /// @brief Compute the determinant of this matrix.
    [[nodiscard]] float determinant() const noexcept {
        return mData[0][0] * mData[1][1] - mData[1][0] * mData[0][1];
    }

    /// @brief Compute the inverse of this matrix.
    /// @todo Handle zero determinant case.
    [[nodiscard]] Mat2 inverse() const noexcept {
        const float det    = determinant();
        const float invDet = 1.0f / det;
        const Mat2& r      = *this;
        return Mat2(
          // clang-format off
         r(1, 1) * invDet, -r(0, 1) * invDet,
        -r(1, 0) * invDet,  r(0, 0) * invDet
          // clang-format on
        );
    }

    /// @brief Compute the transpose of this matrix.
    [[nodiscard]] Mat2 transpose() const noexcept {
        const Mat2& r = *this;
        return Mat2(
          // clang-format off
            r(0,0), r(1,0),
            r(0,1), r(1,1)
          // clang-format on
        );
    }

    /// @{
    /// @brief Addition this matrix with another matrix.
    Mat2& operator+=(const Mat2&) noexcept;
    /// @brief Substract this matrix with another matrix.
    Mat2& operator-=(const Mat2&) noexcept;
    /// @brief Multiply this matrix with another matrix.
    Mat2& operator*=(const Mat2&) noexcept;
    /// @}

    /// @{
    /// @brief Addition 2 matrix.
    [[nodiscard]] Mat2 operator+(const Mat2&) const noexcept;
    /// @brief Substract 2 matrix.
    [[nodiscard]] Mat2 operator-(const Mat2&) const noexcept;
    /// @brief Multiply 2 matrix.
    [[nodiscard]] Mat2 operator*(const Mat2&) const noexcept;
    /// @}

private:
    float mData[2][2];
};

inline constexpr Mat2 Mat2::kZero(0, 0, 0, 0);
inline constexpr Mat2 Mat2::kIdentity(1, 0, 0, 1);

/// @ingroup  Math
/// @related  Mat2
/// @{
/// @brief Multiply a matrix by a value.
[[nodiscard]] Mat2 operator*(const Mat2&, float) noexcept;

/// @copydoc operator*(const Mat2&, float)
[[nodiscard]] Mat2 operator*(float value, const Mat2& mat) noexcept;

/// @brief Multiply a matrix by a value.
Mat2& operator*=(Mat2&, float) noexcept;

/// @brief Multiply the column vector @b v by the matrix @b m
[[nodiscard]] Vec2 operator*(const Mat2& m, const Vec2& v) noexcept;
/// @}

} // namespace fuse
