#include "GTestUtils.h"

#include <math/Vec4.h>

#include <gtest/gtest.h>

using fuse::Vec4;

TEST(Vec4, traits) {
    static_assert(std::is_trivially_copy_constructible_v<Vec4>);
    static_assert(std::is_trivially_copy_assignable_v<Vec4>);
    static_assert(std::is_trivially_move_constructible_v<Vec4>);
    static_assert(std::is_trivially_move_assignable_v<Vec4>);
    static_assert(std::is_trivially_copyable_v<Vec4>);
}

TEST(Vec4, constants) {
    static_assert(Vec4::kZero == Vec4(0.F, 0.F, 0.F, 0.F), "Vec4::kZero should be (0, 0, 0, 0)");
    static_assert(Vec4::kUnitX == Vec4(1.F, 0.F, 0.F, 0.F), "Vec4::kUnitX should be (1, 0, 0, 0)");
    static_assert(Vec4::kUnitY == Vec4(0.F, 1.F, 0.F, 0.F), "Vec4::kUnitY should be (0, 1, 0, 0)");
    static_assert(Vec4::kUnitZ == Vec4(0.F, 0.F, 1.F, 0.F), "Vec4::kUnitZ should be (0, 0, 1, 0)");
    static_assert(Vec4::kUnitW == Vec4(0.F, 0.F, 0.F, 1.F), "Vec4::kUnitZ should be (0, 0, 0, 1)");
    static_assert(Vec4::kUnitXNeg == Vec4(-1.F, 0.F, 0.F, 0.F),
                  "Vec4::kUnitX should be (-1, 0, 0, 0)");
    static_assert(Vec4::kUnitYNeg == Vec4(0.F, -1.F, 0.F, 0.F),
                  "Vec4::kUnitY should be (0, -1, 0, 0)");
    static_assert(Vec4::kUnitZNeg == Vec4(0.F, 0.F, -1.F, 0.F),
                  "Vec4::kUnitZ should be (0, 0, -1, 0)");

    EXPECT_EQ(Vec4::kZero, Vec4(0.F, 0.F, 0.F, 0.f));
    EXPECT_EQ(Vec4::kUnitX, Vec4(1.F, 0.F, 0.F, 0.f));
    EXPECT_EQ(Vec4::kUnitY, Vec4(0.F, 1.F, 0.F, 0.f));
    EXPECT_EQ(Vec4::kUnitZ, Vec4(0.F, 0.F, 1.F, 0.f));
    EXPECT_EQ(Vec4::kUnitW, Vec4(0.F, 0.F, 0.F, 1.f));
    EXPECT_EQ(Vec4::kUnitXNeg, Vec4(-1.F, 0.F, 0.F, 0.f));
    EXPECT_EQ(Vec4::kUnitYNeg, Vec4(0.F, -1.F, 0.F, 0.f));
    EXPECT_EQ(Vec4::kUnitZNeg, Vec4(0.F, 0.F, -1.F, 0.f));
}

// ===========================================================
//              constructor and copy constructor
// ===========================================================

TEST(Vec4, ctor_from_components) {
    const Vec4 v(1, 2, 3, 4);
    EXPECT_FLOAT_EQ(v.x, 1.f);
    EXPECT_FLOAT_EQ(v.y, 2.f);
    EXPECT_FLOAT_EQ(v.z, 3.f);
    EXPECT_FLOAT_EQ(v.w, 4.f);
}

TEST(Vec4, copy_ctor) {
    const Vec4 v1(1.f, 2.f, 3.f, 4.f);
    const Vec4 v2 = v1;
    EXPECT_FLOAT_EQ(v1.x, v2.x);
    EXPECT_FLOAT_EQ(v1.y, v2.y);
    EXPECT_FLOAT_EQ(v1.z, v2.z);
    EXPECT_FLOAT_EQ(v1.w, v2.w);
}

// ===========================================================
//                  unary operators
// ===========================================================

TEST(Vec4, negate) {
    const auto negated = -Vec4(1.f, -2.f, 3.f, 4.f);
    EXPECT_FLOAT_EQ(negated.x, -1.f);
    EXPECT_FLOAT_EQ(negated.y, 2.f);
    EXPECT_FLOAT_EQ(negated.z, -3.f);
    EXPECT_FLOAT_EQ(negated.w, -4.f);
}

// ==============================================
//            comparaison operators
// ==============================================
TEST(Vec4, equals) {
    const auto v1 = Vec4(1.F, 2.F, 3.F, 4.F);
    const auto v2 = Vec4(1.F, 2.F, 3.F, 4.F);
    EXPECT_EQ(v1, v2);
}

