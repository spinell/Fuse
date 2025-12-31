#include "GTestUtils.h"

#include <gtest/gtest.h>
#include <math/Vec2.h>

using fuse::Vec2;

TEST(Vec2, traits) {
    static_assert(std::is_trivially_copy_constructible_v<Vec2>);
    static_assert(std::is_trivially_copy_assignable_v<Vec2>);
    static_assert(std::is_trivially_move_constructible_v<Vec2>);
    static_assert(std::is_trivially_move_assignable_v<Vec2>);
    static_assert(std::is_trivially_copyable_v<Vec2>);
}

TEST(Vec2, constants) {
    static_assert(Vec2::kZero == Vec2(0.F, 0.F), "Vec2::kZero should be (0, 0, 0)");
    static_assert(Vec2::kUnitX == Vec2(1.F, 0.F), "Vec2::kUnitX should be (1, 0, 0)");
    static_assert(Vec2::kUnitY == Vec2(0.F, 1.F), "Vec2::kUnitY should be (0, 1, 0)");
    static_assert(Vec2::kUnitXNeg == Vec2(-1.F, 0.F), "Vec2::kUnitX should be (-1, 0, 0)");
    static_assert(Vec2::kUnitYNeg == Vec2(0.F, -1.F), "Vec2::kUnitY should be (0, -1, 0)");

    EXPECT_EQ(Vec2::kZero, Vec2(0.F, 0.F));
    EXPECT_EQ(Vec2::kUnitX, Vec2(1.F, 0.F));
    EXPECT_EQ(Vec2::kUnitY, Vec2(0.F, 1.F));
    EXPECT_EQ(Vec2::kUnitXNeg, Vec2(-1.F, 0.F));
    EXPECT_EQ(Vec2::kUnitYNeg, Vec2(0.F, -1.F));
}

// ==============================================
//               Constructors
// ==============================================

// Constructor with a single value
TEST(Vec2, ctor_same_value) {
    const auto v = Vec2(2.F);
    EXPECT_FLOAT_EQ(v.x, 2.F);
    EXPECT_FLOAT_EQ(v.y, 2.F);
}

// Constructor with parameters x,y,z
TEST(Vec2, ctor_with_components) {
    const auto v = Vec2(1.F, 2.F);
    EXPECT_FLOAT_EQ(v.x, 1.F);
    EXPECT_FLOAT_EQ(v.y, 2.F);
}

// ==============================================
//              unary operators
// ==============================================

TEST(Vec2, negate) {
    const auto negated = -Vec2(1.F, 2.F);
    EXPECT_FLOAT_EQ(negated.x, -1.F);
    EXPECT_FLOAT_EQ(negated.y, -2.F);
}

// ==============================================
//            comparaison operators
// ==============================================

TEST(Vec2, equals) {
    const auto v1 = Vec2(1.F, 2.F);
    const auto v2 = Vec2(1.F, 2.F);
    EXPECT_EQ(v1, v2);
}

TEST(Vec2, not_equals) {
    EXPECT_NE(Vec2(0.F, 2.F), Vec2(1.F, 2.F)); // x not equal
    EXPECT_NE(Vec2(1.F, 0.F), Vec2(1.F, 2.F)); // y not equal
}

// ==============================================
//                 Addition
// ==============================================

TEST(Vec2, add_two_vector) {
    // operator+(Vec2, Vec2)
    {
        const auto v1 = Vec2(1.F, 2.F);
        const auto v2 = Vec2(10.F, 20.F);
        const auto r  = v1 + v2;
        EXPECT_FLOAT_EQ(r.x, 11.F);
        EXPECT_FLOAT_EQ(r.y, 22.F);
    }
    // operator+=(vec4, vec4)
    {
        auto        v1 = Vec2(1.F, 2.F);
        const auto  v2 = Vec2(10.F, 20.F);
        const auto& r  = v1 += v2;

        // test the value of v1 after assignment
        EXPECT_FLOAT_EQ(v1.x, 11.F);
        EXPECT_FLOAT_EQ(v1.y, 22.F);

        // test the return value
        EXPECT_FLOAT_EQ(r.x, 11.F);
        EXPECT_FLOAT_EQ(r.y, 22.F);
    }
}

// ==============================================
//                Substraction
// ==============================================

