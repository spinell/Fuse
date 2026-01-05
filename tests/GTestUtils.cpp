#include "GTestUtils.h"

#include <math/Angle.h>
#include <math/Mat2.h>
#include <math/Mat3.h>
#include <math/Mat4.h>
#include <math/Quaternion.h>
#include <math/Vec2.h>
#include <math/Vec3.h>
#include <math/Vec4.h>

#include <gtest/gtest.h>

#include <format>

namespace fuse {

void PrintTo(const Angle& a, std::ostream* os) {
    *os << std::format("deg={} radian={}", a.asDegrees(), a.asRadians());
}

void PrintTo(const Vec2& v, std::ostream* os) { *os << std::format("[{}, {}]", v.x, v.y); }

void PrintTo(const Vec3& v, std::ostream* os) { *os << std::format("[{}, {}, {}]", v.x, v.y, v.z); }

void PrintTo(const Vec4& v, std::ostream* os) {
    *os << std::format("[{}, {}, {}, {}]", v.x, v.y, v.z, v.w);
}

void PrintTo(const Mat2& m, std::ostream* os) {
    const auto row0 = std::format("{}, {}", m(0, 0), m(0, 1));
    const auto row1 = std::format("{}, {}", m(1, 0), m(1, 1));
    *os << std::format("[{}] [{}]", row0, row1);
}

void PrintTo(const Mat3& m, std::ostream* os) {
    const auto row0 = std::format("{}, {}, {}", m(0, 0), m(0, 1), m(0, 2));
    const auto row1 = std::format("{}, {}, {}", m(1, 0), m(1, 1), m(1, 2));
    const auto row2 = std::format("{}, {}, {}", m(2, 0), m(2, 1), m(2, 2));
    *os << std::format("[{}] [{}] [{}]", row0, row1, row2);
}

void PrintTo(const Mat4& m, std::ostream* os) {
    const auto row0 = std::format("{}, {}, {}, {}", m(0, 0), m(0, 1), m(0, 2), m(0, 3));
    const auto row1 = std::format("{}, {}, {}, {}", m(1, 0), m(1, 1), m(1, 2), m(1, 3));
    const auto row2 = std::format("{}, {}, {}, {}", m(2, 0), m(2, 1), m(2, 2), m(2, 3));
    const auto row3 = std::format("{}, {}, {}, {}", m(3, 0), m(3, 1), m(3, 2), m(3, 3));
    *os << std::format("[{}] [{}] [{}] [{}]", row0, row1, row2, row3);
}

void PrintTo(const Quaternion& q, std::ostream* os) {
    *os << std::format("[{}, {}, {}, {}]", q.x, q.y, q.z, q.w);
}

} // namespace fuse

static bool almostEquals(float a, float b, float epsilon = std::numeric_limits<float>::epsilon()) {
    const auto diff = a - b;
    return std::abs(diff) < epsilon;
}

::testing::AssertionResult AssertAngleNear(const char* expr1, const char* expr2,
                                           const fuse::Angle& expected, const fuse::Angle& actual) {
    const auto diff = expected - actual;
    if (std::abs(diff.asRadians()) > std::numeric_limits<float>::epsilon()) {
        std::string msg = std::format("Expected equality of these angles\n", expr1, expr2, diff);
        msg += std::format(" {}\n      Which is {}\n", expr1, expected);
        msg += std::format(" {}\n      Which is {}\n", expr2, actual);
        msg += std::format(" diff\n    Which is {}\n", diff);
        return ::testing::AssertionFailure() << msg;
    }
    return ::testing::AssertionSuccess();
}

::testing::AssertionResult AssertVec2Near(const char* expr1, const char* expr2,
                                          const fuse::Vec2& expected, const fuse::Vec2& actual) {
    if (almostEquals(actual.x, expected.x) && almostEquals(actual.y, expected.y)) {
        return ::testing::AssertionSuccess();
    }
    std::string msg = std::format("Expected equality of these Vec2\n");
    msg += std::format(" {}\n      Which is {}\n", expr1, expected);
    msg += std::format(" {}\n      Which is {}\n", expr2, actual);
    return ::testing::AssertionFailure() << msg;
}

::testing::AssertionResult AssertVec3Near(const char* expr1, const char* expr2,
                                          const fuse::Vec3& expected, const fuse::Vec3& actual) {
    if (almostEquals(actual.x, expected.x, 1e-6f) && almostEquals(actual.y, expected.y, 1e-6f) &&
        almostEquals(actual.z, expected.z, 1e-6f)) {
        return ::testing::AssertionSuccess();
    }
    std::string msg = std::format("Expected equality of these Vec3\n");
    msg += std::format(" {}\n      Which is {}\n", expr1, expected);
    msg += std::format(" {}\n      Which is {}\n", expr2, actual);
    return ::testing::AssertionFailure() << msg;
}

::testing::AssertionResult AssertQuaternionNear(const char* expr1, const char* expr2,
                                                const fuse::Quaternion& expected,
                                                const fuse::Quaternion& actual, float tolerance) {
    if (std::abs(expected.x - actual.x) <= tolerance &&
        std::abs(expected.y - actual.y) <= tolerance &&
        std::abs(expected.z - actual.z) <= tolerance &&
        std::abs(expected.w - actual.w) <= tolerance) {
        return ::testing::AssertionSuccess();
    } else {
        return ::testing::AssertionFailure()
               << "  " << expr1 << " and " << expr2 << " are not close enough.\n"
               << "  Expected: (" << expected.x << ", " << expected.y << ", " << expected.z
               << expected.w << ")\n"
               << "  Actual:   (" << actual.x << ", " << actual.y << ", " << actual.z << ", "
               << actual.w << ")\n"
               << "  Tolerance: " << tolerance;
    }
}
