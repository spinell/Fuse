#include "GTestUtils.h"
#include "math/Angle.h"

#include "gmock/gmock-matchers.h"
#include "gtest/gtest.h"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include <format>
#include <string>

using namespace fuse;
using namespace testing;

TEST(Angle, traits) {
    static_assert(std::is_trivially_copy_constructible_v<Angle>);
    static_assert(std::is_trivially_copy_assignable_v<Angle>);
    static_assert(std::is_trivially_move_constructible_v<Angle>);
    static_assert(std::is_trivially_move_assignable_v<Angle>);
    static_assert(std::is_trivially_copyable_v<Angle>);
}

TEST(Angle, operator_unitair) {
    // operator-
    EXPECT_EQ(-degrees(90.F), degrees(-90.F));

    // operator+
    EXPECT_EQ(+degrees(90.F), degrees(90.F));
}

TEST(Angle, operator_comparaison) {
    EXPECT_EQ(degrees(90.F), degrees(90.F)); // ==
    EXPECT_NE(degrees(90.F), degrees(91.F)); // !=
    EXPECT_LT(degrees(90.F), degrees(91.F)); // <
    EXPECT_LE(degrees(90.F), degrees(91.F)); // <=
    EXPECT_LE(degrees(91.F), degrees(91.F)); // <=
    EXPECT_GT(degrees(91.F), degrees(90.F)); // >
    EXPECT_GE(degrees(91.F), degrees(90.F)); // >=
    EXPECT_GE(degrees(91.F), degrees(91.F)); // >=
}

// Test addition operator
TEST(Angle, operator_add) {
    // operator+(Angle,Angle)
    EXPECT_EQ(degrees(90.F) + degrees(45.F), degrees(135.F));

    // operator+=(Angle)
    Angle        angle = degrees(90.F);
    const Angle& r     = angle += degrees(45.F);
    EXPECT_EQ(angle, degrees(135.F));
    EXPECT_EQ(r, degrees(135.F));
    EXPECT_EQ(&r, &angle);
}

// Test substact operator
TEST(Angle, operator_sub) {
    // operator-(Angle,Angle)
    EXPECT_EQ(degrees(90.F) + degrees(45.F), degrees(135.F));

    // operator-=(Angle)
    Angle        angle = degrees(90.F);
    const Angle& r     = angle -= degrees(45.F);
    EXPECT_EQ(angle, degrees(45.F));
    EXPECT_EQ(r, degrees(45.F));
    EXPECT_EQ(&r, &angle);
}

// Test multiplication operator
TEST(Angle, operator_mul) {
    // operator/(Angle, float)
    EXPECT_EQ(degrees(90.F) / 2, degrees(45.F));

    // operator/=(float)
    Angle        angle = degrees(90.F);
    const Angle& r     = angle /= 2;
    EXPECT_EQ(angle, degrees(45.F));
    EXPECT_EQ(r, degrees(45.F));
    EXPECT_EQ(&r, &angle);

    // operator/(Angle, Angle)
    EXPECT_EQ(degrees(90.F) / degrees(90.F), 1.F);
}

// Test division operator
TEST(Angle, operator_div) {
    // operator*(Angle, float)
    EXPECT_EQ(degrees(90.F) * 2, degrees(180.F));

    // operator*(float, Angle)
    EXPECT_EQ(2 * degrees(90.F), degrees(180.F));

    // operator*=(float)
    Angle        angle = degrees(45.F);
    const Angle& r     = angle *= 2;
    EXPECT_EQ(angle, degrees(90.F));
    EXPECT_EQ(r, degrees(90.F));
    EXPECT_EQ(&r, &angle);
}

