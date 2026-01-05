#pragma once
#include "Angle.h"
#include <cmath>
#include <format>

/// @addtogroup Math
/// @brief This is the group's brief description.
///
/// This is the group's detailed description.


namespace fuse {

/// @brief 3D Vector.
/// @ingroup Math
struct Vec2 {
    /// clang-fornat off
    static const Vec2 kZero;     ///< The zero vector (0, 0, 0)
    static const Vec2 kUnitX;    ///< The X unit vector (1, 0, 0), usually facing right
    static const Vec2 kUnitXNeg; ///< The X unit vector (1, 0, 0), usually facing left
    static const Vec2 kUnitY;    ///< The Y unit vector (0, 1, 0), usually facing up
    static const Vec2 kUnitYNeg; ///< The Y unit vector (0, 1, 0), usually facing down
    /// clang-fornat on

    /// @brief Default constructor (Does not initialize members).
    constexpr Vec2() noexcept = default;

    /// @brief Construct a vector component by component.
    /// @param x,y Component of the vector.
    constexpr Vec2(float x, float y)
        : x{x}
        , y{y} {}

    /// @brief Initializes all components with the same value.
    constexpr explicit Vec2(float value) noexcept
        : x(value)
        , y(value) {}

    /// @brief Strict comparaison component by component.
    [[nodiscard]] constexpr bool operator==(const Vec2&) const noexcept = default;


    /// @brief Retrieves the angle required to rotate this vector into another vector.
    ///
    /// This function return the smallest angle between this vector and vector \p v
    /// in range [0, 180] degree.
    ///
    /// @note To compute the angle between 2 vectors, both vector must be normalized.
    ///       This function will normalize both vectors.
    ///
    /// @warning Both vector must have a length > 0.
    ///
    /// @param v The other vector used to compute the angle.
    /// @return The angle needed to rotate this vector into \p v.
    [[nodiscard]] Angle angleBetween(const Vec2& v) const noexcept {
        // use Kahan's Mangled Angles formula to avoid precision error at close angle.
        return radians(std::atan2(std::abs(cross(v)), dot(v)));
        //return fuse::radians(std::acos(normalize().dot(v.normalize())));
    }

    /// @brief Calculates the 2 dimensional cross-product of 2 vectors.
    ///
    /// @note
    ///   - The sign of the 2D cross product tells you whether the second vector is on the left or right
    ///     side of the first vector (the direction of the first vector being front).
    ///   - Positive sign, v1 need to move counter clock wise to join v2
    ///   - Negative sign, v1 need to move clock clock wise to join v2
    ///   - The absolute value of the 2D cross product is the sine of the angle in between the two vectors,
    ///     so taking the arc sine of it would give you the angle in radians.
    [[nodiscard]] constexpr float cross(const Vec2& v) const noexcept {
        return x * v.y - y * v.x;
    }

    /// @brief Returns the distance to another vector.
    /// @return The distance to another vector.
    [[nodiscard]] float distance(const Vec2& rhs) const noexcept;

    /// @brief Returns the square of the distance to another vector.
    /// @return The square distance to another vector.
    [[nodiscard]] constexpr float distanceSquared(const Vec2& rhs) const noexcept;

