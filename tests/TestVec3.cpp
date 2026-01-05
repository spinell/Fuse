#include "GTestUtils.h"

#include <math/Vec3.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace fuse;

TEST(Vec3, traits) {
    static_assert(std::is_trivially_copy_constructible_v<Vec3>);
    static_assert(std::is_trivially_copy_assignable_v<Vec3>);
    static_assert(std::is_trivially_move_constructible_v<Vec3>);
    static_assert(std::is_trivially_move_assignable_v<Vec3>);
    static_assert(std::is_trivially_copyable_v<Vec3>);
}

TEST(Vec3, constants) {
    static_assert(Vec3::kZero == Vec3(0.F, 0.F, 0.F), "Vec3::kZero should be (0, 0, 0)");
    static_assert(Vec3::kUnitX == Vec3(1.F, 0.F, 0.F), "Vec3::kUnitX should be (1, 0, 0)");
    static_assert(Vec3::kUnitY == Vec3(0.F, 1.F, 0.F), "Vec3::kUnitY should be (0, 1, 0)");
    static_assert(Vec3::kUnitZ == Vec3(0.F, 0.F, 1.F), "Vec3::kUnitZ should be (0, 0, 1)");
    static_assert(Vec3::kUnitXNeg == Vec3(-1.F, 0.F, 0.F), "Vec3::kUnitX should be (-1, 0, 0)");
    static_assert(Vec3::kUnitYNeg == Vec3(0.F, -1.F, 0.F), "Vec3::kUnitY should be (0, -1, 0)");
    static_assert(Vec3::kUnitZNeg == Vec3(0.F, 0.F, -1.F), "Vec3::kUnitZ should be (0, 0, -1)");

    EXPECT_EQ(Vec3::kZero, Vec3(0.F, 0.F, 0.F));
    EXPECT_EQ(Vec3::kUnitX, Vec3(1.F, 0.F, 0.F));
    EXPECT_EQ(Vec3::kUnitY, Vec3(0.F, 1.F, 0.F));
    EXPECT_EQ(Vec3::kUnitZ, Vec3(0.F, 0.F, 1.F));
    EXPECT_EQ(Vec3::kUnitXNeg, Vec3(-1.F, 0.F, 0.F));
    EXPECT_EQ(Vec3::kUnitYNeg, Vec3(0.F, -1.F, 0.F));
    EXPECT_EQ(Vec3::kUnitZNeg, Vec3(0.F, 0.F, -1.F));
}

// ==============================================
//               Constructors
// ==============================================

// Constructor with a single value
TEST(Vec3, ctor_same_value) {
    const auto v = Vec3(2.F);
    EXPECT_FLOAT_EQ(v.x, 2.F);
    EXPECT_FLOAT_EQ(v.y, 2.F);
    EXPECT_FLOAT_EQ(v.z, 2.F);
}

// Constructor with parameters x,y,z
TEST(Vec3, ctor_with_components) {
    const auto v = Vec3(1.F, 2.F, 3.F);
    EXPECT_FLOAT_EQ(v.x, 1.F);
    EXPECT_FLOAT_EQ(v.y, 2.F);
    EXPECT_FLOAT_EQ(v.z, 3.F);
}

// ==============================================
//              unary operators
// ==============================================

TEST(Vec3, negate) {
    const auto negated = -Vec3(1.F, 2.F, 3.F);
    EXPECT_FLOAT_EQ(negated.x, -1.F);
    EXPECT_FLOAT_EQ(negated.y, -2.F);
    EXPECT_FLOAT_EQ(negated.z, -3.F);
}

// ==============================================
//            comparaison operators
// ==============================================

TEST(Vec3, equals) {
    const auto v1 = Vec3(1.F, 2.F, 3.F);
    const auto v2 = Vec3(1.F, 2.F, 3.F);
    EXPECT_EQ(v1, v2);
}

TEST(Vec3, not_equals) {
    EXPECT_NE(Vec3(0.F, 2.F, 3.F), Vec3(1.F, 2.F, 3.F)); // x not equal
    EXPECT_NE(Vec3(1.F, 0.F, 3.F), Vec3(1.F, 2.F, 3.F)); // y not equal
    EXPECT_NE(Vec3(1.F, 2.F, 0.F), Vec3(1.F, 2.F, 3.F)); // z not equal
}

