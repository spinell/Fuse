#include "Mat4.h"

namespace fuse {

Mat4 operator*(const Mat4& mat, float value) noexcept {
    Mat4 result;
    for(unsigned i = 0; i < 4; i++){
        for(unsigned j = 0; j < 4; j++){
            result(i,j) = mat(i,j) * value;
        }
    }
    return result;
}

Mat4 operator*(float value, const Mat4& mat) noexcept {
    return mat * value;
}

Mat4& operator*=(Mat4& mat, float value) noexcept {
    return mat = mat * value;
}


Vec4 operator*(const Mat4& m, const Vec4& v) noexcept {
    const auto x = m(0, 0) * v.x + m(0, 1) * v.y + m(0, 2) * v.z + m(0, 3) * v.w;
    const auto y = m(1, 0) * v.x + m(1, 1) * v.y + m(1, 2) * v.z + m(1, 3) * v.w;
    const auto z = m(2, 0) * v.x + m(2, 1) * v.y + m(2, 2) * v.z + m(2, 3) * v.w;
    const auto w = m(3, 0) * v.x + m(3, 1) * v.y + m(3, 2) * v.z + m(3, 3) * v.w;
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
} // namespace fuse