    /// @copydoc Vec3::dot()
    [[nodiscard]] constexpr float dot(const Vec2& other) const noexcept {
        return x * other.x + y * other.y;
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
    [[nodiscard]] Vec2 normalize() const noexcept {
        const float len    = length();
        const float invLen = 1.f / len;
        return {x * invLen, y * invLen};
    }

    /// @brief Project this vector onto another vector.
    /// @warning If the @b other vector is a zero vector, the projection is undefined.
    /// @bug     Check for @b other zero length.
    /// @param  other Vector being projected onto. Need not be normalized.
    /// @return  The projection of this vector into vector @b other.
    [[nodiscard]] constexpr Vec2 projectTo(const Vec2& other) const noexcept;

    /// @brief Compute the perpendicular projection of this vector onto another vector.
    /// @warning If the @b other vector is a zero vector, the projection is undefined.
    /// @bug     Check for @b other zero length.
    /// @param  other Vector being projected onto. Need not be normalized.
    [[nodiscard]] constexpr Vec2 perpendicularTo(const Vec2& other) const noexcept;

    /// @brief   Calculates a reflection vector to the plane with the given normal .
    /// @pre     normal must be normalized.
    /// @remarks Assumes 'this' is pointing AWAY THROUGH the plane, invert if it is not.
    [[nodiscard]] Vec2 reflect(const Vec2& normal) const noexcept;

    float x; ///< The x component of the vector.
    float y; ///< The y component of the vector.
};

inline constexpr Vec2 Vec2::kZero(0.F, 0.F);
inline constexpr Vec2 Vec2::kUnitX(1.F, 0.F);
inline constexpr Vec2 Vec2::kUnitXNeg(-1.F, 0.F);
inline constexpr Vec2 Vec2::kUnitY(0.F, 1.F);
inline constexpr Vec2 Vec2::kUnitYNeg(0.F, -1.F);

/// @related Vec2
/// @ingroup Math
/// @brief Negates the vector, flipping the sign of each component.
[[nodiscard]] constexpr Vec2 operator-(const Vec2& v) noexcept { return {-v.x, -v.y}; }

/// @name Scalar / Vec2 Operations
/// @related Vec2
/// @ingroup Math
/// @{

/// @brief Multiply a vector by a scalar.
[[nodiscard]] constexpr Vec2 operator*(const Vec2& v, float s) noexcept {
    return {v.x * s, v.y * s};
}

/// @copydoc operator*(const Vec2&, float)
[[nodiscard]] constexpr Vec2 operator*(float s, const Vec2& v) noexcept {
    return {v.x * s, v.y * s};
}

/// @copydoc operator*(const Vec2&, float)
constexpr Vec2& operator*=(Vec2& v, float s) noexcept { return v = v * s; }

/// @brief
[[nodiscard]] constexpr Vec2 operator/(const Vec2& v, float s) noexcept {
    const auto inv = 1.f / s;
    return {v.x * inv, v.y * inv};
}

/// @copydoc operator/(const Vec2&, float)
constexpr Vec2& operator/=(Vec2& v, float s) noexcept { return v = v / s; }

/// @}

/// @name Vec2 / Vec2 Operations
/// @related Vec2
/// @{

/// @brief
[[nodiscard]] constexpr Vec2 operator+(const Vec2& a, const Vec2& b) noexcept {
    return {a.x + b.x, a.y + b.y};
}

/// @brief
constexpr Vec2& operator+=(Vec2& a, const Vec2& b) { return a = a + b; }

/// @brief
[[nodiscard]] constexpr Vec2 operator-(const Vec2& a, const Vec2& b) noexcept {
    return {a.x - b.x, a.y - b.y};
}

/// @brief
constexpr Vec2& operator-=(Vec2& a, const Vec2& b) { return a = a - b; }

/// @}

inline float Vec2::distance(const Vec2& rhs) const noexcept { return (*this - rhs).length(); }

constexpr float Vec2::distanceSquared(const Vec2& rhs) const noexcept {
    return (*this - rhs).lengthSquared();
}

constexpr Vec2 Vec2::projectTo(const Vec2& other) const noexcept {
    //             (a dot b)
    // proj(a,b)  ---------- * b
    //             (b dot b)
    return dot(other) / other.dot(other) * other;
}

constexpr Vec2 Vec2::perpendicularTo(const Vec2& other) const noexcept {
    return *this - projectTo(other);
}

inline Vec2 Vec2::reflect(const Vec2& normal) const noexcept {
    //return 2 * dot(normal) * normal - *this;
    return *this - (2 * dot(normal) * normal); // this point to the plane
}

} // namespace fuse

/// \relates Vec2
/// \brief Custom std::formatter
///
/// This formater format the vector as [x,y].
///
/// @note All standard options are supported and they are apply component by component.
template <>
struct std::formatter<fuse::Vec2> : std::formatter<float> {

    constexpr auto parse(auto& ctx) { return std::formatter<float>::parse(ctx); }

    auto format(const fuse::Vec2& v, auto& ctx) const {
        auto&& out = ctx.out();
        std::format_to(out, "[");
        std::formatter<float>::format(v.x, ctx);
        std::format_to(out, ", ");
        std::formatter<float>::format(v.y, ctx);
        return format_to(out, "]");
    }
};