// ==============================================
//                 Addition
// ==============================================

TEST(Vec3, add_two_vector) {
    // operator+(Vec3, Vec3)
    {
        const auto v1 = Vec3(1.F, 2.F, 3.F);
        const auto v2 = Vec3(10.F, 20.F, 30.F);
        const auto r  = v1 + v2;
        EXPECT_FLOAT_EQ(r.x, 11.F);
        EXPECT_FLOAT_EQ(r.y, 22.F);
        EXPECT_FLOAT_EQ(r.z, 33.F);
    }
    // operator+=(vec4, vec4)
    {
        auto        v1 = Vec3(1.F, 2.F, 3.F);
        const auto  v2 = Vec3(10.F, 20.F, 30.F);
        const auto& r  = v1 += v2;

        // test the value of v1 after assignment
        EXPECT_FLOAT_EQ(v1.x, 11.F);
        EXPECT_FLOAT_EQ(v1.y, 22.F);
        EXPECT_FLOAT_EQ(v1.z, 33.F);

        // test the return value
        EXPECT_FLOAT_EQ(r.x, 11.F);
        EXPECT_FLOAT_EQ(r.y, 22.F);
        EXPECT_FLOAT_EQ(r.z, 33.F);
    }
}

// ==============================================
//                Substraction
// ==============================================

TEST(Vec3, substract_two_vector) {
    // operator-(Vec3, Vec3)
    {
        const auto v1 = Vec3(1.F, 2.F, 3.F);
        const auto v2 = Vec3(10.F, 20.F, 30.F);
        const auto r  = v1 - v2;
        EXPECT_FLOAT_EQ(r.x, -9.F);
        EXPECT_FLOAT_EQ(r.y, -18.F);
        EXPECT_FLOAT_EQ(r.z, -27.F);
    }

    // operator-=(Vec3, Vec3)
    {
        auto        v1 = Vec3(1.F, 2.F, 3.F);
        const auto  v2 = Vec3(10.F, 20.F, 30.F);
        const auto& r  = v1 -= v2;

        // test the value of v1 after assignment
        EXPECT_FLOAT_EQ(v1.x, -9.F);
        EXPECT_FLOAT_EQ(v1.y, -18.F);
        EXPECT_FLOAT_EQ(v1.z, -27.F);

        // test the return value
        EXPECT_FLOAT_EQ(r.x, -9.F);
        EXPECT_FLOAT_EQ(r.y, -18.F);
        EXPECT_FLOAT_EQ(r.z, -27.F);
    }
}

// ==============================================
//                Multiply
// ==============================================

TEST(Vec3, multiple_by_scalar) {
    const auto v = Vec3(1.F, 2.F, 3.F);

    // operator*(Vec3, float)
    {
        const auto r = v * 10;
        EXPECT_FLOAT_EQ(r.x, 10.F);
        EXPECT_FLOAT_EQ(r.y, 20.F);
        EXPECT_FLOAT_EQ(r.z, 30.F);
    }

    // operator*(float, Vec3)
    {
        const auto r = 10 * v;
        EXPECT_FLOAT_EQ(r.x, 10.F);
        EXPECT_FLOAT_EQ(r.y, 20.F);
        EXPECT_FLOAT_EQ(r.z, 30.F);
    }

    // operator*=(Vec3, float)
    {
        auto        v1 = v;
        const auto& r  = v1 *= 10.F;

        // test the value of v1 after assignment
        EXPECT_FLOAT_EQ(v1.x, 10.F);
        EXPECT_FLOAT_EQ(v1.y, 20.F);
        EXPECT_FLOAT_EQ(v1.z, 30.F);

        // test the return value
        EXPECT_FLOAT_EQ(r.x, 10.F);
        EXPECT_FLOAT_EQ(r.y, 20.F);
        EXPECT_FLOAT_EQ(r.z, 30.F);
    }
}

// ==============================================
//                  Division
// ==============================================