TEST(Vec4, not_equals) {
    EXPECT_NE(Vec4(0.F, 2.F, 3.F, 4.F), Vec4(1.F, 2.F, 3.F, 4.F)); // x not equal
    EXPECT_NE(Vec4(1.F, 0.F, 3.F, 4.F), Vec4(1.F, 2.F, 3.F, 4.F)); // y not equal
    EXPECT_NE(Vec4(1.F, 2.F, 0.F, 4.F), Vec4(1.F, 2.F, 3.F, 4.F)); // z not equal
    EXPECT_NE(Vec4(1.F, 2.F, 3.F, 0.F), Vec4(1.F, 2.F, 3.F, 4.F)); // w not equal
}

// ===========================================================
//                          Addition
// ===========================================================

TEST(Vec4, add_vec4) {
    // operator+(vec4, vec4)
    {
        const auto r = Vec4(1, 2, 3, 4) + Vec4(10, 20, 30, 40);
        EXPECT_EQ(r.x, 11.f);
        EXPECT_EQ(r.y, 22.f);
        EXPECT_EQ(r.z, 33.f);
        EXPECT_EQ(r.w, 44.f);
    }

    // operator+=(vec4, vec4)
    {
        Vec4        v(1, 2, 3, 4);
        const Vec4  v2(10, 20, 30, 40);
        const Vec4& r = v += v2;

        EXPECT_EQ(v.x, 11.f);
        EXPECT_EQ(v.y, 22.f);
        EXPECT_EQ(v.z, 33.f);
        EXPECT_EQ(v.w, 44.f);

        EXPECT_EQ(r.x, 11.f);
        EXPECT_EQ(r.y, 22.f);
        EXPECT_EQ(r.z, 33.f);
        EXPECT_EQ(r.w, 44.f);
        EXPECT_EQ(&r, &v);
    }
}

// ===========================================================
//                       Substitution
// ===========================================================

TEST(Vec4, substract_two_vector) {
    // operator-(vec4, vec4)
    {
        const auto r = Vec4(10, 20, 30, 40) - Vec4(1, 2, 3, 4);
        EXPECT_EQ(r.x, 9.f);
        EXPECT_EQ(r.y, 18.f);
        EXPECT_EQ(r.z, 27.f);
        EXPECT_EQ(r.w, 36.f);
    }

    // operator-=(vec4, vec4)
    {
        Vec4        v(1, 2, 3, 4);
        const Vec4  v2(10, 20, 30, 40);
        const Vec4& r = v -= v2;

        EXPECT_EQ(v.x, -9.f);
        EXPECT_EQ(v.y, -18.f);
        EXPECT_EQ(v.z, -27.f);
        EXPECT_EQ(v.w, -36.f);

        EXPECT_EQ(r.x, -9.f);
        EXPECT_EQ(r.y, -18.f);
        EXPECT_EQ(r.z, -27.f);
        EXPECT_EQ(r.w, -36.f);
        EXPECT_EQ(&r, &v);
    }
}

// ==========================================================
//                    Multiplication
// ==========================================================
TEST(Vec4, multiple_by_scalar) {
    // operator*(Vec4, scalar)
    {
        const auto r = Vec4(1, 2, 3, 4) * 2;
        EXPECT_EQ(r.x, 2);
        EXPECT_EQ(r.y, 4);
        EXPECT_EQ(r.z, 6);
        EXPECT_EQ(r.w, 8);
    }
    // operator*(scalar, Vec4)
    {
        const auto r = 2 * Vec4(1, 2, 3, 4);
        EXPECT_EQ(r.x, 2);
        EXPECT_EQ(r.y, 4);
        EXPECT_EQ(r.z, 6);
        EXPECT_EQ(r.w, 8);
    }
    // operator*=(Vec4, scalar)
    {
        Vec4        v(1, 2, 3, 4);
        const Vec4& r = v *= 2.f;

        EXPECT_EQ(v.x, 2.f);
        EXPECT_EQ(v.y, 4.f);
        EXPECT_EQ(v.z, 6.f);
        EXPECT_EQ(v.w, 8.f);

        EXPECT_EQ(r.x, 2.f);
        EXPECT_EQ(r.y, 4.f);
        EXPECT_EQ(r.z, 6.f);
        EXPECT_EQ(r.w, 8.f);
        EXPECT_EQ(&r, &v);
    }
}

