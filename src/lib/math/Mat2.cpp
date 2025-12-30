#pragma once
#include "Mat2.h"

namespace fuse {

Vec2 operator*(const Mat2& m, const Vec2& v) noexcept {
    const auto x = m(0, 0) * v.x + m(0, 1) * v.y;
    const auto y = m(1, 0) * v.x + m(1, 1) * v.y;
    return {x, y};
}

Mat2& Mat2::operator*=(const Mat2& other) noexcept{
    return *this = *this * other;
}

Mat2 Mat2::operator*(const Mat2& other) const noexcept{
    const auto& m = *this;
    // clang-format off
    const auto m00 = m(0, 0) * other(0, 0) + m(0, 1) * other(1, 0);
    const auto m01 = m(0, 0) * other(0, 1) + m(0, 1) * other(1, 1);

    const auto m10 = m(1, 0) * other(0, 0) + m(1, 1) * other(1, 0);
    const auto m11 = m(1, 0) * other(0, 1) + m(1, 1) * other(1, 1);
    return {
        m00, m01,
        m10, m11
    };
    // clang-format on
}

} // namespace fuse