TEST(Vec3, division_by_scalar) {
    const auto v = Vec3(10.F, 20.F, 30.F);
    // operator/(Vec3, scalar)
    {
        const auto& r = v / 2.F;
        EXPECT_FLOAT_EQ(r.x, 5.F);
        EXPECT_FLOAT_EQ(r.y, 10.F);
        EXPECT_FLOAT_EQ(r.z, 15.F);
    }

    // operator/=(Vec3, scalar)
    {
        auto        v1 = Vec3(10.F, 20.F, 30.F);
        const auto& r  = v1 /= 2.F;

        EXPECT_FLOAT_EQ(v1.x, 5.F);
        EXPECT_FLOAT_EQ(v1.y, 10.F);
        EXPECT_FLOAT_EQ(v1.z, 15.F);

        EXPECT_FLOAT_EQ(r.x, 5.F);
        EXPECT_FLOAT_EQ(r.y, 10.F);
        EXPECT_FLOAT_EQ(r.z, 15.F);
    }
}

// ==============================================
//            member functions
// ==============================================

TEST(Vec3, angleBetween) {
    using namespace fuse::literals;

    // parallel (same direction) vectors should have 0 degree angle
    EXPECT_EQ(Vec3::kUnitX.angleBetween(Vec3::kUnitX), 0_deg);
    EXPECT_EQ(Vec3::kUnitY.angleBetween(Vec3::kUnitY), 0_deg);
    EXPECT_EQ(Vec3::kUnitZ.angleBetween(Vec3::kUnitZ), 0_deg);
    EXPECT_EQ(Vec3::kUnitXNeg.angleBetween(Vec3::kUnitXNeg), 0_deg);
    EXPECT_EQ(Vec3::kUnitYNeg.angleBetween(Vec3::kUnitYNeg), 0_deg);
    EXPECT_EQ(Vec3::kUnitZNeg.angleBetween(Vec3::kUnitZNeg), 0_deg);
    EXPECT_EQ(Vec3(1, 1, 0).angleBetween(Vec3(1, 1, 0)), 0._deg);
    EXPECT_EQ(Vec3(1, 1, 1).angleBetween(Vec3(1, 1, 1)), 0._deg);

    // parallel (opposite direction) vectors should have 180 degree angle
    EXPECT_EQ(Vec3::kUnitX.angleBetween(Vec3::kUnitXNeg), 180_deg);
    EXPECT_EQ(Vec3::kUnitY.angleBetween(Vec3::kUnitYNeg), 180_deg);
    EXPECT_EQ(Vec3::kUnitZ.angleBetween(Vec3::kUnitZNeg), 180_deg);
    EXPECT_EQ(Vec3::kUnitXNeg.angleBetween(Vec3::kUnitX), 180_deg);
    EXPECT_EQ(Vec3::kUnitYNeg.angleBetween(Vec3::kUnitY), 180_deg);
    EXPECT_EQ(Vec3::kUnitZNeg.angleBetween(Vec3::kUnitZ), 180_deg);
    EXPECT_EQ(Vec3(1, 1, 1).angleBetween(Vec3(-1, -1, -1)), 180_deg);
    EXPECT_EQ(Vec3(-1, -1, -1).angleBetween(Vec3(1, 1, 1)), 180_deg);

    // Test with standard axis that have 90 degree angle
    EXPECT_EQ(Vec3::kUnitX.angleBetween(Vec3::kUnitY), 90_deg);    // x <->  y = 90 degree
    EXPECT_EQ(Vec3::kUnitY.angleBetween(Vec3::kUnitX), 90_deg);    // y <->  x = 90 degree
    EXPECT_EQ(Vec3::kUnitX.angleBetween(Vec3::kUnitZ), 90_deg);    // x <->  z = 90 degree
    EXPECT_EQ(Vec3::kUnitZ.angleBetween(Vec3::kUnitX), 90_deg);    // z <->  x = 90 degree
    EXPECT_EQ(Vec3::kUnitX.angleBetween(Vec3::kUnitYNeg), 90_deg); // x <-> -y = 90 degree
    EXPECT_EQ(Vec3::kUnitX.angleBetween(Vec3::kUnitZNeg), 90_deg); // x <-> -z = 90 degree
    EXPECT_EQ(Vec3::kUnitY.angleBetween(Vec3::kUnitZ), 90_deg);    // y <->  z = 90 degree
    EXPECT_EQ(Vec3::kUnitZ.angleBetween(Vec3::kUnitY), 90_deg);    // z <->  y = 90 degree
    EXPECT_EQ(Vec3::kUnitY.angleBetween(Vec3::kUnitXNeg), 90_deg); // y <-> -x = 90 degree
    EXPECT_EQ(Vec3::kUnitY.angleBetween(Vec3::kUnitZNeg), 90_deg); // y <-> -z = 90 degree
    EXPECT_EQ(Vec3::kUnitZ.angleBetween(Vec3::kUnitXNeg), 90_deg); // z <-> -x = 90 degree
    EXPECT_EQ(Vec3::kUnitZ.angleBetween(Vec3::kUnitYNeg), 90_deg); // z <-> -y = 90 degree

    // Test with custom unnormalized vector.
    {
        const Vec3 vector(2.0f, 2.0f, 0.0f);
        EXPECT_EQ(vector.angleBetween({1, 1, 0}), 0_deg);
        EXPECT_EQ(vector.angleBetween(Vec3::kUnitX * 2), 45_deg);
        EXPECT_EQ(vector.angleBetween(Vec3::kUnitY * 2), 45_deg);

        EXPECT_EQ(vector.angleBetween({-2.0f, 2.0f, 0.0f}), 90_deg);
        EXPECT_EQ(vector.angleBetween(Vec3::kUnitXNeg * 2), 135_deg);
        EXPECT_EQ(vector.angleBetween({-1, -1, 0}), 180_deg);
        EXPECT_EQ(vector.angleBetween(Vec3::kUnitYNeg * 2), 135_deg);
        EXPECT_EQ(vector.angleBetween(Vec3(2.0f, -2.0f, 0.0f)), 90_deg);
    }
}