TEST(Vec2, substract_two_vector) {
    // operator-(Vec2, Vec2)
    {
        const auto v1 = Vec2(1.F, 2.F);
        const auto v2 = Vec2(10.F, 20.F);
        const auto r  = v1 - v2;
        EXPECT_FLOAT_EQ(r.x, -9.F);
        EXPECT_FLOAT_EQ(r.y, -18.F);
    }

    // operator-=(Vec2, Vec2)
    {
        auto        v1 = Vec2(1.F, 2.F);
        const auto  v2 = Vec2(10.F, 20.F);
        const auto& r  = v1 -= v2;

        // test the value of v1 after assignment
        EXPECT_FLOAT_EQ(v1.x, -9.F);
        EXPECT_FLOAT_EQ(v1.y, -18.F);

        // test the return value
        EXPECT_FLOAT_EQ(r.x, -9.F);
        EXPECT_FLOAT_EQ(r.y, -18.F);
    }
}

// ==============================================
//                Multiply
// ==============================================

TEST(Vec2, multiple_by_scalar) {
    const auto v = Vec2(1.F, 2.F);

    // operator*(Vec2, float)
    {
        const auto r = v * 10;
        EXPECT_FLOAT_EQ(r.x, 10.F);
        EXPECT_FLOAT_EQ(r.y, 20.F);
    }

    // operator*(float, Vec2)
    {
        const auto r = 10 * v;
        EXPECT_FLOAT_EQ(r.x, 10.F);
        EXPECT_FLOAT_EQ(r.y, 20.F);
    }

    // operator*=(Vec2, float)
    {
        auto        v1 = v;
        const auto& r  = v1 *= 10.F;

        // test the value of v1 after assignment
        EXPECT_FLOAT_EQ(v1.x, 10.F);
        EXPECT_FLOAT_EQ(v1.y, 20.F);

        // test the return value
        EXPECT_FLOAT_EQ(r.x, 10.F);
        EXPECT_FLOAT_EQ(r.y, 20.F);
    }
}

// ==============================================
//                  Division
// ==============================================


TEST(Vec2, division_by_scalar) {
    // operator/(Vec2, scalar)
    const auto v = Vec2(10.F, 20.F);
    {
        const auto& r = v / 2.F;
        EXPECT_FLOAT_EQ(r.x, 5.F);
        EXPECT_FLOAT_EQ(r.y, 10.F);
    }

    // operator/=(Vec2, scalar)
    {
        auto        v1 = Vec2(10.F, 20.F);
        const auto& r  = v1 /= 2.F;

        EXPECT_FLOAT_EQ(v1.x, 5.F);
        EXPECT_FLOAT_EQ(v1.y, 10.F);

        EXPECT_FLOAT_EQ(r.x, 5.F);
        EXPECT_FLOAT_EQ(r.y, 10.F);
    }
}

// ==============================================
//            member functions
// ==============================================

