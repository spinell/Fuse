#pragma once

namespace fuse {

/// @ingroup Math
struct Vec3 {

    Vec3() = default;

    Vec3(float x, float y, float z)
        : x{x}, y{y}, z{z} {}

    float x;
    float y;
    float z;
};

} // namespace fuse