TEST(Vec3, cross) {
    // cross product of 2 parallel vectors is zero.
    EXPECT_EQ(Vec3::kUnitX.cross(Vec3::kUnitX), Vec3::kZero);
    EXPECT_EQ(Vec3::kUnitX.cross(Vec3::kUnitXNeg), Vec3::kZero);

    EXPECT_EQ(Vec3::kUnitX.cross(Vec3::kUnitY), Vec3::kUnitZ);       // x cross  y =  z
    EXPECT_EQ(Vec3::kUnitX.cross(Vec3::kUnitZ), Vec3::kUnitYNeg);    // x cross  z = -y
    EXPECT_EQ(Vec3::kUnitX.cross(Vec3::kUnitYNeg), Vec3::kUnitZNeg); // x cross -y = -z
    EXPECT_EQ(Vec3::kUnitX.cross(Vec3::kUnitZNeg), Vec3::kUnitY);    // x cross -z =  y

    EXPECT_EQ(Vec3::kUnitY.cross(Vec3::kUnitX), Vec3::kUnitZNeg);    // y cross  x = -z
    EXPECT_EQ(Vec3::kUnitY.cross(Vec3::kUnitZ), Vec3::kUnitX);       // y cross  z =  x
    EXPECT_EQ(Vec3::kUnitY.cross(Vec3::kUnitXNeg), Vec3::kUnitZ);    // y cross -x =  z
    EXPECT_EQ(Vec3::kUnitY.cross(Vec3::kUnitZNeg), Vec3::kUnitXNeg); // y cross -z = -x

    EXPECT_EQ(Vec3::kUnitZ.cross(Vec3::kUnitX), Vec3::kUnitY);       // z cross  x =  y
    EXPECT_EQ(Vec3::kUnitZ.cross(Vec3::kUnitY), Vec3::kUnitXNeg);    // z cross  y = -x
    EXPECT_EQ(Vec3::kUnitZ.cross(Vec3::kUnitXNeg), Vec3::kUnitYNeg); // z cross -x = -y
    EXPECT_EQ(Vec3::kUnitZ.cross(Vec3::kUnitYNeg), Vec3::kUnitX);    // z cross -y =  x
}

