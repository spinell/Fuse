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
    constexpr Mat4(float m00, float m01, float m02, float m03,
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

    /// @brief Strict comparaison component by component.
    [[nodiscard]] constexpr bool operator==(const Mat4&) const noexcept = default;

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

    /// @brief Compute the determinant of this matrix.
    [[nodiscard]] float determinant() const noexcept {
        const float subFactor00 = mData[2][2] * mData[3][3] - mData[3][2] * mData[2][3];
        const float subFactor01 = mData[2][1] * mData[3][3] - mData[3][1] * mData[2][3];
        const float subFactor02 = mData[2][1] * mData[3][2] - mData[3][1] * mData[2][2];
        const float subFactor03 = mData[2][0] * mData[3][3] - mData[3][0] * mData[2][3];
        const float subFactor04 = mData[2][0] * mData[3][2] - mData[3][0] * mData[2][2];
        const float subFactor05 = mData[2][0] * mData[3][1] - mData[3][0] * mData[2][1];

        const Vec4 detCof(
          +(mData[1][1] * subFactor00 - mData[1][2] * subFactor01 + mData[1][3] * subFactor02),
          -(mData[1][0] * subFactor00 - mData[1][2] * subFactor03 + mData[1][3] * subFactor04),
          +(mData[1][0] * subFactor01 - mData[1][1] * subFactor03 + mData[1][3] * subFactor05),
          -(mData[1][0] * subFactor02 - mData[1][1] * subFactor04 + mData[1][2] * subFactor05));

        return mData[0][0] * detCof.x + mData[0][1] * detCof.y + mData[0][2] * detCof.z +
               mData[0][3] * detCof.w;
    }

    /// @brief Compute the inverse of this matrix.
    /// @todo Handle zero determinant case.
    [[nodiscard]] Mat4 inverse() const noexcept {
        const Mat4& r   = *this;
        const float m00 = r(0, 0), m01 = r(0, 1), m02 = r(0, 2), m03 = r(0, 3);
        const float m10 = r(1, 0), m11 = r(1, 1), m12 = r(1, 2), m13 = r(1, 3);
        const float m20 = r(2, 0), m21 = r(2, 1), m22 = r(2, 2), m23 = r(2, 3);
        const float m30 = r(3, 0), m31 = r(3, 1), m32 = r(3, 2), m33 = r(3, 3);

        float v0 = m20 * m31 - m21 * m30;
        float v1 = m20 * m32 - m22 * m30;
        float v2 = m20 * m33 - m23 * m30;
        float v3 = m21 * m32 - m22 * m31;
        float v4 = m21 * m33 - m23 * m31;
        float v5 = m22 * m33 - m23 * m32;

        const float t00 = +(v5 * m11 - v4 * m12 + v3 * m13);
        const float t10 = -(v5 * m10 - v2 * m12 + v1 * m13);
        const float t20 = +(v4 * m10 - v2 * m11 + v0 * m13);
        const float t30 = -(v3 * m10 - v1 * m11 + v0 * m12);

        const float invDet = 1 / (t00 * m00 + t10 * m01 + t20 * m02 + t30 * m03);

        float d00 = t00 * invDet;
        float d10 = t10 * invDet;
        float d20 = t20 * invDet;
        float d30 = t30 * invDet;

        float d01 = -(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
        float d11 = +(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
        float d21 = -(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
        float d31 = +(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

        v0 = m10 * m31 - m11 * m30;
        v1 = m10 * m32 - m12 * m30;
        v2 = m10 * m33 - m13 * m30;
        v3 = m11 * m32 - m12 * m31;
        v4 = m11 * m33 - m13 * m31;
        v5 = m12 * m33 - m13 * m32;

        float d02 = +(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
        float d12 = -(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
        float d22 = +(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
        float d32 = -(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

        v0 = m21 * m10 - m20 * m11;
        v1 = m22 * m10 - m20 * m12;
        v2 = m23 * m10 - m20 * m13;
        v3 = m22 * m11 - m21 * m12;
        v4 = m23 * m11 - m21 * m13;
        v5 = m23 * m12 - m22 * m13;

        float d03 = -(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
        float d13 = +(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
        float d23 = -(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
        float d33 = +(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

        return Mat4(
          // clang-format off
            d00, d01, d02, d03,
            d10, d11, d12, d13,
            d20, d21, d22, d23,
            d30, d31, d32, d33
          // clang-format on
        );
    }

    /// @brief Compute the transpose of this matrix.
    [[nodiscard]] Mat4 transpose() const noexcept {
        const Mat4& r = *this;
        return Mat4(
          // clang-format off
            r(0,0), r(1,0), r(2,0), r(3,0),
            r(0,1), r(1,1), r(2,1), r(3,1),
            r(0,2), r(1,2), r(2,2), r(3,2),
            r(0,3), r(1,3), r(2,3), r(3,3)
          // clang-format on
        );
    }

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
