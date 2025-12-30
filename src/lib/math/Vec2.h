#pragma once

/// @addtogroup Math
/// @brief This is the group's brief description.
///
/// This is the group's detailed description.

namespace fuse {

/// @ingroup Math
struct Vec2 {

    Vec2() = default;

    Vec2(float x, float y)
        : x{x}, y{y} {}

    float x;
    float y;
};

} // namespace fuse