TEST(Angle, wrap) {
    // test negative angle in step of 45 fuse::degrees
    EXPECT_THAT(fuse::degrees(-45.F).wrap(), Eq(fuse::degrees(-45.F)));
    EXPECT_THAT(fuse::degrees(-90.F).wrap(), Eq(fuse::degrees(-90.F)));
    EXPECT_THAT(fuse::degrees(-135.F).wrap(), Eq(fuse::degrees(-135.F)));
    EXPECT_THAT(fuse::degrees(-180.F).wrap(), Eq(fuse::degrees(-180.F)));
    EXPECT_THAT(fuse::degrees(-225.F).wrap(), Eq(fuse::degrees(-225.F)));
    EXPECT_THAT(fuse::degrees(-270.F).wrap(), Eq(fuse::degrees(-270.F)));
    EXPECT_THAT(fuse::degrees(-315.F).wrap(), Eq(fuse::degrees(-315.F)));
    EXPECT_THAT(fuse::degrees(-360.F).wrap(), Eq(fuse::degrees(0.F)));
    EXPECT_THAT(fuse::degrees(-405.F).wrap(), Eq(fuse::degrees(-45.F)));
    EXPECT_THAT(fuse::degrees(-450.F).wrap(), Eq(fuse::degrees(-90.F)));
    EXPECT_THAT(fuse::degrees(-495.F).wrap(), Eq(fuse::degrees(-135.F)));
    EXPECT_THAT(fuse::degrees(-540.F).wrap(), Eq(fuse::degrees(-180.F)));
    EXPECT_THAT(fuse::degrees(-585.F).wrap(), Eq(fuse::degrees(-225.F)));
    EXPECT_THAT(fuse::degrees(-630.F).wrap(), Eq(fuse::degrees(-270.F)));
    EXPECT_THAT(fuse::degrees(-675.F).wrap(), Eq(fuse::degrees(-315.F)));
    EXPECT_THAT(fuse::degrees(-720.F).wrap(), Eq(fuse::degrees(0.F)));

    // test positive angle in step of 45 fuse::degrees
    EXPECT_THAT(fuse::degrees(0.F).wrap(), Eq(fuse::degrees(0.F)));
    EXPECT_THAT(fuse::degrees(45.F).wrap(), Eq(fuse::degrees(45.F)));
    EXPECT_THAT(fuse::degrees(90.F).wrap(), Eq(fuse::degrees(90.F)));
    EXPECT_THAT(fuse::degrees(135.F).wrap(), Eq(fuse::degrees(135.F)));
    EXPECT_THAT(fuse::degrees(180.F).wrap(), Eq(fuse::degrees(180.F)));
    EXPECT_THAT(fuse::degrees(225.F).wrap(), Eq(fuse::degrees(225.F)));
    EXPECT_THAT(fuse::degrees(270.F).wrap(), Eq(fuse::degrees(270.F)));
    EXPECT_THAT(fuse::degrees(315.F).wrap(), Eq(fuse::degrees(315.F)));
    EXPECT_THAT(fuse::degrees(360.F).wrap(), Eq(fuse::degrees(0.F)));
    EXPECT_THAT(fuse::degrees(405.F).wrap(), Eq(fuse::degrees(45.F)));
    EXPECT_THAT(fuse::degrees(450.F).wrap(), Eq(fuse::degrees(90.F)));
    EXPECT_THAT(fuse::degrees(495.F).wrap(), Eq(fuse::degrees(135.F)));
    EXPECT_THAT(fuse::degrees(540.F).wrap(), Eq(fuse::degrees(180.F)));
    EXPECT_THAT(fuse::degrees(585.F).wrap(), Eq(fuse::degrees(225.F)));
    EXPECT_THAT(fuse::degrees(630.F).wrap(), Eq(fuse::degrees(270.F)));
    EXPECT_THAT(fuse::degrees(675.F).wrap(), Eq(fuse::degrees(315.F)));
    EXPECT_THAT(fuse::degrees(720.F).wrap(), Eq(fuse::degrees(0.F)));
}

