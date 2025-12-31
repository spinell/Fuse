#pragma once
#include <ostream>

namespace fuse {

class Vec2;
class Vec3;
class Vec4;
class Mat2;
class Mat3;
class Mat4;

// teach gtest how to print data
void PrintTo(const Vec2& m, std::ostream* os);
void PrintTo(const Vec3& v, std::ostream* os);
void PrintTo(const Vec4& v, std::ostream* os);
void PrintTo(const Mat2& v, std::ostream* os);
void PrintTo(const Mat3& v, std::ostream* os);
void PrintTo(const Mat4& v, std::ostream* os);

} // namespace fuse
