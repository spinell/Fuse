#pragma once

namespace fuse {

/// @ingroup Math
struct Vec4 {

    Vec4() = default;

    /// @brief
    /// @param[in] x,y,z,w Coordinates of the position in 3D space.
    Vec4(float x, float y, float z, float w)
        : x{x}, y{y}, z{z}, w{w} {}

    float x;
    float y;
    float z;
    float w;
};

} // namespace fuse