TEST(Angle, wrapSigned) {
    EXPECT_EQ(degrees(-90.F).wrapSigned(), degrees(-90.F));
    EXPECT_EQ(degrees(-160.F).wrapSigned(), degrees(-160.F));
    EXPECT_EQ(degrees(-270.F).wrapSigned(), degrees(90.F));
    EXPECT_EQ(degrees(-360.F).wrapSigned(), degrees(0.F));
    EXPECT_EQ(degrees(-450.F).wrapSigned(), degrees(-90.F));
    EXPECT_EQ(degrees(-1000.F).wrapSigned(), degrees(80.F));

    EXPECT_EQ(degrees(0.f).wrapSigned(), degrees(0.F));

    EXPECT_EQ(degrees(90.F).wrapSigned(), degrees(90.F));
    EXPECT_EQ(degrees(180.F).wrapSigned(), degrees(180.F));
    EXPECT_EQ(degrees(270.F).wrapSigned(), degrees(-90.F));
    EXPECT_EQ(degrees(360.F).wrapSigned(), degrees(0.F));
    EXPECT_EQ(degrees(405.F).wrapSigned(), degrees(45.F));
    EXPECT_EQ(degrees(585.F).wrapSigned(), degrees(-135.F));
    EXPECT_EQ(degrees(700.F).wrapSigned(), degrees(-20.F));
    EXPECT_EQ(degrees(720.F).wrapSigned(), degrees(0.F));
    EXPECT_EQ(degrees(750.F).wrapSigned(), degrees(30.F));
}

TEST(Angle, wrapUnsigned) {
    EXPECT_EQ(degrees(-45.F).wrapUnsigned(), degrees(315.F));
    EXPECT_EQ(degrees(-90.F).wrapUnsigned(), degrees(270.F));
    EXPECT_EQ(degrees(-180.F).wrapUnsigned(), degrees(180.F));
    EXPECT_EQ(degrees(-270.F).wrapUnsigned(), degrees(90.F));
    EXPECT_EQ(degrees(-360.F).wrapUnsigned(), degrees(0.F));
    EXPECT_EQ(degrees(-750.F).wrapUnsigned(), degrees(330.F));

    EXPECT_EQ(degrees(0.f).wrapUnsigned(), degrees(0.F));

    EXPECT_EQ(degrees(45.F).wrapUnsigned(), degrees(45.F));
    EXPECT_EQ(degrees(90.F).wrapUnsigned(), degrees(90.F));
    EXPECT_EQ(degrees(180.F).wrapUnsigned(), degrees(180.F));
    EXPECT_EQ(degrees(270.F).wrapUnsigned(), degrees(270.F));
    EXPECT_EQ(degrees(360.F).wrapUnsigned(), degrees(0.F));
    EXPECT_EQ(degrees(405.F).wrapUnsigned(), degrees(45.F));
    EXPECT_EQ(degrees(585.F).wrapUnsigned(), degrees(225.F));
    EXPECT_EQ(degrees(720.F).wrapUnsigned(), degrees(0.F));
    EXPECT_EQ(degrees(750.F).wrapUnsigned(), degrees(30.F));
    EXPECT_EQ(degrees(2880.F).wrapUnsigned(), degrees(0.F));
    EXPECT_EQ(degrees(2885.F).wrapUnsigned(), degrees(5.F));
}

// Test literal operator
TEST(Angle, operator_literal) {
    using namespace literals;
    EXPECT_EQ(180_deg, degrees(180.F));   // unsigned long long
    EXPECT_EQ(180.0_deg, degrees(180.F)); // long double

    EXPECT_EQ(1_rad, radians(1));      // unsigned long long
    EXPECT_EQ(1.2_rad, radians(1.2F)); // long double
}

// test custom std::format
TEST(Angle, format) {
    const auto inDegree = std::format("{}", degrees(45.F));
    EXPECT_EQ("45", inDegree);

    const auto inDegree2 = std::format("{:.6f}", degrees(45.F));
    EXPECT_EQ("45.000000", inDegree2);
}