TEST(Vec3, dot) {
    EXPECT_EQ(Vec3::kUnitX.dot(Vec3::kUnitX), 1.0f);
    EXPECT_EQ(Vec3::kUnitY.dot(Vec3::kUnitY), 1.0f);
    EXPECT_EQ(Vec3::kUnitZ.dot(Vec3::kUnitZ), 1.0f);

    EXPECT_EQ(Vec3::kUnitX.dot(-Vec3::kUnitX), -1.0f);
    EXPECT_EQ(Vec3::kUnitY.dot(-Vec3::kUnitY), -1.0f);
    EXPECT_EQ(Vec3::kUnitZ.dot(-Vec3::kUnitZ), -1.0f);

    EXPECT_EQ(Vec3::kUnitX.dot(Vec3::kUnitY), 0.0f);
    EXPECT_EQ(Vec3::kUnitX.dot(Vec3::kUnitZ), 0.0f);
    EXPECT_EQ(Vec3::kUnitX.dot(-Vec3::kUnitY), 0.0f);
    EXPECT_EQ(Vec3::kUnitX.dot(-Vec3::kUnitZ), 0.0f);

    // 1*4 + 2*5 + 3*6 = 4 + 10 + 18 = 32
    EXPECT_EQ(Vec3(1, 2, 3).dot(Vec3(4, 5, 6)), 32.0f);
}

TEST(Vec3, distance) {
    EXPECT_EQ(Vec3::kZero.distance(Vec3::kUnitX), 1.0F);
    EXPECT_EQ(Vec3::kZero.distance(Vec3::kUnitY), 1.0F);
    EXPECT_EQ(Vec3::kZero.distance(Vec3::kUnitZ), 1.0F);
    EXPECT_EQ(Vec3::kZero.distance(Vec3::kUnitXNeg), 1.0F);
    EXPECT_EQ(Vec3::kZero.distance(Vec3::kUnitYNeg), 1.0F);
    EXPECT_EQ(Vec3::kZero.distance(Vec3::kUnitZNeg), 1.0F);

    EXPECT_EQ(Vec3::kUnitX.distance(Vec3::kUnitX), 0.0F);
    EXPECT_EQ(Vec3::kUnitY.distance(Vec3::kUnitY), 0.0F);
    EXPECT_EQ(Vec3::kUnitZ.distance(Vec3::kUnitZ), 0.0F);

    // -x <-> x
    EXPECT_EQ(Vec3::kUnitX.distance(Vec3::kUnitXNeg), 2.0F);
    EXPECT_EQ(Vec3::kUnitXNeg.distance(Vec3::kUnitX), 2.0F);

    // -y <-> y
    EXPECT_EQ(Vec3::kUnitY.distance(Vec3::kUnitYNeg), 2.0F);
    EXPECT_EQ(Vec3::kUnitYNeg.distance(Vec3::kUnitY), 2.0F);

    // -z <-> z
    EXPECT_EQ(Vec3::kUnitZ.distance(Vec3::kUnitZNeg), 2.0F);
    EXPECT_EQ(Vec3::kUnitZNeg.distance(Vec3::kUnitZ), 2.0F);
}