TEST(Vec2, angleBetween) {
#if 0
    // parallel (same direction) vectors should have 0 degree angle
    EXPECT_EQ(Vec2::kUnitX.angleBetween(Vec2::kUnitX), 0_deg);
    EXPECT_EQ(Vec2::kUnitY.angleBetween(Vec2::kUnitY), 0_deg);
    EXPECT_EQ(Vec2::kUnitZ.angleBetween(Vec2::kUnitZ), 0_deg);
    EXPECT_EQ(Vec2::kUnitXNeg.angleBetween(Vec2::kUnitXNeg), 0_deg);
    EXPECT_EQ(Vec2::kUnitYNeg.angleBetween(Vec2::kUnitYNeg), 0_deg);
    EXPECT_EQ(Vec2::kUnitZNeg.angleBetween(Vec2::kUnitZNeg), 0_deg);
    //EXPECT_EQ(Vec2(1, 1, 0).angleBetween(Vec2(1, 1, 0)), 0._deg); // FIXME: fail weird value Angle(degrees=0.01978234, radians=0.00034526698)
    //EXPECT_EQ(Vec2(1, 1, 1).angleBetween(Vec2(1, 1, 1)), 0._deg); // FIXME: fail weird value Angle(degrees=0.01978234, radians=0.00034526698)

    // parallel (opposite direction) vectors should have 180 degree angle
    EXPECT_EQ(Vec2::kUnitX.angleBetween(Vec2::kUnitXNeg), 180_deg);
    EXPECT_EQ(Vec2::kUnitY.angleBetween(Vec2::kUnitYNeg), 180_deg);
    EXPECT_EQ(Vec2::kUnitZ.angleBetween(Vec2::kUnitZNeg), 180_deg);
    EXPECT_EQ(Vec2::kUnitXNeg.angleBetween(Vec2::kUnitX), 180_deg);
    EXPECT_EQ(Vec2::kUnitYNeg.angleBetween(Vec2::kUnitY), 180_deg);
    EXPECT_EQ(Vec2::kUnitZNeg.angleBetween(Vec2::kUnitZ), 180_deg);
    EXPECT_EQ(Vec2(1, 1, 1).angleBetween(Vec2(-1, -1, -1)), 179.98021_deg); // fail precision issue
    EXPECT_EQ(Vec2(-1, -1, -1).angleBetween(Vec2(1, 1, 1)), 179.98021_deg); // fail precision issue

    // Test with standard axis that have 90 degree angle
    EXPECT_EQ(Vec2::kUnitX.angleBetween(Vec2::kUnitY), 90_deg);    // x <->  y = 90 degree
    EXPECT_EQ(Vec2::kUnitY.angleBetween(Vec2::kUnitX), 90_deg);    // y <->  x = 90 degree
    EXPECT_EQ(Vec2::kUnitX.angleBetween(Vec2::kUnitZ), 90_deg);    // x <->  z = 90 degree
    EXPECT_EQ(Vec2::kUnitZ.angleBetween(Vec2::kUnitX), 90_deg);    // z <->  x = 90 degree
    EXPECT_EQ(Vec2::kUnitX.angleBetween(Vec2::kUnitYNeg), 90_deg); // x <-> -y = 90 degree
    EXPECT_EQ(Vec2::kUnitX.angleBetween(Vec2::kUnitZNeg), 90_deg); // x <-> -z = 90 degree
    EXPECT_EQ(Vec2::kUnitY.angleBetween(Vec2::kUnitZ), 90_deg);    // y <->  z = 90 degree
    EXPECT_EQ(Vec2::kUnitZ.angleBetween(Vec2::kUnitY), 90_deg);    // z <->  y = 90 degree
    EXPECT_EQ(Vec2::kUnitY.angleBetween(Vec2::kUnitXNeg), 90_deg); // y <-> -x = 90 degree
    EXPECT_EQ(Vec2::kUnitY.angleBetween(Vec2::kUnitZNeg), 90_deg); // y <-> -z = 90 degree
    EXPECT_EQ(Vec2::kUnitZ.angleBetween(Vec2::kUnitXNeg), 90_deg); // z <-> -x = 90 degree
    EXPECT_EQ(Vec2::kUnitZ.angleBetween(Vec2::kUnitYNeg), 90_deg); // z <-> -y = 90 degree

    // Test with custom unnormalized vector.
    {
        const Vec2 vector(2.0f, -2.0f, 0.0f);
        EXPECT_EQ(Vec2::kUnitX.angleBetween(vector), 45_deg);
        EXPECT_EQ(vector.angleBetween(Vec2::kUnitX), 45_deg);

        EXPECT_EQ(Vec2(1, 1, 0).angleBetween(vector), 90_deg);
        EXPECT_EQ(vector.angleBetween({1, 1, 0}), 90_deg);

        EXPECT_EQ(Vec2::kUnitY.angleBetween(vector), 135_deg);
        EXPECT_EQ(vector.angleBetween(Vec2::kUnitY), 135_deg);

        //EXPECT_EQ(vector.angleBetween({-1, 1, 0}), 180_deg);    // FIXME: fail precision issue  Angle(degrees=179.98021, radians=3.1412473)
        //EXPECT_EQ(Vec2(-1, 1, 0).angleBetween(vector), 180_deg);// FIXME: fail precision issue  Angle(degrees=179.98021, radians=3.1412473)

        EXPECT_EQ(Vec2::kUnitXNeg.angleBetween(vector), 135_deg);
        EXPECT_EQ(vector.angleBetween(Vec2::kUnitXNeg), 135_deg);

        EXPECT_EQ(Vec2(-1, -1, 0).angleBetween(vector), 90_deg);
        EXPECT_EQ(vector.angleBetween({1, 1, 0}), 90_deg);

        EXPECT_EQ(Vec2::kUnitYNeg.angleBetween(vector), 45_deg);
        EXPECT_EQ(vector.angleBetween(Vec2::kUnitYNeg), 45_deg);
    }
#endif
}

TEST(Vec2, dot) {
    EXPECT_EQ(Vec2::kUnitX.dot(Vec2::kUnitX), 1.0f);
    EXPECT_EQ(Vec2::kUnitY.dot(Vec2::kUnitY), 1.0f);

    EXPECT_EQ(Vec2::kUnitX.dot(-Vec2::kUnitX), -1.0f);
    EXPECT_EQ(Vec2::kUnitY.dot(-Vec2::kUnitY), -1.0f);

    EXPECT_EQ(Vec2::kUnitX.dot(Vec2::kUnitY), 0.0f);
    EXPECT_EQ(Vec2::kUnitX.dot(-Vec2::kUnitY), 0.0f);

    EXPECT_EQ(Vec2(1, 2).dot(Vec2(4, 5)), 14.0f);
}

