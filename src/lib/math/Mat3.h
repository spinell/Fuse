#pragma once
#include "Vec3.h"

namespace fuse {
class Angle;

/// @brief 3D matrix (column-major memory layout).
/// @ingroup Math
class Mat3 {
public:
    static const Mat3 kZero;
    static const Mat3 kIdentity;

    /// @brief Default constructor, <b>does not</b> initialise the matrix members.
    Mat3() = default;

    /// @brief Constructor which allow specifying which member.
    ///
    /// Each paramater represent an element at position (i,j) where <br>
    ///  - @b i is the row
    ///  - @b j is the column
    ///
    // clang-format off
    constexpr Mat3(float m00, float m01, float m02,
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

    /// @brief Strict comparaison component by component.
    [[nodiscard]] constexpr bool operator==(const Mat3&) const noexcept = default;

    /// @{
    /// @brief Direct access to elements @p row / @p col.
    /// @param[in] row The row index
    /// @param[in] col The col index
    [[nodiscard]] constexpr float& operator()(unsigned row, unsigned col) noexcept { return mData[col][row]; }

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
    /// @note
    ///  - The determinant of the identity matrix is 1.
    ///  - The determinant of a matrix product is the product of the determinants.
    ///  - The determinant of a matrix is equals to the determinant of its transpose.
    ///  - If the determinant is zero, the matrix is singular (not invertible).
    ///  - If the determinant is positive, the orientation is preserved.
    ///  - If the determinant is negative, the orientation is reversed.
    /// @return The determinant value.
    [[nodiscard]] float determinant() const noexcept {
        // Uses the rule of Sarrus.
        const float a = mData[0][0] * (mData[1][1] * mData[2][2] - mData[1][2] * mData[2][1]);
        const float b = mData[0][1] * (mData[1][0] * mData[2][2] - mData[1][2] * mData[2][0]);
        const float c = mData[0][2] * (mData[1][0] * mData[2][1] - mData[1][1] * mData[2][0]);
        return a - b + c;
    }

    /// @brief Compute the inverse of this matrix.
    /// @todo Handle zero determinant case.
    [[nodiscard]] Mat3 inverse() const noexcept {
        const float det    = determinant();
        const float invDet = 1.0f / det;
        const Mat3& r      = *this;
        // clang-format off
        const float m00 = (r(1,1) * r(2,2) - r(1,2) * r(2,1)) * invDet;
        const float m01 = (r(0,2) * r(2,1) - r(0,1) * r(2,2)) * invDet;
        const float m02 = (r(0,1) * r(1,2) - r(0,2) * r(1,1)) * invDet;
        const float m10 = (r(1,2) * r(2,0) - r(1,0) * r(2,2)) * invDet;
        const float m11 = (r(0,0) * r(2,2) - r(0,2) * r(2,0)) * invDet;
        const float m12 = (r(0,2) * r(1,0) - r(0,0) * r(1,2)) * invDet;
        const float m20 = (r(1,0) * r(2,1) - r(1,1) * r(2,0)) * invDet;
        const float m21 = (r(0,1) * r(2,0) - r(0,0) * r(2,1)) * invDet;
        const float m22 = (r(0,0) * r(1,1) - r(0,1) * r(1,0)) * invDet;
        // clang-format on

        return Mat3(
          // clang-format off
          m00, m01, m02,
          m10, m11, m12,
          m20, m21, m22
          // clang-format on
        );
    }

    [[nodiscard]] float trace() const noexcept { return mData[0][0] + mData[1][1] + mData[2][2]; }

    /// @brief Compute the transpose of this matrix.
    [[nodiscard]] Mat3 transpose() const noexcept {
        const Mat3& r = *this;
        return Mat3(
          // clang-format off
            r(0, 0), r(1, 0), r(2, 0),
            r(0, 1), r(1, 1), r(2, 1),
            r(0, 2), r(1, 2), r(2, 2)
          // clang-format on
        );
    }

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

    /// @name Transform
    ///@{

    /// @brief Create a scaling matrix.
    [[nodiscard]] static Mat3 CreateScaling(const Vec3& scale) noexcept;

    /// @brief Create a scaling matrix.
    /// @param[in] scale The scaling factor.
    /// @param[in] direction The direction of the scaling axis (Must be normalized)
    [[nodiscard]] static Mat3 CreateScaling(float scale, const Vec3& direction) noexcept;

    /// @brief Create a rotation matrix around x-axis.
    /// 1    0           0
    /// 0 cos(angle) -sin(angle)
    /// 0 sin(angle)  cos(angle)
    [[nodiscard]] static Mat3 CreateRotationX(const Angle& angle) noexcept;

    /// @brief Create a rotation matrix around y-axis.
    /// cos(angle)   0   sin(angle)
    ///      0       1      0
    /// -sin(angle)  0   cos(angle)
    [[nodiscard]] static Mat3 CreateRotationY(const Angle& angle) noexcept;

    /// @brief Create a rotation matrix around z-axis.
    /// cos(angle) -sin(angle)  0
    /// sin(angle)  cos(angle)  0
    ///     0           0       1
    [[nodiscard]] static Mat3 CreateRotationZ(const Angle& angle) noexcept;

    /// @brief Create a rotation matrix from a angle and an axis.
    [[nodiscard]] static Mat3 CreateRotation(Angle angle, const Vec3& axis) noexcept;

    /// @brief Create a reflection matrix.
    /// The reflection matrix reflect points across a plane.
    /// The plane is defined by its normal vector.
    /// @param normal The normal of the reflection plane (Must be normalized).
    [[nodiscard]] static Mat3 CreateReflection(const Vec3& normal) noexcept;

    ///@}

private:
    float mData[3][3];
};

inline constexpr Mat3 Mat3::kZero(0, 0, 0, 0, 0, 0, 0, 0, 0);
inline constexpr Mat3 Mat3::kIdentity(1, 0, 0, 0, 1, 0, 0, 0, 1);

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

/// @brief Multiply the row vector @b v by the matrix @b m
[[nodiscard]] Vec3 operator*(const Vec3& v, const Mat3& m) noexcept;

/// @}

} // namespace fuse
