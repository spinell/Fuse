#pragma once
#include "Vec3.h"
#include "Vec4.h"

namespace fuse {
class Angle;

/// @brief 4x4 homogeneous matrix (column-major memory layout).
/// @ingroup Math
class Mat4 {
public:
    static const Mat4 kZero;     ///< The zero matrix.
    static const Mat4 kIdentity; ///< The identity matrix.

    /// @brief Default constructor, <b>does not</b> initialise the matrix members.
    Mat4() = default;

    /// @brief Constructor which allow specifying which member.
    ///
    /// Each paramater represent an element at position (i,j) where <br>
    ///  - @b i is the row
    ///  - @b j is the column
    ///
    constexpr Mat4(float m00, float m01, float m02, float m03, float m10, float m11, float m12,
                   float m13, float m20, float m21, float m22, float m23, float m30, float m31,
                   float m32, float m33) noexcept {
        // clang-format off
            mData[0][0] = m00; mData[0][1] = m10; mData[0][2] = m20; mData[0][3] = m30;
            mData[1][0] = m01; mData[1][1] = m11; mData[1][2] = m21; mData[1][3] = m31;
            mData[2][0] = m02; mData[2][1] = m12; mData[2][2] = m22; mData[2][3] = m32;
            mData[3][0] = m03; mData[3][1] = m13; mData[3][2] = m23; mData[3][3] = m33;
        // clang-format on
    }