TEST(Vec2, distance) {
    EXPECT_EQ(Vec2::kZero.distance(Vec2::kUnitX), 1.0F);
    EXPECT_EQ(Vec2::kZero.distance(Vec2::kUnitY), 1.0F);
    EXPECT_EQ(Vec2::kZero.distance(Vec2::kUnitXNeg), 1.0F);
    EXPECT_EQ(Vec2::kZero.distance(Vec2::kUnitYNeg), 1.0F);

    EXPECT_EQ(Vec2::kUnitX.distance(Vec2::kUnitX), 0.0F);
    EXPECT_EQ(Vec2::kUnitY.distance(Vec2::kUnitY), 0.0F);

    // -x <-> x
    EXPECT_EQ(Vec2::kUnitX.distance(Vec2::kUnitXNeg), 2.0F);
    EXPECT_EQ(Vec2::kUnitXNeg.distance(Vec2::kUnitX), 2.0F);

    // -y <-> y
    EXPECT_EQ(Vec2::kUnitY.distance(Vec2::kUnitYNeg), 2.0F);
    EXPECT_EQ(Vec2::kUnitYNeg.distance(Vec2::kUnitY), 2.0F);
}

TEST(Vec2, distanceSquared) {
    EXPECT_EQ(Vec2::kZero.distanceSquared(Vec2::kUnitX), 1.0F);
    EXPECT_EQ(Vec2::kZero.distanceSquared(Vec2::kUnitY), 1.0F);
    EXPECT_EQ(Vec2::kZero.distanceSquared(Vec2::kUnitXNeg), 1.0F);
    EXPECT_EQ(Vec2::kZero.distanceSquared(Vec2::kUnitYNeg), 1.0F);

    EXPECT_EQ(Vec2::kUnitX.distanceSquared(Vec2::kUnitX), 0.0F);
    EXPECT_EQ(Vec2::kUnitY.distanceSquared(Vec2::kUnitY), 0.0F);

    // -x <-> x
    EXPECT_EQ(Vec2::kUnitX.distanceSquared(Vec2::kUnitXNeg), 4.0F);
    EXPECT_EQ(Vec2::kUnitXNeg.distanceSquared(Vec2::kUnitX), 4.0F);

    // -y <-> y
    EXPECT_EQ(Vec2::kUnitY.distanceSquared(Vec2::kUnitYNeg), 4.0F);
    EXPECT_EQ(Vec2::kUnitYNeg.distanceSquared(Vec2::kUnitY), 4.0F);
}

TEST(Vec2, length) {
    EXPECT_FLOAT_EQ(Vec2::kZero.length(), 0.0F);

    EXPECT_FLOAT_EQ(Vec2(-2, 0).length(), 2.0F);
    EXPECT_FLOAT_EQ(Vec2(1, 0).length(), 1.0F);
    EXPECT_FLOAT_EQ(Vec2(2, 0).length(), 2.0F);

    EXPECT_FLOAT_EQ(Vec2(0, -2).length(), 2.0F);
    EXPECT_FLOAT_EQ(Vec2(0, 1).length(), 1.0F);
    EXPECT_FLOAT_EQ(Vec2(0, 2).length(), 2.0F);

    EXPECT_FLOAT_EQ(Vec2(3, 4).length(), 5.F);
    EXPECT_FLOAT_EQ(Vec2(-3, 4).length(), 5.F);
    EXPECT_FLOAT_EQ(Vec2(3, -4).length(), 5.F);
    EXPECT_FLOAT_EQ(Vec2(-3, -4).length(), 5.F);
}

TEST(Vec2, lengthSquared) {
    EXPECT_FLOAT_EQ(Vec2::kZero.lengthSquared(), 0.0F);

    EXPECT_FLOAT_EQ(Vec2(-2, 0).lengthSquared(), 4.0F);
    EXPECT_FLOAT_EQ(Vec2(1, 0).lengthSquared(), 1.0F);
    EXPECT_FLOAT_EQ(Vec2(2, 0).lengthSquared(), 4.0F);

    EXPECT_FLOAT_EQ(Vec2(0, -2).lengthSquared(), 4.0F);
    EXPECT_FLOAT_EQ(Vec2(0, 1).lengthSquared(), 1.0F);
    EXPECT_FLOAT_EQ(Vec2(0, 2).lengthSquared(), 4.0F);

    EXPECT_FLOAT_EQ(Vec2(3, 4).lengthSquared(), 25.F);
    EXPECT_FLOAT_EQ(Vec2(-3, 4).lengthSquared(), 25.F);
    EXPECT_FLOAT_EQ(Vec2(3, -4).lengthSquared(), 25.F);
    EXPECT_FLOAT_EQ(Vec2(-3, -4).lengthSquared(), 25.F);
}

