#pragma once
#include "Vec4.h"


namespace fuse {

/// @brief 4x4 homogeneous matrix (column-major memory layout).
/// @ingroup Math
class Mat4 {
public:
    /// @brief Default constructor, <b>does not</b> initialise the matrix members.
    Mat4() = default;

    /// @brief Constructor which allow specifying which member.
    ///
    /// Each paramater represent an element at position (i,j) where <br>
    ///  - @b i is the row
    ///  - @b j is the column
    ///
    // clang-format off
    Mat4(float m00, float m01, float m02, float m03,
         float m10, float m11, float m12, float m13,
         float m20, float m21, float m22, float m23,
         float m30, float m31, float m32, float m33) noexcept {
            mData[0][0] = m00; mData[0][1] = m10; mData[0][2] = m20; mData[0][3] = m30;
            mData[1][0] = m01; mData[1][1] = m11; mData[1][2] = m21; mData[1][3] = m31;
            mData[2][0] = m02; mData[2][1] = m12; mData[2][2] = m22; mData[2][3] = m32;
            mData[3][0] = m03; mData[3][1] = m13; mData[3][2] = m23; mData[3][3] = m33;
    }

    // clang-format on

    /// @brief Construct a 4x4 matrix from 4 column vectors.
    /// @param col0 The first column of the matrix.
    /// @param col1 The second column of the matrix.
    /// @param col2 The third column of the matrix.
    /// @param col3 The fourth column of the matrix.
    constexpr Mat4(const Vec4& col0,
                   const Vec4& col1,
                   const Vec4& col2,
                   const Vec4& col3) noexcept {
        // clang-format off
        mData[0][0] = col0.x; mData[0][1] = col0.y; mData[0][2] = col0.z; mData[0][3] = col0.w;
        mData[1][0] = col1.x; mData[1][1] = col1.y; mData[1][2] = col1.z; mData[1][3] = col1.w;
        mData[2][0] = col2.x; mData[2][1] = col2.y; mData[2][2] = col2.z; mData[2][3] = col2.w;
        mData[3][0] = col3.x; mData[3][1] = col3.y; mData[3][2] = col3.z; mData[3][3] = col3.w;
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
    Mat4& operator+=(const Mat4&) noexcept;
    /// @brief Substract this matrix with another matrix.
    Mat4& operator-=(const Mat4&) noexcept;
    /// @brief Multiply this matrix with another matrix.
    Mat4& operator*=(const Mat4&) noexcept;
    /// @}

    /// @{
    /// @brief Addition 2 matrix..
    [[nodiscard]] Mat4 operator+(const Mat4&) const noexcept;
    /// @brief Substract 2 matrix.
    [[nodiscard]] Mat4 operator-(const Mat4&) const noexcept;
    /// @brief Multiply 2 matrix.
    [[nodiscard]] Mat4 operator*(const Mat4&) const noexcept;
    /// @}

private:
    float mData[4][4];
};

/// @ingroup  Math
/// @related  Mat4
/// @{
/// @brief Multiply a matrix by a value.
[[nodiscard]] Mat4 operator*(const Mat4&, float) noexcept;

/// @copydoc operator*(const Mat4&, float)
[[nodiscard]] Mat4 operator*(float value, const Mat4& mat) noexcept;

/// @brief Multiply a matrix by a value.
Mat4& operator*=(Mat4&, float) noexcept;

/// @brief Multiply the column vector @b v by the matrix @b m
[[nodiscard]] Vec4 operator*(const Mat4& m, const Vec4& v) noexcept;
/// @}

} // namespace fuse
