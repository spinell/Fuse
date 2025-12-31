#include "GtestUtils.h"

#include <math/Mat2.h>
#include <math/Mat3.h>
#include <math/Mat4.h>
#include <math/Vec2.h>
#include <math/Vec3.h>
#include <math/Vec4.h>

#include <format>

namespace fuse {

void PrintTo(const Vec2& v, std::ostream* os) {
    *os << std::format("[{}, {}]", v.x, v.y);
}

void PrintTo(const Vec3& v, std::ostream* os) {
    *os << std::format("[{}, {}, {}]", v.x, v.y, v.z);
}

void PrintTo(const Vec4& v, std::ostream* os) {
    *os << std::format("[{}, {}, {}, {}]", v.x, v.y, v.z, v.w);
}

void PrintTo(const Mat2& v, std::ostream* os) {
    /// @todo Implement me
}

void PrintTo(const Mat3& v, std::ostream* os) {
    /// @todo Implement me
}

void PrintTo(const Mat4& v, std::ostream* os) {
    /// @todo Implement me
}


} // namespace fuse