    /// @brief Construct a 4x4 matrix from 4 column vectors.
    /// @param col0 The first column of the matrix.
    /// @param col1 The second column of the matrix.
    /// @param col2 The third column of the matrix.
    /// @param col3 The fourth column of the matrix.
    constexpr Mat4(const Vec4& col0, const Vec4& col1, const Vec4& col2,
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

    /// @name Transform
    ///@{

    /// @brief Create a translation matrix.
    [[nodiscard]] static Mat4 CreateTranslation(const Vec3& translation) noexcept;

    /// @brief Create a scaling matrix.
    [[nodiscard]] static Mat4 CreateScaling(const Vec3& scale) noexcept;

    /// @brief Create a scaling matrix.
    /// @param[in] scale The scaling factor.
    /// @param[in] direction The direction of the scaling axis (Must be normalized)
    [[nodiscard]] static Mat4 CreateScaling(float scale, const Vec3& direction) noexcept;

    /// @brief Create a rotation matrix around x-axis.
    /// 1      0           0      0
    /// 0 cos(angle) -sin(angle)  0
    /// 0 sin(angle)  cos(angle)  0
    /// 0      0          1       1
    [[nodiscard]] static Mat4 CreateRotationX(const Angle& angle) noexcept;

    /// @brief Create a rotation matrix around y-axis.
    /// cos(a)   0   sin(a)  0
    ///      0   1     0     0
    /// -sin(a)  0   cos(a)  0
    ///    0     0     1     1
    [[nodiscard]] static Mat4 CreateRotationY(const Angle& angle) noexcept;

    /// @brief Create a rotation matrix around z-axis.
    /// cos(angle) -sin(angle)  0  0
    /// sin(angle)  cos(angle)  0  0
    ///     0            0      1  0
    ///     0            0      0  1
    [[nodiscard]] static Mat4 CreateRotationZ(const Angle& angle) noexcept;

    /// @brief Create a rotation matrix from a angle and an axis.
    [[nodiscard]] static Mat4 CreateRotation(Angle angle, const Vec3& axis) noexcept;

    /// @brief Create a 4x4 reflection matrix.
    /// The reflection matrix reflect points across a plane.
    /// The plane is defined by its normal vector.
    /// @param normal The normal of the reflection plane (Must be normalized).
    [[nodiscard]] static Mat4 CreateReflection(const Vec3& normal) noexcept;

    ///@}

    /// @name View
    ///@{

    /// @brief Create a right-handed view matrix.
    ///
    /// Create a view matrix from a camera position, a reference point to look at and an up direction.
    ///  - The right vector is pointing @b +x.
    ///  - The up vector is pointing @b +y.
    ///  - The forward vector is pointing @b -Z.
    ///
    /// @pre The @b position and @b target must not be the same.
    /// @pre The @b upVector vector must not be parallel to the line of sight from the
    ///       camera position to the reference point.
    /// @param [in] position Position of the camera.
    /// @param [in] target   Position of the reference point to look at.
    /// @param [in] upVector Up direction of the camera.
    [[nodiscard]] static Mat4 CreateViewLookAt(const Vec3& position, const Vec3& target,
                                               const Vec3& upVector = Vec3::kUnitY) noexcept;

    /// @brief Builds a view matrix for a right-handed coordinate.
    ///
    /// Create a view matrix from a camera position, a direction to look at and an up direction.
    ///  - The right vector is pointing @b +x.
    ///  - The up vector is pointing @b +y.
    ///  - The forward vector is pointing @b -Z.
    ///
    /// @pre The @b upVector vector must not be parallel to the line of sight from the
    ///       camera position to the reference point.
    /// @pre The @b direction vector must not be 0.
    /// @param [in] position  Position of the camera.
    /// @param [in] direction Direction of the camera.
    /// @param [in] upVector  Up direction of the camera.
    [[nodiscard]] static Mat4 CreateViewLookTo(const Vec3& position, const Vec3& direction,
                                               const Vec3& upVector = Vec3::kUnitY) noexcept;
    ///@}


    /// @name Projection
    ///@{

    /// @brief Create a orthographic projection matrix.
    /// The resulting matrix is a centered orthographic projection matrix.
    /// This function is the equivalent of
    /// <b>ProjOrthoOffCenter(-width/2, width/2, -height/2, height/2, near, far)</b>
    /// @see ProjOrthoOffCenter()
    [[nodiscard]] static Mat4 ProjOrtho(float width, float height, float near = -1.0F,
                                        float far = 1.0F) noexcept;

    /// @brief Create a orthographic projection matrix.
    ///
    /// All the parameters are distances in camera space. The parameters describe the dimensions of the view volume.
    ///
    /// @note This function use OpenGL convention.
    /// The view volume is a unit cube with +z pointing into the screen.
    ///
    /// @param left   The coordinates for the left vertical clipping plane.
    /// @param right  The coordinates for theright vertical clipping plane.
    /// @param bottom The coordinates for the bottom horizontal clipping plane.
    /// @param top    The coordinates for the top horizontal clipping plans.
    /// @param near   The distances to the nearer depth clipping plane.
    ///               This distance is negative if the plane is to be behind the viewer.
    /// @param far    The distances to the farther depth clipping plane.
    ///               This distance is negative if the plane is to be behind the viewer.
    /// @return The projection matrix.
    [[nodiscard]] static Mat4 ProjOrthoOffCenter(float left, float right, float bottom, float top,
                                                 float near /* =-1*/, float far /*= 1*/) noexcept;

    /// @brief Create a customized, right-handed perspective projection matrix.
    ///
    /// @param [in] left   The x-coordinate of the left side of the clipping frustum at the near clipping plane.
    /// @param [in] right  The x-coordinate of the right side of the clipping frustum at the near clipping plane.
    /// @param [in] bottom The y-coordinate of the bottom side of the clipping frustum at the near clipping plane.
    /// @param [in] top    The y-coordinate of the top side of the clipping frustum at the near clipping plane.
    /// @param [in] zNear  Distance to the near clipping plane. Must be greater than zero.
    /// @param [in] zFar   Distance to the far clipping plane. Must be greater than zero.
    /// @return Returns the custom perspective projection matrix.
    /// @warning @b zNear and @b zFar cannot be the same value.
    /// @remark
    ///     For typical usage, @b zNear is less than @b zFar. <br/>
    ///     However, if you flip these values so @b zFar is
    ///     less than @b zNear, the result is an inverted z buffer which can provide increased
    ///     floating-point precision.
    [[nodiscard]] static Mat4 CreateProjectionPerspectiveOffCenter(float left, float right,
                                                                   float bottom, float top,
                                                                   float zNear /* =-1*/,
                                                                   float zFar /*= 1*/) noexcept;

    /// @brief Create a right-handed perspective projection matrix based on a horizontal field of view.
    ///
    /// @param[in] fovx 	   Horizontal field of view.
    /// @param[in] aspectRatio Aspect ratio, defined as view space width divided by height.
    ///                        Determines the vertical field of view.
    /// @param[in] zNear       Distance to the near plane.
    /// @param[in] zFar		   Distance to the far plane.
    /// @remark
    ///     For typical usage, @b zNear is less than @b zFar. <br/>
    ///     However, if you flip these values so @b zFar is
    ///     less than @b zNear, the result is an inverted z buffer which can provide increased
    ///     floating-point precision.
    [[nodiscard]] static Mat4 CreateProjectionPerspectiveFOVX(const Angle& fovx, float aspectRatio,
                                                              float zNear, float zFar);

    /// @brief Create a right-handed perspective projection matrix based on a vertical field of view.
    ///
    /// @param[in] fovY 	   Vertical field of view.
    /// @param[in] aspectRatio Aspect ratio, defined as view space width divided by height.
    ///                        Determines the vertical field of view.
    /// @param[in] zNear       Distance to the near plane.
    /// @param[in] zFar		   Distance to the far plane.
    /// @remark
    ///     For typical usage, @b zNear is less than @b zFar. <br/>
    ///     However, if you flip these values so @b zFar is
    ///     less than @b zNear, the result is an inverted z buffer which can provide increased
    ///     floating-point precision.
    [[nodiscard]] static Mat4 CreateProjectionPerspectiveFOVY(const Angle& fovY, float aspectRatio,
                                                              float zNear, float zFar);

    ///@}

private:
    float mData[4][4];
};

inline constexpr Mat4 Mat4::kZero(0.F, 0.F, 0.F, 0.F, 0.F, 0.F, 0.F, 0.F, 0.F, 0.F, 0.F, 0.F, 0.F,
                                  0.F, 0.F, 0.F);
inline constexpr Mat4 Mat4::kIdentity(1.F, 0.F, 0.F, 0.F, 0.F, 1.F, 0.F, 0.F, 0.F, 0.F, 1.F, 0.F,
                                      0.F, 0.F, 0.F, 1.F);

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

/// @brief Multiply the row vector @b v by the matrix @b m
[[nodiscard]] Vec4 operator*(const Vec4& v, const Mat4& m) noexcept;
/// @}

} // namespace fuse
