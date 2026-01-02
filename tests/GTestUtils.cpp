#include "GTestUtils.h"

#include <math/Angle.h>
#include <math/Mat2.h>
#include <math/Mat3.h>
#include <math/Mat4.h>
#include <math/Vec2.h>
#include <math/Vec3.h>
#include <math/Vec4.h>

#include <format>

namespace fuse {

void PrintTo(const Angle& a, std::ostream* os) {
    *os << std::format("deg={} radian={}", a.asDegrees(), a.asRadians());
}

void PrintTo(const Vec2& v, std::ostream* os) {
    *os << std::format("[{}, {}]", v.x, v.y);
}

void PrintTo(const Vec3& v, std::ostream* os) {
    *os << std::format("[{}, {}, {}]", v.x, v.y, v.z);
}

void PrintTo(const Vec4& v, std::ostream* os) {
    *os << std::format("[{}, {}, {}, {}]", v.x, v.y, v.z, v.w);
}

void PrintTo(const Mat2& m, std::ostream* os) {
    const auto row0 = std::format("{}, {}", m(0,0), m(0,1));
    const auto row1 = std::format("{}, {}", m(1,0), m(1,1));
     *os << std::format("[{}] [{}]", row0, row1);
}

void PrintTo(const Mat3& m, std::ostream* os) {
    const auto row0 = std::format("{}, {}, {}", m(0,0), m(0,1), m(0,2));
    const auto row1 = std::format("{}, {}, {}", m(1,0), m(1,1), m(1,2));
    const auto row2 = std::format("{}, {}, {}", m(1,0), m(1,1), m(2,2));
     *os << std::format("[{}] [{}] [{}]", row0, row1, row2);
}

void PrintTo(const Mat4& m, std::ostream* os) {
    const auto row0 = std::format("{}, {}, {}, {}", m(0,0), m(0,1), m(0,2), m(0,3));
    const auto row1 = std::format("{}, {}, {}, {}", m(1,0), m(1,1), m(1,2), m(1,3));
    const auto row2 = std::format("{}, {}, {}, {}", m(1,0), m(1,1), m(2,2), m(2,3));
    const auto row3 = std::format("{}, {}, {}, {}", m(1,0), m(1,1), m(2,2), m(3,3));
     *os << std::format("[{}] [{}] [{}] [{}]", row0, row1, row2, row3);
}


} // namespace fuse
