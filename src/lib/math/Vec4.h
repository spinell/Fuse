#pragma once

#include <cmath>
#include <format>

namespace fuse {


/// @brief 4D Vector
/// @ingroup Math
struct Vec4 {

    // clang-fornat off
    static const Vec4 kZero;  ///< The zero vector   (0, 0, 0, 0)
    static const Vec4 kUnitX; ///< The unit X vector (1, 0, 0, 0)
    static const Vec4 kUnitXNeg;
    static const Vec4 kUnitY; ///< The unit Y vector (0, 1, 0, 0)
    static const Vec4 kUnitYNeg;
    static const Vec4 kUnitZ; ///< The unit Z vector (0, 0, 1, 0)
    static const Vec4 kUnitZNeg;
    static const Vec4 kUnitW; ///< The unit W vector (0, 0, 0, 1)
    // clang-fornat on

    /// @brief Default constructor (Does not initialize members).
    Vec4() = default;

    /// \brief Constructor that sets components explicitly.
    /// \param x,y,z,w The components of the vector.
    constexpr Vec4(float x, float y, float z, float w) noexcept
        : x(x)
        , y(y)
        , z(z)
        , w(w) {}

    /// @brief Strict comparaison component by component.
    [[nodiscard]] constexpr bool operator==(const Vec4&) const noexcept = default;

    /// @copydoc Vec3::dot()
    [[nodiscard]] constexpr float dot(const Vec4& other) const noexcept {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }

    /// @brief Compute the length (magnitude) of this vector.
    /// @note If the exact length is not required, by example for comparing
    ///       the distance of 2 vectors, we can use lengthSquared() which is faster.
    /// @return Returns the length of this vector.
    [[nodiscard]] float length() const noexcept { return std::sqrt(dot(*this)); }

    /// @brief Compute the square length (magnitude) of this vector.
    /// @return Returns the square length of this vector.
    [[nodiscard]] constexpr float lengthSquared() const noexcept { return dot(*this); }

    /// @brief Normalize the vector.
    /// @bug When vector is 0 length.
    [[nodiscard]] Vec4 normalize() const noexcept {
        const float len    = length();
        const float invLen = 1.f / len;
        return {x * invLen, y * invLen, z * invLen, w * invLen};
    }

    float x; ///< The x component of the vector
    float y; ///< The y component of the vector
    float z; ///< The x component of the vector
    float w; ///< The z component of the vector
};

static_assert(sizeof(Vec4) == 16, "Vec4 size must be 16 bytes.");

// clang-fornat off
inline constexpr Vec4 Vec4::kZero     = {0.F, 0.F, 0.F, 0.F};
inline constexpr Vec4 Vec4::kUnitX    = {1.F, 0.F, 0.F, 0.F};
inline constexpr Vec4 Vec4::kUnitXNeg = {-1.F, 0.F, 0.F, 0.F};
inline constexpr Vec4 Vec4::kUnitY    = {0.F, 1.F, 0.F, 0.F};
inline constexpr Vec4 Vec4::kUnitYNeg = {0.F, -1.F, 0.F, 0.F};
inline constexpr Vec4 Vec4::kUnitZ    = {0.F, 0.F, 1.F, 0.F};
inline constexpr Vec4 Vec4::kUnitZNeg = {0.F, 0.F, -1.F, 0.F};
inline constexpr Vec4 Vec4::kUnitW    = {0.F, 0.F, 0.F, 1.F};

// clang-fornat on

/// @brief Negates the vector, flipping the sign of each component.
/// @related Vec4
/// @ingroup Math
[[nodiscard]] inline constexpr Vec4 operator-(const Vec4& v) noexcept {
    return {-v.x, -v.y, -v.z, -v.w};
}

/// @name Operations Scalar / Vec4
/// @related Vec4
/// @ingroup Math
/// @{

/// @brief Multiply a vector by a scalar.
[[nodiscard]] inline constexpr Vec4 operator*(const Vec4& lhs, float rhs) noexcept {
    return {lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs};
}

/// @brief Multiply a scalar by a vector.
[[nodiscard]] inline constexpr Vec4 operator*(float n, const Vec4& v) noexcept { return v * n; }

/// @copydoc operator*(const Vec4&, float)
constexpr Vec4& operator*=(Vec4& v, float s) noexcept { return v = v * s; }

/// @brief
[[nodiscard]] inline constexpr Vec4 operator/(const Vec4& v, float n) noexcept {
    const float inv = 1.f / n;
    return v * inv;
}

/// @copydoc operator/(const Vec4&, float)
constexpr Vec4& operator/=(Vec4& v, float s) noexcept { return v = v / s; }

/// @}


/// @name Operations Vec4 / Vec4
/// @related Vec4
/// @{

/// @brief Addition 2 vector.
[[nodiscard]] inline constexpr Vec4 operator+(const Vec4& lhs, const Vec4& rhs) noexcept {
    return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w};
}

/// @brief Addition 2 vector.
constexpr Vec4& operator+=(Vec4& a, const Vec4& b) { return a = a + b; }

/// @brief Substract 2 vector.
[[nodiscard]] inline constexpr Vec4 operator-(const Vec4& lhs, const Vec4& rhs) noexcept {
    return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w};
}

/// @brief Substract 2 vector.
constexpr Vec4& operator-=(Vec4& a, const Vec4& b) { return a = a - b; }

/// @}


} // namespace fuse

/// \relates Vec4
/// \brief Custom std::formatter
///
/// This formater format the vector as [x,y,z,w].
///
/// @note All standard options are supported and they are apply component by component.
template <>
struct std::formatter<fuse::Vec4> : std::formatter<float> {

    constexpr auto parse(auto& ctx) { return std::formatter<float>::parse(ctx); }

    auto format(const fuse::Vec4& v, auto& ctx) const {
        auto&& out = ctx.out();
        std::format_to(out, "[");
        std::formatter<float>::format(v.x, ctx);
        std::format_to(out, ", ");
        std::formatter<float>::format(v.y, ctx);
        std::format_to(out, ", ");
        std::formatter<float>::format(v.z, ctx);
        std::format_to(out, ", ");
        std::formatter<float>::format(v.w, ctx);
        return format_to(out, "]");
    }
};
