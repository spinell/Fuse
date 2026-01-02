#pragma once
#include <ostream>

namespace fuse {

class Angle;
class Vec2;
class Vec3;
class Vec4;
class Mat2;
class Mat3;
class Mat4;

// teach gtest how to print data
void PrintTo(const Angle&, std::ostream* os);
void PrintTo(const Vec2&, std::ostream* os);
void PrintTo(const Vec3&, std::ostream* os);
void PrintTo(const Vec4&, std::ostream* os);
void PrintTo(const Mat2&, std::ostream* os);
void PrintTo(const Mat3&, std::ostream* os);
void PrintTo(const Mat4&, std::ostream* os);

} // namespace fuse
