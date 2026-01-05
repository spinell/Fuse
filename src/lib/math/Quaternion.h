#pragma once
#include "Mat3.h"
#include "Vec3.h"

#include <array>
#include <format>

namespace fuse {
class Angle;

/// @brief A quaternion represents rotation in 3D space.
struct Quaternion {
public:
    static const Quaternion kIdentity; ///< The identity quaternion.

    /// @brief Default constructor that leaves the components uninitialized.
    Quaternion() = default;

    /// @brief Constructor that sets components explicitly.
    /// @param x,y,z The components of the vector part.
    /// @param w The scalar part.
    constexpr Quaternion(float x, float y, float z, float w)
        : x(x)
        , y(y)
        , z(z)
        , w(w) {}

    /// @brief Constructor that sets components explicitly.
    /// @param v The components of the vector part.
    /// @param w The scalar part.
    constexpr Quaternion(const Vec3& v, float w)
        : x(v.x)
        , y(v.y)
        , z(v.z)
        , w(w) {}

    /// @brief Construct a quaternion from an axis ans a angle.
    /// @param axis The axis of rotation.
    /// @param angle The angle of rotation.
    Quaternion(const Vec3& axis, const Angle& angle) noexcept;

    /// @brief Strict comparaison component by component.
    [[nodiscard]] constexpr bool operator==(const Quaternion&) const noexcept = default;

    /// @brief Return the conjugate (reverse) of this quaternion.
    /// The conjugate of a quaternion is contained by negating the vector part (x,y,z) while keeping the scaler part (w) unchanged.
    [[nodiscard]] constexpr Quaternion conjugate() const noexcept { return {-x, -y, -z, w}; }

    /// @brief Returns the dot product.
    [[nodiscard]] constexpr float dot(const Quaternion& other) const noexcept {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }

    /// @brief Return the inverse of this quaternion.
    /// The inverse quaternion reverse the original rotation.
    /// @todo check division by 0 ?
    [[nodiscard]] constexpr Quaternion inverse() const noexcept;

    /// @brief Normalize the quaternion.
    /// @bug When vector is 0 length.
    [[nodiscard]] Quaternion normalize() const noexcept {
        const float len    = length();
        const float invLen = 1.f / len;
        return {x * invLen, y * invLen, z * invLen, w * invLen};
    }

    /// @brief Compute the length (magnitude) of this vector.
    /// @return Returns the length of this vector.
    [[nodiscard]] float length() const noexcept { return std::sqrt(x * x + y * y + z * z + w * w); }

    /// @brief Compute the square length (magnitude) of this vector.
    /// @return Returns the square length of this vector.
    [[nodiscard]] constexpr float lengthSquared() const noexcept {
        return (x * x + y * y + z * z + w * w);
    }

    /// @brief Return the x-axis of the orientation represented by this quaternion.
    /// This is equivalent to multiply the vector [1,0,0] by this quaternion.
    [[nodiscard]] Vec3 axisX() const noexcept;

    /// @brief Return the y-axis of the orientation represented by this quaternion.
    /// This is equivalent to multiply the vector [0,1,0] by this quaternion.
    [[nodiscard]] Vec3 axisY() const noexcept;

    /// @brief Return the z-axis of the orientation represented by this quaternion.
    /// This is equivalent to multiply the vector [0,0,1] by this quaternion.
    [[nodiscard]] Vec3 axisZ() const noexcept;

    /// @brief Return the axis of the orientation represented by this quaternion.
    /// @see axisX() axisY() axisZ()
    [[nodiscard]] std::array<Vec3, 3> axis() const noexcept;

    /// @brief Converts the to a 3x3 matrix.
    [[nodiscard]] Mat3 asMatrix() const noexcept;

    /// @brief Convert a matrix to a quaternion.
    /// Sets the components of this quaternion to values that represents the same rotation as the
    /// one represented by the @b matrix.
	///
    /// @note This function expects the @b matrix to be orthogonal and have a determinant of +1.
    /// If these conditions are not met, then the results are unlikely to be meaningful.
    /// @param matrix The matrix to convert.
    void setMatrix(const Mat3& matrix) noexcept;

    /// @brief Define this quaternion has a rotation around an axis.
    /// @param axis The axis about which to rotate.
    /// @param angle The angle of rotation
    /// @pre This vector @b axis must have unit magnitude.
    Quaternion& fromAxisAngle(const Vec3& axis, Angle angle) noexcept;

    /// @brief Convert the quaternion to an axis and angle.
    /// @param axis The axis of rotation.
    /// @param angle The angle of rotation.
    void toAxisAngle(Vec3& axis, Angle& angle) const noexcept;

