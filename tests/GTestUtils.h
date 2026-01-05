#pragma once
#include <gtest/gtest.h>

#include <ostream>

namespace fuse {

class Angle;
class Vec2;
class Vec3;
class Vec4;
class Mat2;
class Mat3;
class Mat4;
class Quaternion;

// teach gtest how to print data
// they need be be in namespace fuse otherwise GTest will not find them.
// required for argument-dependent lookup (ADL)
void PrintTo(const Angle&, std::ostream* os);
void PrintTo(const Vec2&, std::ostream* os);
void PrintTo(const Vec3&, std::ostream* os);
void PrintTo(const Vec4&, std::ostream* os);
void PrintTo(const Mat2&, std::ostream* os);
void PrintTo(const Mat3&, std::ostream* os);
void PrintTo(const Mat4&, std::ostream* os);
void PrintTo(const Quaternion&, std::ostream* os);

} // namespace fuse

//
// Custom Assert to test Angle
//
::testing::AssertionResult AssertAngleNear(const char*        expr1,
                                           const char*        expr2,
                                           const fuse::Angle& expected,
                                           const fuse::Angle& actual);

::testing::AssertionResult AssertVec2Near(const char*        expr1,
                                          const char*        expr2,
                                          const fuse::Vec2& expected,
                                          const fuse::Vec2& actual);




::testing::AssertionResult AssertVec3Near(const char*        expr1,
                                          const char*        expr2,
                                          const fuse::Vec3& expected,
                                          const fuse::Vec3& actual);

::testing::AssertionResult AssertQuaternionNear(
  const char*             expr1,
  const char*             expr2,
  const fuse::Quaternion& expected,
  const fuse::Quaternion& actual,
  float                   tolerance = std::numeric_limits<float>::epsilon());


#define EXPECT_ANGLE_NEAR(expected, actual) EXPECT_PRED_FORMAT2(AssertAngleNear, expected, actual)
#define ASSERT_ANGLE_NEAR(expected, actual) ASSERT_PRED_FORMAT2(AssertAngleNear, expected, actual)

#define EXPECT_VEC2_NEAR(expected, actual) EXPECT_PRED_FORMAT2(AssertVec2Near, expected, actual)
#define ASSERT_VEC2_NEAR(expected, actual) ASSERT_PRED_FORMAT2(AssertVec2Near, expected, actual)
#define EXPECT_VEC3_NEAR(expected, actual) EXPECT_PRED_FORMAT2(AssertVec3Near, expected, actual)
#define ASSERT_VEC3_NEAR(expected, actual) ASSERT_PRED_FORMAT2(AssertVec3Near, expected, actual)

#define EXPECT_QUAT_NEAR(expected, actual) EXPECT_PRED_FORMAT2(AssertQuaternionNear, expected, actual)
#define ASSERT_QUAT_NEAR(expected, actual) ASSERT_PRED_FORMAT2(AssertQuaternionNear, expected, actual)