TEST(Vec3, distanceSquared) {
    EXPECT_EQ(Vec3::kZero.distanceSquared(Vec3::kUnitX), 1.0F);
    EXPECT_EQ(Vec3::kZero.distanceSquared(Vec3::kUnitY), 1.0F);
    EXPECT_EQ(Vec3::kZero.distanceSquared(Vec3::kUnitZ), 1.0F);
    EXPECT_EQ(Vec3::kZero.distanceSquared(Vec3::kUnitXNeg), 1.0F);
    EXPECT_EQ(Vec3::kZero.distanceSquared(Vec3::kUnitYNeg), 1.0F);
    EXPECT_EQ(Vec3::kZero.distanceSquared(Vec3::kUnitZNeg), 1.0F);

    EXPECT_EQ(Vec3::kUnitX.distanceSquared(Vec3::kUnitX), 0.0F);
    EXPECT_EQ(Vec3::kUnitY.distanceSquared(Vec3::kUnitY), 0.0F);
    EXPECT_EQ(Vec3::kUnitZ.distanceSquared(Vec3::kUnitZ), 0.0F);

    // -x <-> x
    EXPECT_EQ(Vec3::kUnitX.distanceSquared(Vec3::kUnitXNeg), 4.0F);
    EXPECT_EQ(Vec3::kUnitXNeg.distanceSquared(Vec3::kUnitX), 4.0F);

    // -y <-> y
    EXPECT_EQ(Vec3::kUnitY.distanceSquared(Vec3::kUnitYNeg), 4.0F);
    EXPECT_EQ(Vec3::kUnitYNeg.distanceSquared(Vec3::kUnitY), 4.0F);

    // -z <-> z
    EXPECT_EQ(Vec3::kUnitZ.distanceSquared(Vec3::kUnitZNeg), 4.0F);
    EXPECT_EQ(Vec3::kUnitZNeg.distanceSquared(Vec3::kUnitZ), 4.0F);
}

TEST(Vec3, length) {
    EXPECT_FLOAT_EQ(Vec3::kZero.length(), 0.0F);

    EXPECT_FLOAT_EQ(Vec3(1, 0, 0).length(), 1.0F);
    EXPECT_FLOAT_EQ(Vec3(0, 1, 0).length(), 1.0F);
    EXPECT_FLOAT_EQ(Vec3(0, 0, 1).length(), 1.0F);

    EXPECT_FLOAT_EQ(Vec3(-1, 0, 0).length(), 1.0F);
    EXPECT_FLOAT_EQ(Vec3(0, -1, 0).length(), 1.0F);
    EXPECT_FLOAT_EQ(Vec3(0, 0, -1).length(), 1.0F);

    EXPECT_FLOAT_EQ(Vec3(3, 4, 0).length(), 5.F);
    EXPECT_FLOAT_EQ(Vec3(3, 0, 4).length(), 5.F);
    EXPECT_FLOAT_EQ(Vec3(0, 3, 4).length(), 5.F);

    EXPECT_FLOAT_EQ(Vec3(3, 4, 5).length(), 7.0710678F);
    EXPECT_FLOAT_EQ(Vec3(-3, -4, -5).length(), 7.0710678F);
}

TEST(Vec3, lengthSquared) {
    EXPECT_FLOAT_EQ(Vec3::kZero.lengthSquared(), 0.0F);

    EXPECT_FLOAT_EQ(Vec3(1, 0, 0).lengthSquared(), 1.0F);
    EXPECT_FLOAT_EQ(Vec3(0, 1, 0).lengthSquared(), 1.0F);
    EXPECT_FLOAT_EQ(Vec3(0, 0, 1).lengthSquared(), 1.0F);

    EXPECT_FLOAT_EQ(Vec3(-1, 0, 0).lengthSquared(), 1.0F);
    EXPECT_FLOAT_EQ(Vec3(0, -1, 0).lengthSquared(), 1.0F);
    EXPECT_FLOAT_EQ(Vec3(0, 0, -1).lengthSquared(), 1.0F);

    EXPECT_FLOAT_EQ(Vec3(3, 4, 0).lengthSquared(), 25.F);
    EXPECT_FLOAT_EQ(Vec3(3, 0, 4).lengthSquared(), 25.F);
    EXPECT_FLOAT_EQ(Vec3(0, 3, 4).lengthSquared(), 25.F);

    EXPECT_FLOAT_EQ(Vec3(3, 4, 5).lengthSquared(), 50.0F);
    EXPECT_FLOAT_EQ(Vec3(-3, -4, -5).lengthSquared(), 50.0F);
}