    [[nodiscard]] static Quaternion MakeRotationX(Angle angle) noexcept;
    [[nodiscard]] static Quaternion MakeRotationY(Angle angle) noexcept;
    [[nodiscard]] static Quaternion MakeRotationZ(Angle angle) noexcept;

    float x;
    float y;
    float z;
    float w;
};

inline constexpr Quaternion Quaternion::kIdentity(0, 0, 0, 1);

/// @brief Addition 2 quaternions.
[[nodiscard]] constexpr Quaternion operator+(const Quaternion& q1, const Quaternion& q2) noexcept {
    return {q1.x + q2.x, q1.y + q2.y, q1.z + q2.z, q1.w + q2.w};
}

/// @brief Substract 2 quaternions.
[[nodiscard]] constexpr Quaternion operator-(const Quaternion& q1, const Quaternion& q2) noexcept {
    return {q1.x - q2.x, q1.y - q2.y, q1.z - q2.z, q1.w - q2.w};
}

/// @brief Multiply a scalar with a quaternion.
[[nodiscard]] constexpr Quaternion operator*(float scalar, const Quaternion& q) noexcept {
    return {q.x * scalar, q.y * scalar, q.z * scalar, q.w * scalar};
}

/// @brief Multiply a quaternion by a scalar.
[[nodiscard]] constexpr Quaternion operator*(const Quaternion& q, float scalar) noexcept {
    return {q.x * scalar, q.y * scalar, q.z * scalar, q.w * scalar};
}

/// @brief Multiply a quaternion by a scalar.
constexpr Quaternion& operator*=(Quaternion& q, float scalar) noexcept { return q = q * scalar; }

/// @brief Multiply 2 quaternions.
/// @note Quaternion multiplication is not commutative (Except if the vector part are parallel).
[[nodiscard]] constexpr Quaternion operator*(const Quaternion& q1, const Quaternion& q2) noexcept {
    return {q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
            q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x,
            q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w,
            q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z};
}

/// @brief Multiply 2 quaternions.
/// @note Quaternion multiplication is not commutative.
constexpr Quaternion& operator*=(Quaternion& q1, const Quaternion& q2) noexcept {
    return q1 = q1 * q2;
}

/// @brief Multiply (rotate) a vector with a quaternion.
[[nodiscard]] constexpr Vec3 operator*(const Quaternion& q, const Vec3& v) noexcept {
#define IMPL 2
#if IMPL == 0
    // nVidia SDK implementation
    Vec3 qvec(q.x, q.y, q.z);
    Vec3 uv  = qvec.cross(v);
    Vec3 uuv = qvec.cross(uv);
    uv *= (2.0f * q.w);
    uuv *= 2.0f;
    return v + uv + uuv;
#elif IMPL == 1
    // https://gamedev.stackexchange.com/questions/28395/rotating-vector3-by-a-quaternion
    const Vec3  u(q.x, q.y, q.z);
    const float s = q.w;
    return  2.0f * u.dot(v) * u +
            (s * s - u.dot(u)) * v +
            2.0f * s * u.cross(v);
#else
    // From Foundations of Game Engine Development Volume Mathematics
    Vec3 b(q.x, q.y, q.z);
    const float b2 = b.x * b.x + b.y * b.y + b.z * b.z;
    return  2.0F * v.dot(b) * b +
            (q.w * q.w - b2) * v +
            2.0F * q.w * b.cross(v);
#endif
#undef IMPL
}

/// @brief Divide a quaternion by a scalar.
[[nodiscard]] constexpr Quaternion operator/(const Quaternion& q, float scalar) noexcept {
    const auto inv = 1.f / scalar;
    return {q.x * inv, q.y * inv, q.z * inv, q.w * inv};
}

/// @brief Divide a quaternion by a scalar.
constexpr Quaternion& operator/=(Quaternion& q, float scalar) noexcept { return q = q / scalar; }

constexpr Quaternion Quaternion::inverse() const noexcept {
    const float inv = 1.0f / lengthSquared();
    return conjugate() * inv;
}

} // namespace fuse

/// @relates Quaternion
/// @brief Custom std::formatter
///
/// This formater format the Quaternion as [x,y,z,w].
///
/// @note All standard options are supported and they are apply component by component.
template <>
struct std::formatter<fuse::Quaternion> : std::formatter<float> {

    constexpr auto parse(auto& ctx) { return std::formatter<float>::parse(ctx); }

    auto format(const fuse::Quaternion& q, auto& ctx) const {
        auto&& out = ctx.out();
        std::format_to(out, "[");
        std::formatter<float>::format(q.x, ctx);
        std::format_to(out, ", ");
        std::formatter<float>::format(q.y, ctx);
        std::format_to(out, ", ");
        std::formatter<float>::format(q.z, ctx);
        std::format_to(out, ", ");
        std::formatter<float>::format(q.w, ctx);
        return format_to(out, "]");
    }
};
