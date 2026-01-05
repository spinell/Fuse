#include "Mat3.h"

#include "Angle.h"

#include <assert.h>

namespace fuse {

Mat3 operator*(const Mat3& mat, float value) noexcept {
    Mat3 result;
    for (unsigned i = 0; i < 3; i++) {
        for (unsigned j = 0; j < 3; j++) {
            result(i, j) = mat(i, j) * value;
        }
    }
    return result;
}

Mat3 operator*(float value, const Mat3& mat) noexcept { return mat * value; }

Mat3& operator*=(Mat3& mat, float value) noexcept { return mat = mat * value; }

Vec3 operator*(const Mat3& m, const Vec3& v) noexcept {
    const auto x = m(0, 0) * v.x + m(0, 1) * v.y + m(0, 2) * v.z;
    const auto y = m(1, 0) * v.x + m(1, 1) * v.y + m(1, 2) * v.z;
    const auto z = m(2, 0) * v.x + m(2, 1) * v.y + m(2, 2) * v.z;
    return {x, y, z};
}

Vec3 operator*(const Vec3& v, const Mat3& m) noexcept {
    const auto x = v.x * m(0, 0) + v.y * m(1, 0) + v.z * m(2, 0);
    const auto y = v.x * m(0, 1) + v.y * m(1, 1) + v.z * m(2, 1);
    const auto z = v.x * m(0, 2) + v.y * m(1, 2) + v.z * m(2, 2);
    return {x, y, z};
}

Mat3& Mat3::operator+=(const Mat3& other) noexcept { return *this = *this + other; }

Mat3& Mat3::operator-=(const Mat3& other) noexcept { return *this = *this - other; }

Mat3& Mat3::operator*=(const Mat3& other) noexcept { return *this = *this * other; }

Mat3 Mat3::operator+(const Mat3& other) const noexcept {
    const auto& m   = *this;
    const auto  m00 = m(0, 0) + other(0, 0);
    const auto  m01 = m(0, 1) + other(0, 1);
    const auto  m02 = m(0, 2) + other(0, 2);

    const auto m10 = m(1, 0) + other(1, 0);
    const auto m11 = m(1, 1) + other(1, 1);
    const auto m12 = m(1, 2) + other(1, 2);

    const auto m20 = m(2, 0) + other(2, 0);
    const auto m21 = m(2, 1) + other(2, 1);
    const auto m22 = m(2, 2) + other(2, 2);
    return {
      // clang-format off
        m00, m01, m02,
        m10, m11, m12,
        m20, m21, m22,
      // clang-format on
    };
}

Mat3 Mat3::operator-(const Mat3& other) const noexcept {
    const auto& m   = *this;
    const auto  m00 = m(0, 0) - other(0, 0);
    const auto  m01 = m(0, 1) - other(0, 1);
    const auto  m02 = m(0, 2) - other(0, 2);

    const auto m10 = m(1, 0) - other(1, 0);
    const auto m11 = m(1, 1) - other(1, 1);
    const auto m12 = m(1, 2) - other(1, 2);

    const auto m20 = m(2, 0) - other(2, 0);
    const auto m21 = m(2, 1) - other(2, 1);
    const auto m22 = m(2, 2) - other(2, 2);
    return {
      // clang-format off
        m00, m01, m02,
        m10, m11, m12,
        m20, m21, m22,
      // clang-format on
    };
}

Mat3 Mat3::operator*(const Mat3& other) const noexcept {
    const auto& m = *this;
    // clang-format off
    const auto m00 = m(0, 0) * other(0, 0) + m(0, 1) * other(1, 0) + m(0, 2) * other(2, 0);
    const auto m01 = m(0, 0) * other(0, 1) + m(0, 1) * other(1, 1) + m(0, 2) * other(2, 1);
    const auto m02 = m(0, 0) * other(0, 2) + m(0, 1) * other(1, 2) + m(0, 2) * other(2, 2);

    const auto m10 = m(1, 0) * other(0, 0) + m(1, 1) * other(1, 0) + m(1, 2) * other(2, 0);
    const auto m11 = m(1, 0) * other(0, 1) + m(1, 1) * other(1, 1) + m(1, 2) * other(2, 1);
    const auto m12 = m(1, 0) * other(0, 2) + m(1, 1) * other(1, 2) + m(1, 2) * other(2, 2);

    const auto m20 = m(2, 0) * other(0, 0) + m(2, 1) * other(1, 0) + m(2, 2) * other(2, 0);
    const auto m21 = m(2, 0) * other(0, 1) + m(2, 1) * other(1, 1) + m(2, 2) * other(2, 1);
    const auto m22 = m(2, 0) * other(0, 2) + m(2, 1) * other(1, 2) + m(2, 2) * other(2, 2);
    return {
        m00, m01, m02,
        m10, m11, m12,
        m20, m21, m22,
    };
    // clang-format on
}

Mat3 Mat3::CreateScaling(const Vec3& scale) noexcept {
    Mat3 mat  = Mat3::kIdentity;
    mat(0, 0) = scale.x;
    mat(1, 1) = scale.y;
    mat(2, 2) = scale.z;
    return mat;
}

Mat3 Mat3::CreateScaling(float scale, const Vec3& direction) noexcept {
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
        m00,  axay, axaz,
        axay, m11,  ayaz,
        axaz, ayaz,  m22
      // clang-format on
    };
}

Mat3 Mat3::CreateRotationX(const Angle& angle) noexcept {
    // 1      0    0
    // 0 cos(a) -sin(a)
    // 0 sin(a)  cos(a)
    const float sin = std::sin(angle);
    const float cos = std::cos(angle);
    return {1, 0, 0, 0, cos, -sin, 0, sin, cos};
}

Mat3 Mat3::CreateRotationY(const Angle& angle) noexcept {
    // cos(a)   0   sin(a)
    //      0   1     0
    // -sin(a)  0   cos(a)
    const float sin = std::sin(angle);
    const float cos = std::cos(angle);
    return {cos, 0, sin, 0, 1, 0, -sin, 0, cos};
}

Mat3 Mat3::CreateRotationZ(const Angle& angle) noexcept {
    // cos(a) -sin(a)  0
    // sin(a)  cos(a)  0
    //     0       0   1
    const float sin = std::sin(angle);
    const float cos = std::cos(angle);
    return {cos, -sin, 0, sin, cos, 0, 0, 0, 1};
}

Mat3 Mat3::CreateRotation(Angle angle, const Vec3& axis) noexcept {
    const float cos         = std::cos(angle);
    const float sin         = std::sin(angle);
    const float oneMinusCos = 1 - cos;
    const Vec3  axisNorm    = axis.normalize();
    Mat3        rot;
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

Mat3 Mat3::CreateReflection(const Vec3& normal) noexcept {
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
        m00,  axay, axaz,
        axay,  m11, ayaz,
        axaz, ayaz,  m22
      // clang-format on
    };
}

} // namespace fuse