TEST(Vec3, normalize) {
    EXPECT_EQ(Vec3(1, 0, 0).normalize(), Vec3(1.F, 0.F, 0.F));
    EXPECT_EQ(Vec3(0, 1, 0).normalize(), Vec3(0.F, 1.F, 0.F));
    EXPECT_EQ(Vec3(0, 0, 1).normalize(), Vec3(0.F, 0.F, 1.F));

    EXPECT_EQ(Vec3(2, 0, 0).normalize(), Vec3(1.F, 0.F, 0.F));
    EXPECT_EQ(Vec3(0, 2, 0).normalize(), Vec3(0.F, 1.F, 0.F));
    EXPECT_EQ(Vec3(0, 0, 2).normalize(), Vec3(0.F, 0.F, 1.F));

    EXPECT_EQ(Vec3(-2, 0, 0).normalize(), Vec3(-1.F, 0.F, 0.F));
    EXPECT_EQ(Vec3(0, -2, 0).normalize(), Vec3(0.F, -1.F, 0.F));
    EXPECT_EQ(Vec3(0, 0, -2).normalize(), Vec3(0.F, 0.F, -1.F));

    EXPECT_EQ(Vec3(1, 2, 3).normalize(), Vec3(0.26726124F, 0.53452247F, 0.80178368F));
    EXPECT_EQ(Vec3(-1, -2, -3).normalize(), Vec3(-0.26726124F, -0.53452247F, -0.80178368F));
}

TEST(Vec3, projectTo) {
    {
        const Vec3 v(3, 4, 5);
        EXPECT_EQ(v.projectTo(Vec3::kUnitX), Vec3(3, 0, 0));    // projection onto  x axis
        EXPECT_EQ(v.projectTo(Vec3::kUnitXNeg), Vec3(3, 0, 0)); // projection onto -x axis
        EXPECT_EQ(v.projectTo(Vec3::kUnitY), Vec3(0, 4, 0));    // projection onto  y axis
        EXPECT_EQ(v.projectTo(Vec3::kUnitYNeg), Vec3(0, 4, 0)); // projection onto -y axis
        EXPECT_EQ(v.projectTo(Vec3::kUnitZ), Vec3(0, 0, 5));    // projection onto  z axis
        EXPECT_EQ(v.projectTo(Vec3::kUnitZNeg), Vec3(0, 0, 5)); // projection onto -z axis
    }

    {
        const Vec3 v(-3, -4, -5);
        EXPECT_EQ(v.projectTo(Vec3::kUnitX), Vec3(-3, 0, 0));    // projection onto  x axis
        EXPECT_EQ(v.projectTo(Vec3::kUnitXNeg), Vec3(-3, 0, 0)); // projection onto -x axis
        EXPECT_EQ(v.projectTo(Vec3::kUnitY), Vec3(0, -4, 0));    // projection onto  y axis
        EXPECT_EQ(v.projectTo(Vec3::kUnitYNeg), Vec3(0, -4, 0)); // projection onto -y axis
        EXPECT_EQ(v.projectTo(Vec3::kUnitZ), Vec3(0, 0, -5));    // projection onto  z axis
        EXPECT_EQ(v.projectTo(Vec3::kUnitZNeg), Vec3(0, 0, -5)); // projection onto -z axis
    }
}

TEST(Vec3, perpendicularOnto) {
    {
        const Vec3 v(3, 4, 5);
        EXPECT_EQ(v.perpendicularTo(Vec3::kUnitX), Vec3(0, 4, 5));
        EXPECT_EQ(v.perpendicularTo(Vec3::kUnitY), Vec3(3, 0, 5));
        EXPECT_EQ(v.perpendicularTo(Vec3::kUnitZ), Vec3(3, 4, 0));
        EXPECT_EQ(v.perpendicularTo(Vec3::kUnitXNeg), Vec3(0, 4, 5));
        EXPECT_EQ(v.perpendicularTo(Vec3::kUnitYNeg), Vec3(3, 0, 5));
        EXPECT_EQ(v.perpendicularTo(Vec3::kUnitZNeg), Vec3(3, 4, 0));
    }

    {
        const Vec3 v(-3, -4, -5);
        EXPECT_EQ(v.perpendicularTo(Vec3::kUnitX), Vec3(0, -4, -5));
        EXPECT_EQ(v.perpendicularTo(Vec3::kUnitY), Vec3(-3, 0, -5));
        EXPECT_EQ(v.perpendicularTo(Vec3::kUnitZ), Vec3(-3, -4, 0));
        EXPECT_EQ(v.perpendicularTo(Vec3::kUnitXNeg), Vec3(0, -4, -5));
        EXPECT_EQ(v.perpendicularTo(Vec3::kUnitYNeg), Vec3(-3, 0, -5));
        EXPECT_EQ(v.perpendicularTo(Vec3::kUnitZNeg), Vec3(-3, -4, 0));
    }

    EXPECT_EQ(Vec3(1, 1, 0).perpendicularTo(Vec3::kUnitX), Vec3(0, 1, 0));
    EXPECT_EQ(Vec3(1, 2, 0).perpendicularTo(Vec3(1, 1, 0)), Vec3(-0.5, 0.5, 0));
}