// ===========================================================
//                      Division
// ==========================================================
TEST(Vec4, div_scalar) {
    // operator/(Vec4, scalar)
    {
        const auto r = Vec4(10, 20, 30, 40) / 2;
        EXPECT_EQ(r.x, 5);
        EXPECT_EQ(r.y, 10);
        EXPECT_EQ(r.z, 15);
        EXPECT_EQ(r.w, 20);
    }
    // operator/=(Vec4, scalar)
    {
        Vec4        v(10, 20, 30, 40);
        const Vec4& r = v /= 2.f;

        EXPECT_EQ(v.x, 5.f);
        EXPECT_EQ(v.y, 10.f);
        EXPECT_EQ(v.z, 15.f);
        EXPECT_EQ(v.w, 20.f);

        EXPECT_EQ(r.x, 5.f);
        EXPECT_EQ(r.y, 10.f);
        EXPECT_EQ(r.z, 15.f);
        EXPECT_EQ(r.w, 20.f);
        EXPECT_EQ(&r, &v);
    }
}

// ==============================================
//            member functions
// ==============================================

TEST(Vec4, dot) {
    const Vec4 v1(1, 2, 3, 4);
    const Vec4 v2(5, 6, 7, 8);
    EXPECT_EQ(v1.dot(v2), 70.f);
}

TEST(Vec4, length) {
    EXPECT_FLOAT_EQ(Vec4::kZero.length(), 0.f);
    EXPECT_FLOAT_EQ(Vec4::kUnitX.length(), 1.f);
    EXPECT_FLOAT_EQ(Vec4::kUnitY.length(), 1.f);
    EXPECT_FLOAT_EQ(Vec4::kUnitZ.length(), 1.f);
    EXPECT_FLOAT_EQ(Vec4::kUnitW.length(), 1.f);
    EXPECT_FLOAT_EQ(Vec4(1, 1, 0, 0).length(), 1.4142135f);
    EXPECT_FLOAT_EQ(Vec4(1, 1, 1, 0).length(), 1.7320508f);
    EXPECT_FLOAT_EQ(Vec4(1, 1, 1, 1).length(), 2.f);
}

TEST(Vec4, lengthSquared) {
    EXPECT_FLOAT_EQ(Vec4::kZero.lengthSquared(), 0.f);
    EXPECT_FLOAT_EQ(Vec4::kUnitX.lengthSquared(), 1.f);
    EXPECT_FLOAT_EQ(Vec4::kUnitY.lengthSquared(), 1.f);
    EXPECT_FLOAT_EQ(Vec4::kUnitZ.lengthSquared(), 1.f);
    EXPECT_FLOAT_EQ(Vec4::kUnitW.lengthSquared(), 1.f);
    EXPECT_FLOAT_EQ(Vec4(1, 1, 0, 0).lengthSquared(), 2.f);
    EXPECT_FLOAT_EQ(Vec4(1, 1, 1, 0).lengthSquared(), 3.f);
    EXPECT_FLOAT_EQ(Vec4(1, 1, 1, 1).lengthSquared(), 4.f);
}

TEST(Vec4, normalize) {
    // they are already normalized, nothing should change.
    EXPECT_EQ(Vec4(1, 0, 0, 0.f).normalize(), Vec4(1.f, 0.f, 0.f, 0.f));
    EXPECT_EQ(Vec4(0, 1, 0, 0.f).normalize(), Vec4(0.f, 1.f, 0.f, 0.f));
    EXPECT_EQ(Vec4(0, 0, 1, 0.f).normalize(), Vec4(0.f, 0.f, 1.f, 0.f));
    EXPECT_EQ(Vec4(0, 0, 0, 1.f).normalize(), Vec4(0.f, 0.f, 0.f, 1.f));

    // they are already normalized, nothing should change.
    EXPECT_EQ(Vec4(-1, 0, 0, 0.f).normalize(), Vec4(-1.f, 0.f, 0.f, 0.f));
    EXPECT_EQ(Vec4(0, -1, 0, 0.f).normalize(), Vec4(0.f, -1.f, 0.f, 0.f));
    EXPECT_EQ(Vec4(0, 0, -1, 0.f).normalize(), Vec4(0.f, 0.f, -1.f, 0.f));
    EXPECT_EQ(Vec4(0, 0, 0, -1.f).normalize(), Vec4(0.f, 0.f, 0.f, -1.f));

    const auto v = Vec4(1, 2, 3, 4).normalize();
    EXPECT_FLOAT_EQ(v.x, 0.18257418f);
    EXPECT_FLOAT_EQ(v.y, 0.36514837f);
    EXPECT_FLOAT_EQ(v.z, 0.54772258f);
    EXPECT_FLOAT_EQ(v.w, 0.73029673f);
}

TEST(Vec4, std_format) { ASSERT_EQ(std::format("{}", Vec4(1, 2, 3, 4)), "[1, 2, 3, 4]"); }
