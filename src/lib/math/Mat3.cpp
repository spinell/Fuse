#include "Mat3.h"

namespace fuse {

Mat3 operator*(const Mat3& mat, float value) noexcept {
    Mat3 result;
    for(unsigned i = 0; i < 3; i++){
        for(unsigned j = 0; j < 3; j++){
            result(i,j) = mat(i,j) * value;
        }
    }
    return result;
}

Mat3 operator*(float value, const Mat3& mat) noexcept {
    return mat * value;
}

Mat3& operator*=(Mat3& mat, float value) noexcept {
    return mat = mat * value;
}


Vec3 operator*(const Mat3& m, const Vec3& v) noexcept {
    const auto x = m(0, 0) * v.x + m(0, 1) * v.y + m(0, 2) * v.z;
    const auto y = m(1, 0) * v.x + m(1, 1) * v.y + m(1, 2) * v.z;
    const auto z = m(2, 0) * v.x + m(2, 1) * v.y + m(2, 2) * v.z;
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

} // namespace fuse
