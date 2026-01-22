#pragma once
#include "Angle.h"

#include <cmath>
#include <format>

namespace fuse {

/// @brief 3D Vector.
/// @ingroup Math
struct Vec3 {
    /// clang-fornat off
    static const Vec3 kZero;     ///< The zero vector (0, 0, 0)
    static const Vec3 kUnitX;    ///< The X unit vector (1, 0, 0), usually facing right
    static const Vec3 kUnitXNeg; ///< The X unit vector (1, 0, 0), usually facing left
    static const Vec3 kUnitY;    ///< The Y unit vector (0, 1, 0), usually facing up
    static const Vec3 kUnitYNeg; ///< The Y unit vector (0, 1, 0), usually facing down
    static const Vec3 kUnitZ;    ///< The Z unit vector (0, 0, 1), usually facing away the screen
    static const Vec3 kUnitZNeg; ///< The Z unit vector (0, 0, 1), usually facing into the screen
    /// clang-fornat on

    /// @brief Default constructor (Does not initialize members).
    constexpr Vec3() noexcept = default;

    /// @brief Construct a vector component by component.
    /// @param x,y,z Component of the vector.
    constexpr Vec3(float x, float y, float z)
        : x{x}
        , y{y}
        , z{z} {}

    /// @brief Initializes all components with the same value.
    constexpr explicit Vec3(float value) noexcept
        : x(value)
        , y(value)
        , z(value) {}

    /// @brief Strict comparaison component by component.
    [[nodiscard]] constexpr bool operator==(const Vec3&) const noexcept = default;

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
    [[nodiscard]] Angle angleBetween(const Vec3& v) const noexcept {
        // use Kahan's Mangled Angles formula to avoid precision error at close angle.
        const float d = dot(v);
        const float c = cross(v).length();
        return radians(std::atan2(c, d));
        //return fuse::radians(std::acos(normalize().dot(v.normalize())));
    }

    /// @brief Compute the cross product of this vector with another vector (right-hand rule)
    ///
    ///  In 3D space, the cross product of 2 vector result to another
    ///  3D vector orthogonal (perpendicular) to both vector.
    ///
    ///  The cross product is defined as:
    ///  \f[
    ///  \vec{a} \times \vec{b} = (a.y \times b.z - a.z \times b.y, a.z \times b.x -
    ///  a.x \times b.z, a.x \times b.y - a.y \times b.x)
    ///  \f]
    ///
    /// Also, the cross product is defined by: \f$ \|\vec{a} \times \vec{b}\| = \|\vec{a}\| \|\vec{b}\| \sin \Theta \f$ where:
    ///  - \f$ \|\vec{a} \times \vec{b}\|  \f$ : The length of resulting vector
    ///  - \f$ \|\vec{a}\| \f$ : The length of vector @b a
    ///  - \f$ \|\vec{b}\| \f$ : The length of vector @b b
    ///  - \f$ \Theta      \f$ : The angle between vector @b a and @b b
    ///
    /// @note
    ///  - If the vectors are parallel, the cross product is zero.
    ///  - If the vectors are anti-parallel (pointing in opposite directions), the cross product is also zero.
    ///  - If both vectors are normalized, the cross product result will be a normalized perpendicular vector.
    /// @warning Cross product between 2 vectors are not commutative. ( \f$ \vec{a} \times \vec{b} = -\vec{b} \times \vec{a} \f$ )
    [[nodiscard]] constexpr Vec3 cross(const Vec3& other) const noexcept {
        return {y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x};
    }

    /// @brief Returns the distance to another vector.
    /// @return The distance to another vector.
    [[nodiscard]] float distance(const Vec3& rhs) const;

    /// @brief Returns the square of the distance to another vector.
    /// @return The square distance to another vector.
    [[nodiscard]] constexpr float distanceSquared(const Vec3& rhs) const noexcept;

