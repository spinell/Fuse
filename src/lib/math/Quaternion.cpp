#include "Quaternion.h"

#include "Angle.h"

#include <cassert>

namespace fuse {

Quaternion::Quaternion(const Vec3& axis, const Angle& angle) noexcept {
    fromAxisAngle(axis, angle);
}

std::array<Vec3, 3> Quaternion::axis() const noexcept {
    // Ensure quaternion is normalized to produce orthonormal axes
    //const float len = std::sqrt(x * x + y * y + z * z + w * w);
    float       qx  = x;
    float       qy  = y;
    float       qz  = z;
    float       qw  = w;
    //if (len > 0.f) {
    //    const float inv = 1.f / len;
    //    qx *= inv;
    //    qy *= inv;
    //    qz *= inv;
    //    qw *= inv;
    //}

    const float xx = qx * qx;
    const float yy = qy * qy;
    const float zz = qz * qz;
    const float xy = qx * qy;
    const float xz = qx * qz;
    const float yz = qy * qz;
    const float wx = qw * qx;
    const float wy = qw * qy;
    const float wz = qw * qz;

    const float m00 = 1.f - 2.f * (yy + zz);
    const float m01 = 2.f * (xy - wz);
    const float m02 = 2.f * (xz + wy);

    const float m10 = 2.f * (xy + wz);
    const float m11 = 1.f - 2.f * (xx + zz);
    const float m12 = 2.f * (yz - wx);

    const float m20 = 2.f * (xz - wy);
    const float m21 = 2.f * (yz + wx);
    const float m22 = 1.f - 2.f * (xx + yy);

    // Return columns of the rotation matrix as the rotated basis axes
    return {Vec3(m00, m10, m20), Vec3(m01, m11, m21), Vec3(m02, m12, m22)};
}

Vec3 Quaternion::axisX() const noexcept {
    return {1.f - 2.f * (y * y + z * z), 2.f * (x * y + w * z), 2.f * (x * z - w * y)};
}

Vec3 Quaternion::axisY() const noexcept {
    return {2.f * (x * y - w * z), 1.f - 2.f * (x * x + z * z), 2.f * (y * z + w * x)};
}

Vec3 Quaternion::axisZ() const noexcept {
    return {2.f * (x * z + w * y), 2.f * (y * z - w * x), 1.f - 2.f * (x * x + y * y)};
}

Mat3 Quaternion::asMatrix() const noexcept {
    const float xx  = x * x;
    const float yy  = y * y;
    const float zz  = z * z;
    const float m00 = 1 - 2 * yy - 2 * zz;
    const float m01 = 2 * x * y - 2 * w * z;
    const float m02 = 2 * x * z + 2 * w * y;
    const float m10 = 2 * x * y + 2 * w * z;
    const float m11 = 1 - 2 * xx - 2 * zz;
    const float m12 = 2 * y * z - 2 * w * x;
    const float m20 = 2 * x * z - 2 * w * y;
    const float m21 = 2 * y * z + 2 * w * x;
    const float m22 = 1 - 2 * xx - 2 * yy;
    return {
      // clang-format off
        m00, m01, m02,
        m10, m11, m12,
        m20, m21, m22
      // clang-format on
    };
}

void Quaternion::setMatrix(const Mat3& rot) noexcept {
    // from Foundations of Game Engine Development Volume 1 Mathematics
    const float m00   = rot(0, 0);
    const float m11   = rot(1, 1);
    const float m22   = rot(2, 2);
    const float trace = m00 + m11 + m22;
    if (trace > 0.0F) {
        w             = std::sqrt(trace + 1.0f) * 0.5f;
        const float f = 0.25F / w;
        x             = (rot(2, 1) - rot(1, 2)) * f;
        y             = (rot(0, 2) - rot(2, 0)) * f;
        z             = (rot(1, 0) - rot(0, 1)) * f;
    } else if ((m00 > m11) && (m00 > m22)) {
        x             = sqrt(m00 - m11 - m22 + 1.0F) * 0.5F;
        const float f = 0.25F / x;
        y             = (rot(1, 0) + rot(0, 1)) * f;
        z             = (rot(0, 2) + rot(2, 0)) * f;
        w             = (rot(2, 1) - rot(1, 2)) * f;
    } else if (m11 > m22) {
        y             = sqrt(m11 - m00 - m22 + 1.0F) * 0.5F;
        const float f = 0.25F / y;
        x             = (rot(1, 0) + rot(0, 1)) * f;
        z             = (rot(2, 1) + rot(1, 2)) * f;
        w             = (rot(0, 2) - rot(2, 0)) * f;
    } else {
        z             = sqrt(m22 - m00 - m11 + 1.0F) * 0.5F;
        const float f = 0.25F / z;
        x             = (rot(0, 2) + rot(2, 0)) * f;
        y             = (rot(2, 1) + rot(1, 2)) * f;
        w             = (rot(1, 0) - rot(0, 1)) * f;
    }
}

Quaternion& Quaternion::fromAxisAngle(const Vec3& axis, Angle angle) noexcept {
    const float halfAngle = (angle * 0.5f).asRadians();
    const float sin       = std::sin(halfAngle);
    x                     = axis.x * sin;
    y                     = axis.y * sin;
    z                     = axis.z * sin;
    w                     = std::cos(halfAngle);
    return *this;
}

void Quaternion::toAxisAngle(Vec3& pAxis, Angle& pAngle) const noexcept {
    // q = cos(A/2) + sin(A/2)*(x*i+y*j+z*k)

    const float sqrLength = x * x + y * y + z * z;
    if (sqrLength > 0.0f) {
        pAngle = radians(2.0f * std::acos(w));

        const float invLength = 1.f / std::sqrt(sqrLength);
        pAxis.x               = x * invLength;
        pAxis.y               = y * invLength;
        pAxis.z               = z * invLength;
    } else {
        // Angle is 0 (mod 2*pi), so any axis will do
        pAngle  = {};
        pAxis.x = 0.0f;
        pAxis.y = 0.0f;
        pAxis.z = 0.0f;
    }
}

Quaternion Quaternion::MakeRotationX(Angle angle) noexcept {
    const auto cos = std::cos(angle* 0.5F);
    const auto sin = std::sin(angle* 0.5F);
    return {sin, 0.0F, 0.0F, cos};
}

Quaternion Quaternion::MakeRotationY(Angle angle) noexcept {
    const auto cos = std::cos(angle* 0.5F);
    const auto sin = std::sin(angle* 0.5F);
    return {0.0F, sin, 0.0F, cos};
}

Quaternion Quaternion::MakeRotationZ(Angle angle) noexcept {
    const auto cos = std::cos(angle* 0.5F);
    const auto sin = std::sin(angle* 0.5F);
    return {0.0F, 0.0F, sin, cos};
}

} // namespace fuse