TEST(Vec2, normalized) {
    EXPECT_EQ(Vec2(1, 0).normalize(), Vec2(1.F, 0.F));
    EXPECT_EQ(Vec2(2, 0).normalize(), Vec2(1.F, 0.F));
    EXPECT_EQ(Vec2(-2, 0).normalize(), Vec2(-1.F, 0.F));

    EXPECT_EQ(Vec2(0, 1).normalize(), Vec2(0.F, 1.F));
    EXPECT_EQ(Vec2(0, 2).normalize(), Vec2(0.F, 1.F));
    EXPECT_EQ(Vec2(0, -2).normalize(), Vec2(0.F, -1.F));
}

TEST(Vec2, projectTo) {
    {
        const Vec2 v(3, 4);
        EXPECT_EQ(v.projectTo(Vec2::kUnitX), Vec2(3, 0));    // projection onto  x axis
        EXPECT_EQ(v.projectTo(Vec2::kUnitXNeg), Vec2(3, 0)); // projection onto -x axis
        EXPECT_EQ(v.projectTo(Vec2::kUnitY), Vec2(0, 4));    // projection onto  y axis
        EXPECT_EQ(v.projectTo(Vec2::kUnitYNeg), Vec2(0, 4)); // projection onto -y axis
    }

    {
        const Vec2 v(-3, -4);
        EXPECT_EQ(v.projectTo(Vec2::kUnitX), Vec2(-3, 0));    // projection onto  x axis
        EXPECT_EQ(v.projectTo(Vec2::kUnitXNeg), Vec2(-3, 0)); // projection onto -x axis
        EXPECT_EQ(v.projectTo(Vec2::kUnitY), Vec2(0, -4));    // projection onto  y axis
        EXPECT_EQ(v.projectTo(Vec2::kUnitYNeg), Vec2(0, -4)); // projection onto -y axis
    }
}

TEST(Vec2, perpendicularOnto) {
    {
        const Vec2 v(3, 4);
        EXPECT_EQ(v.perpendicularTo(Vec2::kUnitX), Vec2(0, 4));
        EXPECT_EQ(v.perpendicularTo(Vec2::kUnitY), Vec2(3, 0));
        EXPECT_EQ(v.perpendicularTo(Vec2::kUnitXNeg), Vec2(0, 4));
        EXPECT_EQ(v.perpendicularTo(Vec2::kUnitYNeg), Vec2(3, 0));
    }

    {
        const Vec2 v(-3, -4);
        EXPECT_EQ(v.perpendicularTo(Vec2::kUnitX), Vec2(0, -4));
        EXPECT_EQ(v.perpendicularTo(Vec2::kUnitY), Vec2(-3, 0));
        EXPECT_EQ(v.perpendicularTo(Vec2::kUnitXNeg), Vec2(0, -4));
        EXPECT_EQ(v.perpendicularTo(Vec2::kUnitYNeg), Vec2(-3, 0));
    }

    EXPECT_EQ(Vec2(1, 1).perpendicularTo(Vec2::kUnitX), Vec2(0, 1));
    EXPECT_EQ(Vec2(1, 2).perpendicularTo(Vec2(1, 1)), Vec2(-0.5, 0.5));
}

TEST(Vec2, ptr) {
#if 0
    // const version
    const auto  v   = Vec2(3.F, 4.F, 0.F);
    const auto* ptr = v.ptr();
    EXPECT_FLOAT_EQ(ptr[0], 3.F);
    EXPECT_FLOAT_EQ(ptr[1], 4.F);
    EXPECT_FLOAT_EQ(ptr[2], 0.F);

    // non const version
    auto        v2   = Vec2(3.F, 4.F, 0.F);
    const auto* ptr2 = v2.ptr();
    EXPECT_FLOAT_EQ(ptr2[0], 3.F);
    EXPECT_FLOAT_EQ(ptr2[1], 4.F);
    EXPECT_FLOAT_EQ(ptr2[2], 0.F);
#endif
}
