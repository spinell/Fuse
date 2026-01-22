#include "fuse/math/Mat4.h"

#include "fuse/math/Angle.h"
#include "fuse/math/Vec3.h"

#include <assert.h>

namespace fuse {

Mat4 operator*(const Mat4& mat, float value) noexcept {
    Mat4 result;
    for (unsigned i = 0; i < 4; i++) {
        for (unsigned j = 0; j < 4; j++) {
            result(i, j) = mat(i, j) * value;
        }
    }
    return result;
}

Mat4 operator*(float value, const Mat4& mat) noexcept { return mat * value; }

Mat4& operator*=(Mat4& mat, float value) noexcept { return mat = mat * value; }

Vec4 operator*(const Mat4& m, const Vec4& v) noexcept {
    const auto x = m(0, 0) * v.x + m(0, 1) * v.y + m(0, 2) * v.z + m(0, 3) * v.w;
    const auto y = m(1, 0) * v.x + m(1, 1) * v.y + m(1, 2) * v.z + m(1, 3) * v.w;
    const auto z = m(2, 0) * v.x + m(2, 1) * v.y + m(2, 2) * v.z + m(2, 3) * v.w;
    const auto w = m(3, 0) * v.x + m(3, 1) * v.y + m(3, 2) * v.z + m(3, 3) * v.w;
    return {x, y, z, w};
}

Vec4 operator*(const Vec4& v, const Mat4& m) noexcept {
    const auto x = v.x * m(0, 0) + v.y * m(1, 0) + v.z * m(2, 0) + v.w * m(3, 0);
    const auto y = v.x * m(0, 1) + v.y * m(1, 1) + v.z * m(2, 1) + v.w * m(3, 1);
    const auto z = v.x * m(0, 2) + v.y * m(1, 2) + v.z * m(2, 2) + v.w * m(3, 2);
    const auto w = v.x * m(0, 3) + v.y * m(1, 3) + v.z * m(2, 3) + v.w * m(3, 3);
    return {x, y, z, w};
}

Mat4& Mat4::operator+=(const Mat4& other) noexcept { return *this = *this + other; }

Mat4& Mat4::operator-=(const Mat4& other) noexcept { return *this = *this - other; }

Mat4& Mat4::operator*=(const Mat4& other) noexcept { return *this = *this * other; }

Mat4 Mat4::operator+(const Mat4& other) const noexcept {
    const auto& m   = *this;
    const auto  m00 = m(0, 0) + other(0, 0);
    const auto  m01 = m(0, 1) + other(0, 1);
    const auto  m02 = m(0, 2) + other(0, 2);
    const auto  m03 = m(0, 3) + other(0, 3);

    const auto m10 = m(1, 0) + other(1, 0);
    const auto m11 = m(1, 1) + other(1, 1);
    const auto m12 = m(1, 2) + other(1, 2);
    const auto m13 = m(1, 3) + other(1, 3);

    const auto m20 = m(2, 0) + other(2, 0);
    const auto m21 = m(2, 1) + other(2, 1);
    const auto m22 = m(2, 2) + other(2, 2);
    const auto m23 = m(2, 3) + other(2, 3);

    const auto m30 = m(3, 0) + other(3, 0);
    const auto m31 = m(3, 1) + other(3, 1);
    const auto m32 = m(3, 2) + other(3, 2);
    const auto m33 = m(3, 3) + other(3, 3);
    return {
      // clang-format off
        m00, m01, m02, m03,
        m10, m11, m12, m13,
        m20, m21, m22, m23,
        m30, m31, m32, m33
      // clang-format on
    };
}

Mat4 Mat4::operator-(const Mat4& other) const noexcept {
    const auto& m   = *this;
    const auto  m00 = m(0, 0) - other(0, 0);
    const auto  m01 = m(0, 1) - other(0, 1);
    const auto  m02 = m(0, 2) - other(0, 2);
    const auto  m03 = m(0, 3) - other(0, 3);

    const auto m10 = m(1, 0) - other(1, 0);
    const auto m11 = m(1, 1) - other(1, 1);
    const auto m12 = m(1, 2) - other(1, 2);
    const auto m13 = m(1, 3) - other(1, 3);

    const auto m20 = m(2, 0) - other(2, 0);
    const auto m21 = m(2, 1) - other(2, 1);
    const auto m22 = m(2, 2) - other(2, 2);
    const auto m23 = m(2, 3) - other(2, 3);

    const auto m30 = m(3, 0) - other(3, 0);
    const auto m31 = m(3, 1) - other(3, 1);
    const auto m32 = m(3, 2) - other(3, 2);
    const auto m33 = m(3, 3) - other(3, 3);
    return {
      // clang-format off
        m00, m01, m02, m03,
        m10, m11, m12, m13,
        m20, m21, m22, m23,
        m30, m31, m32, m33
      // clang-format on
    };
}

Mat4 Mat4::operator*(const Mat4& other) const noexcept {
    const auto& m = *this;
    // clang-format off
    const auto m00 = m(0, 0) * other(0, 0) + m(0, 1) * other(1, 0) + m(0, 2) * other(2, 0) + m(0, 3) * other(3, 0);
    const auto m01 = m(0, 0) * other(0, 1) + m(0, 1) * other(1, 1) + m(0, 2) * other(2, 1) + m(0, 3) * other(3, 1);
    const auto m02 = m(0, 0) * other(0, 2) + m(0, 1) * other(1, 2) + m(0, 2) * other(2, 2) + m(0, 3) * other(3, 2);
    const auto m03 = m(0, 0) * other(0, 3) + m(0, 1) * other(1, 3) + m(0, 2) * other(2, 3) + m(0, 3) * other(3, 3);

    const auto m10 = m(1, 0) * other(0, 0) + m(1, 1) * other(1, 0) + m(1, 2) * other(2, 0) + m(1, 3) * other(3, 0);
    const auto m11 = m(1, 0) * other(0, 1) + m(1, 1) * other(1, 1) + m(1, 2) * other(2, 1) + m(1, 3) * other(3, 1);
    const auto m12 = m(1, 0) * other(0, 2) + m(1, 1) * other(1, 2) + m(1, 2) * other(2, 2) + m(1, 3) * other(3, 2);
    const auto m13 = m(1, 0) * other(0, 3) + m(1, 1) * other(1, 3) + m(1, 2) * other(2, 3) + m(1, 3) * other(3, 3);

    const auto m20 = m(2, 0) * other(0, 0) + m(2, 1) * other(1, 0) + m(2, 2) * other(2, 0) + m(2, 3) * other(3, 0);
    const auto m21 = m(2, 0) * other(0, 1) + m(2, 1) * other(1, 1) + m(2, 2) * other(2, 1) + m(2, 3) * other(3, 1);
    const auto m22 = m(2, 0) * other(0, 2) + m(2, 1) * other(1, 2) + m(2, 2) * other(2, 2) + m(2, 3) * other(3, 2);
    const auto m23 = m(2, 0) * other(0, 3) + m(2, 1) * other(1, 3) + m(2, 2) * other(2, 3) + m(2, 3) * other(3, 3);

    const auto m30 = m(3, 0) * other(0, 0) + m(3, 1) * other(1, 0) + m(3, 2) * other(2, 0) + m(3, 3) * other(3, 0);
    const auto m31 = m(3, 0) * other(0, 1) + m(3, 1) * other(1, 1) + m(3, 2) * other(2, 1) + m(3, 3) * other(3, 1);
    const auto m32 = m(3, 0) * other(0, 2) + m(3, 1) * other(1, 2) + m(3, 2) * other(2, 2) + m(3, 3) * other(3, 2);
    const auto m33 = m(3, 0) * other(0, 3) + m(3, 1) * other(1, 3) + m(3, 2) * other(2, 3) + m(3, 3) * other(3, 3);
    return {
        m00, m01, m02, m03,
        m10, m11, m12, m13,
        m20, m21, m22, m23,
        m30, m31, m32, m33
    };
    // clang-format on
}

// =========================================================
//                      Transform
// =========================================================

Mat4 Mat4::CreateTranslation(const Vec3& translation) noexcept {
    Mat4 mat  = Mat4::kIdentity;
    mat(0, 3) = translation.x;
    mat(1, 3) = translation.y;
    mat(2, 3) = translation.z;
    return mat;
}

Mat4 Mat4::CreateScaling(const Vec3& scale) noexcept {
    Mat4 mat  = Mat4::kIdentity;
    mat(0, 0) = scale.x;
    mat(1, 1) = scale.y;
    mat(2, 2) = scale.z;
    return mat;
}

Mat4 Mat4::CreateScaling(float scale, const Vec3& direction) noexcept {
    //assert(direction.lengthSquared() == 1 && "The normal vector must be non-zero.");
    scale -= 1.f;
    const float x    = direction.x * scale;
    const float y    = direction.y * scale;
    const float z    = direction.z * scale;
    const float axay = x * direction.y;
    const float axaz = x * direction.z;
    const float ayaz = y * direction.z;
    const float m00  = x * direction.x + 1.f;
    const float m11  = y * direction.y + 1.f;
    const float m22  = z * direction.z + 1.f;
    return {
      // clang-format off
        m00,  axay, axaz, 0.f,
        axay, m11,  ayaz, 0.f,
        axaz, ayaz,  m22, 0.f,
           0,    0,    0, 1.f
      // clang-format on
    };
}

Mat4 Mat4::CreateRotationX(const Angle& angle) noexcept {
    // 1      0    0    0
    // 0 cos(a) -sin(a) 0
    // 0 sin(a)  cos(a) 0
    // 0      0     1   1
    const float sin = std::sin(angle);
    const float cos = std::cos(angle);

    Mat4 rot  = Mat4::kIdentity;
    rot(1, 1) = cos;
    rot(1, 2) = -sin;
    rot(2, 1) = sin;
    rot(2, 2) = cos;
    return rot;
}

Mat4 Mat4::CreateRotationY(const Angle& angle) noexcept {
    // cos(a)   0   sin(a)  0
    //      0   1     0     0
    // -sin(a)  0   cos(a)  0
    //    0     0     1     1
    const float sin = std::sin(angle);
    const float cos = std::cos(angle);

    Mat4 rot  = Mat4::kIdentity;
    rot(0, 0) = cos;
    rot(0, 2) = sin;
    rot(2, 0) = -sin;
    rot(2, 2) = cos;
    return rot;
}

Mat4 Mat4::CreateRotationZ(const Angle& angle) noexcept {
    // cos(a) -sin(a)  0  0
    // sin(a)  cos(a)  0  0
    //     0       0   1  0
    //     0       0   0  1
    const float sin = std::sin(angle);
    const float cos = std::cos(angle);

    Mat4 rot  = Mat4::kIdentity;
    rot(0, 0) = cos;
    rot(0, 1) = -sin;
    rot(1, 0) = sin;
    rot(1, 1) = cos;
    return rot;
}

Mat4 Mat4::CreateRotation(Angle angle, const Vec3& axis) noexcept {
    const float cos         = std::cos(angle);
    const float sin         = std::sin(angle);
    const float oneMinusCos = 1 - cos;
    const Vec3  axisNorm    = axis.normalize();
    Mat4        rot         = Mat4::kIdentity;
    // First row
    rot(0, 0) = cos + oneMinusCos * (axisNorm.x * axisNorm.x);
    rot(0, 1) = axisNorm.x * axisNorm.y * oneMinusCos - axisNorm.z * sin;
    rot(0, 2) = axisNorm.x * axisNorm.z * oneMinusCos + axisNorm.y * sin;

    // Second row
    rot(1, 0) = axisNorm.y * axisNorm.x * oneMinusCos + axisNorm.z * sin;
    rot(1, 1) = cos + (axisNorm.y * axisNorm.y) * oneMinusCos;
    rot(1, 2) = axisNorm.y * axisNorm.z * oneMinusCos - axisNorm.x * sin;

    // Third row
    rot(2, 0) = axisNorm.z * axisNorm.x * oneMinusCos - axisNorm.y * sin;
    rot(2, 1) = axisNorm.z * axisNorm.y * oneMinusCos + axisNorm.x * sin;
    rot(2, 2) = cos + (axisNorm.z * axisNorm.z) * oneMinusCos;
    return rot;
}

Mat4 Mat4::CreateReflection(const Vec3& normal) noexcept {
    assert(normal.lengthSquared() == 1 && "The normal vector must be non-zero.");
    const float x    = normal.x * -2.f;
    const float y    = normal.y * -2.f;
    const float z    = normal.z * -2.f;
    const float axay = x * normal.y;
    const float axaz = x * normal.z;
    const float ayaz = y * normal.z;
    const float m00  = x * normal.x + 1.f;
    const float m11  = y * normal.y + 1.f;
    const float m22  = z * normal.z + 1.f;
    return {
      // clang-format off
        m00,  axay, axaz, 0.f,
        axay,  m11, ayaz, 0.f,
        axaz, ayaz,  m22, 0.f,
         0.f,  0.f,  0.f, 1.f
      // clang-format on
    };
}

// ======================================================
//                   View matrix
// ======================================================

Mat4 Mat4::CreateViewLookAt(const Vec3& position, const Vec3& target,
                            const Vec3& upVector) noexcept {
    assert(target != position);
    // View matrix
    //   Rx  Ry  Rz -Tx
    //   Ux  Uy  Uz -Ty
    //   Dx  Dy  Dz -Tz
    //    0  0   0   1
    //
    // Where:
    //	R => the right vector (x-axis)
    //	U => the up vector (y-axis)
    //	D => the direction vector (z-axis)
    //	T => the translation vector
    //
    const auto zAxis = (position - target).normalize();   // The "forward" vector.
    const auto xAxis = upVector.cross(zAxis).normalize(); // The "right" vector.
    const auto yAxis = zAxis.cross(xAxis).normalize();    // The "up" vector.

    Mat4 mat = Mat4::kIdentity;
    // set the rotation part.
    mat(0, 0) = xAxis.x;
    mat(0, 1) = xAxis.y;
    mat(0, 2) = xAxis.z;

    mat(1, 0) = yAxis.x;
    mat(1, 1) = yAxis.y;
    mat(1, 2) = yAxis.z;

    mat(2, 0) = zAxis.x;
    mat(2, 1) = zAxis.y;
    mat(2, 2) = zAxis.z;

    // set the translation part
    mat(0, 3) = -xAxis.dot(position);
    mat(1, 3) = -yAxis.dot(position);
    mat(2, 3) = -zAxis.dot(position);

    mat(3, 0) = 0.0f;
    mat(3, 1) = 0.0f;
    mat(3, 2) = 0.0f;
    mat(3, 3) = 1.0f;
    return mat;
}

Mat4 Mat4::CreateViewLookTo(const Vec3& position, const Vec3& direction,
                            const Vec3& upVector) noexcept {

    const auto target = position + direction;
    return CreateViewLookAt(position, target, upVector);
}

// ======================================================
//                  Projection matrix
// ======================================================

Mat4 Mat4::ProjOrtho(float width, float height, float near, float far) noexcept {
    return ProjOrthoOffCenter(-width * 0.5f,
                              width * 0.5f,
                              -height * 0.5f,
                              height * 0.5f,
                              near,
                              far);
}

Mat4 Mat4::ProjOrthoOffCenter(float left, float right, float bottom, float top, float near /* =-1*/,
                              float far /*= 1*/) noexcept {
    // ============================================================
    //  P = A(ATA)−1AT
    //  The projection matrix will be defined as fallow
    //
    //  A  0  0  tx
    //  0  B  0  ty
    //  0  0  C  tz
    //  0  0  0  1
    //
    // Where:
    //  A=>     2 / ( right - left )
    //  B=>     2 / ( top - bottom )
    //  C=>   - 2 / ( far - near )
    //  tx=>  -(right + left) / (right - left)
    //  ty=>  -(top + bottom) / (top - bottom)
    //  tz=>  -(far + near)   / (far - near)
    //
    // ============================================================

    const float width  = (right - left);
    const float height = (top - bottom);
    const float depth  = (far - near);

    const float invWidth  = 1.0f / width;
    const float invHeight = 1.0f / height;
    const float invDepth  = 1.0f / depth;

    // compute the translation
    const float tx = -(right + left) * invWidth;
    const float ty = -(top + bottom) * invHeight;
    const float tz = -(far + near) * invDepth;

    Mat4 mat;

    mat(0, 0) = 2.0f * invWidth;
    mat(0, 1) = 0.0f;
    mat(0, 2) = 0.0f;
    mat(0, 3) = tx;

    mat(1, 0) = 0.0f;
    mat(1, 1) = 2.0f * invHeight;
    mat(1, 2) = 0.0f;
    mat(1, 3) = ty;

    mat(2, 0) = 0.0f;
    mat(2, 1) = 0.0f;
    mat(2, 2) = -2.0f * invDepth;
    mat(2, 3) = tz;

    mat(3, 0) = 0.0f;
    mat(3, 1) = 0.0f;
    mat(3, 2) = 0.0f;
    mat(3, 3) = 1.0f;
    return mat;
}

Mat4 Mat4::CreateProjectionPerspectiveOffCenter(float left, float right, float bottom, float top,
                                                float zNear, float zFar) noexcept {
    // ============================================================
    //
    //  The projection matrix will be defined as fallow
    //
    //  A  0  B  0
    //  0  C  D  0
    //  0  0  E  F
    //  0  0 -1  0
    //
    // Where:
    //  A =>   (2 * near) / (right - left)
    //  B =>   (right + left) / (right - left)
    //  C =>   (2 * near) / ( top - bottom )
    //  D =>   (top + bottom) / (top - bottom)
    //  E =>  -(far + near) / (far - near)
    //  F =>  -( 2 * far * near) / (far - near)
    // ============================================================

    assert(zNear > 0);
    assert(zFar > 0);
    assert(zNear != 0);

    const float width     = right - left;
    const float height    = top - bottom;
    const float depth     = zFar - zNear;
    const float invWidth  = 1.0f / width;
    const float invHeight = 1.0f / height;
    const float invDepth  = 1.0f / depth;

    const float a = 2.0f * zNear * invWidth;
    const float b = (right + left) * invWidth;
    const float c = 2.0f * zNear * invHeight;
    const float d = (top + bottom) * invHeight;
    const float e = -(zFar + zNear) * invDepth;
    const float f = -2.0f * zFar * zNear * invDepth;

    Mat4 matrix = Mat4::kIdentity;
    // clang-format off
    matrix(0, 0) = a;    matrix(0, 1) = 0.0f; matrix(0, 2) =  b;   matrix(0, 3) = 0.0f;
    matrix(1, 0) = 0.0f; matrix(1, 1) = c;    matrix(1, 2) =  d;   matrix(1, 3) = 0.0f;
    matrix(2, 0) = 0.0f; matrix(2, 1) = 0.0f; matrix(2, 2) =  e;   matrix(2, 3) = f;
    matrix(3, 0) = 0.0f; matrix(3, 1) = 0.0f; matrix(3, 2) = -1.f; matrix(3, 3) = 0.0f;
    // clang-format on
    return matrix;
}

Mat4 Mat4::CreateProjectionPerspectiveFOVX(const Angle& fovx, float aspectRatio, float zNear,
                                           float zFar) {
    // =====================================
    // Frustum view from top
    //
    //       \     w              /
    //        \---------         /
    //         \       |        /
    //          \      |       /
    //           \     | n    /
    //            \    |     /
    //             \   |    /
    //              \ a|   /
    //               \ |  /
    //                \| /
    //                 \/
    //  Where:
    //      w => half width of the near plane
    //      n => distance of the near plane
    //      a => half fov x
    //
    // By definition tan(x) = opposite / adjacent
    //  tan(a) = w / n => w = n * tan(a)
    //
    // =====================================
    const float nearHalfWidth  = zNear * std::tan(0.5f * fovx);
    const float nearHalfHeight = nearHalfWidth / aspectRatio;
    return CreateProjectionPerspectiveOffCenter(-nearHalfWidth,
                                                nearHalfWidth,
                                                -nearHalfHeight,
                                                nearHalfHeight,
                                                zNear,
                                                zFar);
}

Mat4 Mat4::CreateProjectionPerspectiveFOVY(const Angle& fovY, float aspectRatio, float zNear,
                                           float zFar) {
    // =====================================
    //  Frustum view from side
    //
    //          /
    //         / |
    //        /  |
    //       /   |
    //      /    | h
    //     /     |
    //      a--------
    //     \    n
    //      \
    //       \
    //  Where:
    //      h => half height of the near plane
    //      n => distance of the near plane
    //      a => half fov y
    //
    // By definition tan(x) = opposite / adjacent so,
    //  tan(a) = h / n => h = n * tan(a)
    // =====================================
    const float nearHalfHeight = zNear * std::tan(fovY * 0.5f);
    const float nearHalfWidth  = nearHalfHeight * aspectRatio;
    return CreateProjectionPerspectiveOffCenter(-nearHalfWidth,
                                                nearHalfWidth,
                                                -nearHalfHeight,
                                                nearHalfHeight,
                                                zNear,
                                                zFar);
}


} // namespace fuse
