#pragma once
#include "Vec3.h"

namespace fuse {

/// @brief 3D matrix (column-major memory layout).
/// @ingroup Math
class Mat3 {
public:
    /// @brief Default constructor, <b>does not</b> initialise the matrix members.
    Mat3() = default;

    /// @brief Constructor which allow specifying which member.
    ///
    /// Each paramater represent an element at position (i,j) where <br>
    ///  - @b i is the row
    ///  - @b j is the column
    ///
    // clang-format off
    Mat3(float m00, float m01, float m02,
         float m10, float m11, float m12,
         float m20, float m21, float m22) noexcept {
            mData[0][0] = m00; mData[0][1] = m10; mData[0][2] = m20;
            mData[1][0] = m01; mData[1][1] = m11; mData[1][2] = m21;
            mData[2][0] = m02; mData[2][1] = m12; mData[2][2] = m22;
    }

    // clang-format on

    /// @brief Construct a 3x3 matrix from 3 column vectors.
    /// @param col0 The first column of the matrix.
    /// @param col1 The second column of the matrix.
    /// @param col2 The third column of the matrix.
    constexpr Mat3(const Vec3& col0, const Vec3& col1, const Vec3& col2) noexcept {
        // clang-format off
        mData[0][0] = col0.x; mData[0][1] = col0.y; mData[0][2] = col0.z;
        mData[1][0] = col1.x; mData[1][1] = col1.y; mData[1][2] = col1.z;
        mData[2][0] = col2.x; mData[2][1] = col2.y; mData[2][2] = col2.z;
        // clang-format on
    }

    /// @{
    /// @brief Direct access to elements @p row / @p col.
    /// @param[in] row The row index
    /// @param[in] col The col index
    float& operator()(int row, int col) noexcept { return mData[col][row]; }

    /// @copydoc operator()(int, int)
    float operator()(int row, int col) const noexcept { return mData[col][row]; }

    /// @brief Direct access to the underlying data.
    [[nodiscard]] float* data() noexcept { return reinterpret_cast<float*>(mData); }

    /// @copydoc data()
    [[nodiscard]] const float* data() const noexcept {
        return reinterpret_cast<const float*>(mData);
    }

    /// @}

    /// @{
    /// @brief Addition this matrix with another matrix.
    Mat3& operator+=(const Mat3&) noexcept;
    /// @brief Substract this matrix with another matrix.
    Mat3& operator-=(const Mat3&) noexcept;
    /// @brief Multiply this matrix with another matrix.
    Mat3& operator*=(const Mat3&) noexcept;
    /// @}

    /// @{
    /// @brief Addition 2 matrix..
    [[nodiscard]] Mat3 operator+(const Mat3&) const noexcept;
    /// @brief Substract 2 matrix.
    [[nodiscard]] Mat3 operator-(const Mat3&) const noexcept;
    /// @brief Multiply 2 matrix.
    [[nodiscard]] Mat3 operator*(const Mat3&) const noexcept;
    /// @}

private:
    float mData[3][3];
};

/// @ingroup  Math
/// @related  Mat3
/// @{
/// @brief Multiply a matrix by a value.
[[nodiscard]] Mat3 operator*(const Mat3&, float) noexcept;

/// @copydoc operator*(const Mat3&, float)
[[nodiscard]] Mat3 operator*(float value, const Mat3& mat) noexcept;

/// @brief Multiply a matrix by a value.
Mat3& operator*=(Mat3&, float) noexcept;

/// @brief Multiply the column vector @b v by the matrix @b m
[[nodiscard]] Vec3 operator*(const Mat3& m, const Vec3& v) noexcept;
/// @}

} // namespace fuse
