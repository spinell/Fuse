#pragma once

namespace fuse {

struct Vec2 {

    Vec2() = default;

    Vec2(float x, float y)
        : x{x}, y{y} {}

    float x;
    float y;
};

} // namespace fuse