    /// @brief Compute the dot product (scalar product) of this vector with another vector.
    ///
    /// The dot product is a measure of how parallel two vectors are.
    /// It is calculated as the sum of the products of their corresponding components.
    /// \f[
    /// \vec{a}\cdot \vec{b} = a.x \times b.x + a.y \times b.y + a.z \times b.z
    /// \f]
    /// Also, the dot product is defined by:
    /// \f[
    /// \vec{a}\cdot \vec{b} = \|\vec{a}\|\|\vec{b}\| \cos \Theta
    /// \f]
    /// Where,
    ///  - \f$ \|\vec{a}\| \f$ is the length is the first vector
    ///  - \f$ \|\vec{b}\| \f$ is the length is the second vector
    ///  - \f$ \Theta      \f$ : The angle between vector @b a and @b b
    ///
    /// @note
    ///  - The dot product is commutative, meaning \p a.dot(b) is equal to \p b.dot(a).
    ///  - If both vectors are normalized (unit vectors), the dot product is in range [-1,1].
    ///  - If the vectors are orthogonal (perpendicular), the dot product is zero.
    ///  - If the vectors are parallel, the dot product is equal to the product of their lengths.
    ///  - If the vectors are anti-parallel (pointing in opposite directions), the dot product is
    ///    negative and equal to the negative product of their lengths.
    [[nodiscard]] constexpr float dot(const Vec3& other) const noexcept {
        return x * other.x + y * other.y + z * other.z;
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
    [[nodiscard]] Vec3 normalize() const noexcept {
        const float len    = length();
        const float invLen = 1.f / len;
        return {x * invLen, y * invLen, z * invLen};
    }

    /// @brief Project this vector onto another vector.
    ///
    /// Projection is a method of finding components of a vector along the
    /// direction of a second vector. By projecting a vector on another vector
    /// we obtain a vector which represents the component of the first vector
    /// along the direction of the second vector. It represents the length of
    /// the shadow of a vector over another vector.
    ///
    /// For example, if you have two vectors:
    ///
    ///     a = [1, 1, 0]
    ///     b = [2, 0, 0]
    ///     the projection of a onto b is the vector [1,0,0], that lies along b
    ///     and represents how much of a is in the direction of b.
    ///
    /// It is calculated as the dot product of the first vector with the second
    /// vector, divided by the square of the length of the second vector,
    /// multiplied by the second vector.
    /// @f[
    /// \text{proj}_{\text{other}}(\text{this}) = \frac{\text{this} \cdot
    /// \text{other}}{\text{other} \cdot \text{other}} \cdot \text{other}
    /// @f]
    /// @note
    ///   - The @b other vector does not need to be normalized.
    ///   - The direction of @b other vector does not matter. [1,0,0] and [-1,0,0] will yield the same projection.
    ///   - If the projection is zero, the two vectors are orthogonal (perpendicular).
    /// @warning If the @b other vector is a zero vector, the projection is undefined.
    /// @bug     Check for @b other zero length.
    /// @param  other Vector being projected onto. Need not be normalized.
    /// @return  The projection of this vector into vector @b other.
    [[nodiscard]] constexpr Vec3 projectTo(const Vec3& other) const noexcept;

    /// @brief Compute the perpendicular projection of this vector onto another vector.
    ///
    ///  The perpendicular vector is a vector that is orthogonal (perpendicular)
    ///  to the other vector and lies in the plane defined by this vector and
    ///  the other vector.
    ///
    ///  It is calculated as the difference between this vector and its
    ///  projection onto the other vector.
    ///  \f[
    ///  \text{perp}_{\text{other}}(\text{this}) = \text{this} -
    ///  \text{proj}_{\text{other}}(\text{this})
    ///  \f]
    ///
    /// @note
    ///  The perpendicular vector is orthogonal (perpendicular) to the other
    ///  vector. It lies in the plane defined by this vector and the other
    ///  vector. The perpendicular vector is the component of this vector that
    ///  is orthogonal to the other vector. It is the vector that is left after
    ///  removing the projection of this vector onto the other vector.
    /// @warning If the @b other vector is a zero vector, the projection is undefined.
    /// @bug     Check for @b other zero length.
    /// @param  other Vector being projected onto. Need not be normalized.
    [[nodiscard]] constexpr Vec3 perpendicularTo(const Vec3& other) const noexcept;

    /// @brief   Calculates a reflection vector to the plane with the given normal .
    /// @pre     normal must be normalized.
    /// @remarks Assumes 'this' is pointing AWAY THROUGH the plane, invert if it is not.
    [[nodiscard]] Vec3 reflect(const Vec3& normal) const noexcept;

    float x; ///< The x component of the vector.
    float y; ///< The y component of the vector.
    float z; ///< The z component of the vector.
};

inline constexpr Vec3 Vec3::kZero(0.F, 0.F, 0.F);
inline constexpr Vec3 Vec3::kUnitX(1.F, 0.F, 0.F);
inline constexpr Vec3 Vec3::kUnitXNeg(-1.F, 0.F, 0.F);
inline constexpr Vec3 Vec3::kUnitY(0.F, 1.F, 0.F);
inline constexpr Vec3 Vec3::kUnitYNeg(0.F, -1.F, 0.F);
inline constexpr Vec3 Vec3::kUnitZ(0.F, 0.F, 1.F);
inline constexpr Vec3 Vec3::kUnitZNeg(0.F, 0.F, -1.F);

/// @brief Negates the vector, flipping the sign of each component.
/// @related Vec3
/// @ingroup Math
[[nodiscard]] constexpr Vec3 operator-(const Vec3& v) noexcept { return {-v.x, -v.y, -v.z}; }

/// @name Scalar / Vec3 Operations
/// @related Vec3
/// @ingroup Math
/// @{

/// @brief Multiply a vector by a scalar.
[[nodiscard]] constexpr Vec3 operator*(const Vec3& v, float s) noexcept {
    return {v.x * s, v.y * s, v.z * s};
}

/// @copydoc operator*(const Vec3&, float)
[[nodiscard]] constexpr Vec3 operator*(float s, const Vec3& v) noexcept {
    return {v.x * s, v.y * s, v.z * s};
}

/// @copydoc operator*(const Vec3&, float)
constexpr Vec3& operator*=(Vec3& v, float s) noexcept { return v = v * s; }

/// @brief
[[nodiscard]] constexpr Vec3 operator/(const Vec3& v, float s) noexcept {
    const auto inv = 1.f / s;
    return {v.x * inv, v.y * inv, v.z * inv};
}

/// @copydoc operator/(const Vec3&, float)
constexpr Vec3& operator/=(Vec3& v, float s) noexcept { return v = v / s; }

/// @}

/// @name Vec3 / Vec3 Operations
/// @related Vec3
/// @{

/// @brief
[[nodiscard]] constexpr Vec3 operator+(const Vec3& a, const Vec3& b) noexcept {
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

/// @brief
constexpr Vec3& operator+=(Vec3& a, const Vec3& b) { return a = a + b; }

/// @brief
[[nodiscard]] constexpr Vec3 operator-(const Vec3& a, const Vec3& b) noexcept {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

/// @brief
constexpr Vec3& operator-=(Vec3& a, const Vec3& b) { return a = a - b; }

/// @}

inline float Vec3::distance(const Vec3& rhs) const { return (*this - rhs).length(); }

constexpr float Vec3::distanceSquared(const Vec3& rhs) const noexcept {
    return (*this - rhs).lengthSquared();
}

constexpr Vec3 Vec3::projectTo(const Vec3& other) const noexcept {
    //             (a dot b)
    // proj(a,b)  ---------- * b
    //             (b dot b)
    return dot(other) / other.dot(other) * other;
}

constexpr Vec3 Vec3::perpendicularTo(const Vec3& other) const noexcept {
    return *this - projectTo(other);
}

inline Vec3 Vec3::reflect(const Vec3& normal) const noexcept {
    //return 2 * (dot(normal)) * normal - *this;
    return *this - (2 * dot(normal) * normal);
}

} // namespace fuse

/// \relates Vec3
/// \brief Custom std::formatter
///
/// This formater format the vector as [x,y,z].
///
/// @note All standard options are supported and they are apply component by component.
template <>
struct std::formatter<fuse::Vec3> : std::formatter<float> {

    constexpr auto parse(auto& ctx) { return std::formatter<float>::parse(ctx); }

    auto format(const fuse::Vec3& v, auto& ctx) const {
        auto&& out = ctx.out();
        std::format_to(out, "[");
        std::formatter<float>::format(v.x, ctx);
        std::format_to(out, ", ");
        std::formatter<float>::format(v.y, ctx);
        std::format_to(out, ", ");
        std::formatter<float>::format(v.z, ctx);
        return format_to(out, "]");
    }
};