TEST(Vec3, reflect) {
    // test reflection with a plane on the X-axis
    {
        EXPECT_EQ(Vec3(3, 4, 0).reflect(Vec3::kUnitY), Vec3(3, -4, 0));
        EXPECT_EQ(Vec3(3, 4, 0).reflect(Vec3::kUnitYNeg), Vec3(3, -4, 0));

        EXPECT_EQ(Vec3(3, -4, 0).reflect(Vec3::kUnitY), Vec3(3, 4, 0));
        EXPECT_EQ(Vec3(3, -4, 0).reflect(Vec3::kUnitYNeg), Vec3(3, 4, 0));

        EXPECT_EQ(Vec3(-3, -4, 0).reflect(Vec3::kUnitY), Vec3(-3, 4, 0));
        EXPECT_EQ(Vec3(-3, -4, 0).reflect(Vec3::kUnitYNeg), Vec3(-3, 4, 0));

        EXPECT_EQ(Vec3(-3, 4, 0).reflect(Vec3::kUnitY), Vec3(-3, -4, 0));
        EXPECT_EQ(Vec3(-3, 4, 0).reflect(Vec3::kUnitYNeg), Vec3(-3, -4, 0));

        EXPECT_EQ(Vec3(0, 4, 0).reflect(Vec3::kUnitY), Vec3(0, -4, 0));
        EXPECT_EQ(Vec3(0, 4, 0).reflect(Vec3::kUnitYNeg), Vec3(0, -4, 0));
    }

    // test with a plane pointing at (1,1)
    {
        const auto normal = Vec3(1, 1, 1).normalize();
        EXPECT_VEC3_NEAR(Vec3(-1, -1, -1).reflect(normal), Vec3(1.f, 1.f, 1));
        EXPECT_VEC3_NEAR(Vec3(-1, -1, -1).reflect(-normal), Vec3(1.f, 1.f, 1));

        EXPECT_VEC3_NEAR(Vec3::kUnitYNeg.reflect(normal), Vec3(0.6666666f, -0.33333337f, 0.6666666f));
        EXPECT_VEC3_NEAR(Vec3::kUnitYNeg.reflect(-normal), Vec3(0.6666666f, -0.33333337f, 0.6666666f));

        EXPECT_VEC3_NEAR(Vec3::kUnitXNeg.reflect(normal), Vec3(-0.33333337f, 0.6666666f, 0.6666666f));
        EXPECT_VEC3_NEAR(Vec3::kUnitXNeg.reflect(-normal), Vec3(-0.33333337f, 0.6666666f, 0.6666666f));
    }
}

TEST(Vec3, ptr) {
#if 0
    // const version
    const auto  v   = Vec3(3.F, 4.F, 0.F);
    const auto* ptr = v.ptr();
    EXPECT_FLOAT_EQ(ptr[0], 3.F);
    EXPECT_FLOAT_EQ(ptr[1], 4.F);
    EXPECT_FLOAT_EQ(ptr[2], 0.F);

    // non const version
    auto        v2   = Vec3(3.F, 4.F, 0.F);
    const auto* ptr2 = v2.ptr();
    EXPECT_FLOAT_EQ(ptr2[0], 3.F);
    EXPECT_FLOAT_EQ(ptr2[1], 4.F);
    EXPECT_FLOAT_EQ(ptr2[2], 0.F);
#endif
}

TEST(Vec3, std_format) { ASSERT_EQ(std::format("{}", Vec3(1, 2, 3)), "[1, 2, 3]"); }
