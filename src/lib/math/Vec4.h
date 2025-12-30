#pragma once

namespace fuse {

struct Vec4 {

    Vec4() = default;

    Vec4(float x, float y, float z, float w)
        : x{x}, y{y}, z{z}, w{w} {}

    float x;
    float y;
    float z;
    float w;
};

} // namespace fuse
