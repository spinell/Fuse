#pragma once
#include <cmath>
#include <format>
#include <numbers>

namespace fuse {

/// @brief Thin wrapper around a float that represent a angle.
///
/// Angle encapsulates an angle value in a flexible way.
/// It allows for defining an angle value either as a number
/// of degrees or radians. It also works the other way
/// around. You can read an angle value as either a number
/// of degrees or radians.
///
/// Angle values support the usual mathematical operations.
/// You can add or subtract two angles, multiply or divide
/// an angle by a number, compare two angles, etc.
///
/// Angle values also provide a custom <a>
/// href="https://en.cppreference.com/w/cpp/utility/format/formatter">std::formater</a>. This
/// formater extend the float formater and output the angle in degree.
///
/// This also provide user literal.
///
/// To create an angle with a value use:
///     - fuse::degrees()
///     - fuse::radians()
///
/// To get the value of the angle use:
///     - Angle::asDegrees()
///     - Angle::asRadians()
///
/// Usage example:
/// @code
/// using namespace fuse;
/// Angle a1  = degrees(90);
/// float radians = a1.asRadians(); // 1.5708f
///
/// Angle a2 = radians(3.141592654f);
/// float degrees = a2.asDegrees(); // 180.0f
///
/// using namespace fuse::literals;
/// Angle a3 = 10_deg;   // 10 degrees
/// Angle a4 = 1.5_deg;  // 1.5 degrees
/// Angle a5 = 1_rad;    // 1 radians
/// Angle a6 = 3.14_rad; // 3.14 radians
/// @endcode
/// @ingroup Math
class Angle final {
public:
    /// @brief Default constructor. Create a angle of 0 degree.
    constexpr Angle() = default;

    // clang-format off
    [[nodiscard]] constexpr auto operator<=>(const Angle&) const = default;

    [[nodiscard]] constexpr Angle operator+() const noexcept { return mValue; }
    [[nodiscard]] constexpr Angle operator-() const noexcept { return -mValue;}

    constexpr Angle& operator+=(Angle angle) noexcept { mValue += angle.mValue; return *this; }
    constexpr Angle& operator-=(Angle angle) noexcept { mValue -= angle.mValue; return *this; }
    constexpr Angle& operator*=(float value) noexcept { mValue *= value; return *this; }
    constexpr Angle& operator/=(float value) noexcept { mValue /= value; return *this; }

    [[nodiscard]] constexpr Angle operator+(Angle angle) const noexcept { return mValue + angle.mValue; }
    [[nodiscard]] constexpr Angle operator-(Angle angle) const noexcept { return mValue - angle.mValue; }
    [[nodiscard]] constexpr Angle operator*(float value) const noexcept { return mValue * value; }
    [[nodiscard]] constexpr Angle operator/(float value) const noexcept { return mValue / value; }
    [[nodiscard]] constexpr float operator/(Angle angle) const noexcept { return mValue / angle.mValue; }
    // clang-format on

    /// @return Return the value of the angle in degree.
    [[nodiscard]] constexpr float asDegrees() const noexcept { return mValue; }

    /// @return Return the value of the angle in radian.
    [[nodiscard]] constexpr float asRadians() const noexcept {
        return mValue * std::numbers::pi_v<float> / 180.F;
    }

    /// @brief Wrap the angle in the range [-359, 359] degrees.
    /// @return Unsigned angle, wrapped to [-359, 359] degrees.
    [[nodiscard]] Angle wrap() const noexcept;

    /// @brief Wrap the angle in the range [-180, 180]
    /// @return Unsigned angle, wrapped to [-180, 180]
    [[nodiscard]] Angle wrapSigned() const noexcept;

    /// @brief Wrap the angle in the range [0, 360]
    /// @return Unsigned angle, wrapped to [0, 360]
    [[nodiscard]] Angle wrapUnsigned() const noexcept;

private:
    friend constexpr Angle degrees(float degree) noexcept;
    friend constexpr Angle radians(float radian) noexcept;

    constexpr Angle(float degree)
        : mValue(degree) {}

    /// The angle in degree.
    float mValue{};
};

/// @brief Create a new Angle from a value in degree.
/// @param degree The value in degree used to create the angle.
/// @return The new Angle.
/// @see radians()
[[nodiscard]] constexpr Angle degrees(float degree) noexcept { return {degree}; }

/// @brief Create a new Angle from a value in radian.
/// @param radian The value in radian used to create the angle.
/// @return The new Angle.
/// @see degrees()
[[nodiscard]] constexpr Angle radians(float radian) noexcept {
    return {radian * 180.F / std::numbers::pi_v<float>};
}

/// @relates Angle
[[nodiscard]] constexpr Angle operator*(float left, Angle right) noexcept {
    return degrees(left * right.asDegrees());
}

namespace literals {

/// @relates fuse::Angle
/// @brief Create a new Angle from a value in degrees.
/// @param angle The angle in degrees.
/// @return The new angle.
constexpr Angle operator""_deg(long double angle) { return degrees(static_cast<float>(angle)); }

/// @relates fuse::Angle
/// @copydoc operator""_deg()
constexpr Angle operator""_deg(unsigned long long angle) {
    return degrees(static_cast<float>(angle));
}

/// @relates fuse::Angle
/// @brief User defined literal for angles in radians
/// @param angle The angle in radian.
/// @return An angle.
constexpr Angle operator""_rad(long double angle) { return radians(static_cast<float>(angle)); }

/// @relatesalso fuse::Angle
/// @copydoc operator""_rad(long double)
constexpr Angle operator""_rad(unsigned long long angle) {
    return radians(static_cast<float>(angle));
}

} // namespace literals

} // namespace fuse

namespace std {

/// @relatesalso fuse::Angle
inline float cos(fuse::Angle angle) { return std::cos(angle.asRadians()); }

/// @relatesalso fuse::Angle
inline float sin(fuse::Angle angle) { return std::sin(angle.asRadians()); }

/// @relatesalso fuse::Angle
inline float tan(fuse::Angle angle) { return std::tan(angle.asRadians()); }

/// @relatesalso fuse::Angle
/// Custom std::formatter
template <>
struct formatter<fuse::Angle> : formatter<float> {
    auto format(const fuse::Angle& angle, auto& ctx) const {
        return std::formatter<float>::format(angle.asDegrees(), ctx);
    }
};

} // namespace std
