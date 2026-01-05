#include "GTestUtils.h"

#include <math/Mat2.h>

#include <gtest/gtest.h>


using namespace fuse;
using namespace testing;

TEST(Mat2, traits) {
    static_assert(std::is_trivially_copy_constructible_v<Mat2>);
    static_assert(std::is_trivially_copy_assignable_v<Mat2>);
    static_assert(std::is_trivially_move_constructible_v<Mat2>);
    static_assert(std::is_trivially_move_assignable_v<Mat2>);
    static_assert(std::is_trivially_copyable_v<Mat2>);
}

TEST(Mat2, constants) {
    static_assert(Mat2::kZero == Mat2(0.F, 0.F, 0.F, 0.f));
    static_assert(Mat2::kIdentity(0, 0) == 1.F);
    static_assert(Mat2::kIdentity(0, 1) == 0.F);
    static_assert(Mat2::kIdentity(1, 0) == 0.F);
    static_assert(Mat2::kIdentity(1, 1) == 1.F);

    EXPECT_EQ(Mat2::kZero, Mat2(0.F, 0.F, 0.F, 0.f));
    EXPECT_EQ(Mat2::kIdentity(0, 0), 1.F);
    EXPECT_EQ(Mat2::kIdentity(0, 1), 0.F);
    EXPECT_EQ(Mat2::kIdentity(1, 0), 0.F);
    EXPECT_EQ(Mat2::kIdentity(1, 1), 1.F);
}

TEST(Mat2, ctor_by_elements) {
    const Mat2 m(0, 1, 2, 3);

    auto* a = m.data();
    EXPECT_EQ(a[0], 0);
    EXPECT_EQ(a[2], 1);
    EXPECT_EQ(a[1], 2);
    EXPECT_EQ(a[3], 3);

    EXPECT_EQ(m(0, 0), 0);
    EXPECT_EQ(m(0, 1), 1);
    EXPECT_EQ(m(1, 0), 2);
    EXPECT_EQ(m(1, 1), 3);
}

TEST(Mat2, ctor_by_vec2) {
    const Mat2 m(Vec2(0, 2), Vec2(1, 3));
    EXPECT_EQ(m(0, 0), 0);
    EXPECT_EQ(m(0, 1), 1);
    EXPECT_EQ(m(1, 0), 2);
    EXPECT_EQ(m(1, 1), 3);
}

// ============================================
//                  Addition
// ============================================
TEST(Mat2, addition) {
    const Mat2 matrix(0, 1, 2, 3);
    // operator+(Mat2, Mat2)
    {
        const Mat2 r = matrix + matrix;
        EXPECT_EQ(r(0, 0), 0);
        EXPECT_EQ(r(0, 1), 2);
        EXPECT_EQ(r(1, 0), 4);
        EXPECT_EQ(r(1, 1), 6);
    }
    // operator+=(Mat2, Mat2)
    {
        Mat2        m(matrix);
        const Mat2& r = m += m;
        EXPECT_EQ(m(0, 0), 0);
        EXPECT_EQ(m(0, 1), 2);
        EXPECT_EQ(m(1, 0), 4);
        EXPECT_EQ(m(1, 1), 6);

        EXPECT_EQ(r(0, 0), 0);
        EXPECT_EQ(r(0, 1), 2);
        EXPECT_EQ(r(1, 0), 4);
        EXPECT_EQ(r(1, 1), 6);

        EXPECT_EQ(&r, &m);
    }
}

// ============================================
//                  Substation
// ============================================
TEST(Mat2, substract) {
    const Mat2 matrix(0, 1, 2, 3);
    // operator-(Mat2, Mat2)
    {
        const Mat2 r = matrix - matrix;
        EXPECT_EQ(r(0, 0), 0);
        EXPECT_EQ(r(0, 1), 0);
        EXPECT_EQ(r(1, 0), 0);
        EXPECT_EQ(r(1, 1), 0);
    }
    // operator-=(Mat2, Mat2)
    {
        Mat2        m(matrix);
        const Mat2& r = m -= m;
        EXPECT_EQ(m(0, 0), 0);
        EXPECT_EQ(m(0, 1), 0);
        EXPECT_EQ(m(1, 0), 0);
        EXPECT_EQ(m(1, 1), 0);

        EXPECT_EQ(r(0, 0), 0);
        EXPECT_EQ(r(0, 1), 0);
        EXPECT_EQ(r(1, 0), 0);
        EXPECT_EQ(r(1, 1), 0);

        EXPECT_EQ(&r, &m);
    }
}

// ============================================
//              Multiplication
// ============================================

TEST(Mat2, mul_by_scalar) {
    const Mat2 matrix(1, 2, 3, 4);

    // operator*(scalar, Mat2)
    {
        const Mat2 r = 2 * matrix;
        EXPECT_EQ(r(0, 0), 2);
        EXPECT_EQ(r(0, 1), 4);
        EXPECT_EQ(r(1, 0), 6);
        EXPECT_EQ(r(1, 1), 8);
    }
    // operator*(Mat2, scalar)
    {
        const Mat2 r = matrix * 2;
        EXPECT_EQ(r(0, 0), 2);
        EXPECT_EQ(r(0, 1), 4);
        EXPECT_EQ(r(1, 0), 6);
        EXPECT_EQ(r(1, 1), 8);
    }
    // operator*=(Mat2, scalar)
    {
        Mat2        m(matrix);
        const Mat2& r = m *= 2;
        EXPECT_EQ(m(0, 0), 2);
        EXPECT_EQ(m(0, 1), 4);
        EXPECT_EQ(m(1, 0), 6);
        EXPECT_EQ(m(1, 1), 8);

        EXPECT_EQ(r(0, 0), 2);
        EXPECT_EQ(r(0, 1), 4);
        EXPECT_EQ(r(1, 0), 6);
        EXPECT_EQ(r(1, 1), 8);

        EXPECT_EQ(&r, &m);
    }
}

TEST(Mat2, mul_by_vec2) {
    //
    // 1  2     10    50
    // 4  5  X  20 =  140
    //
    const Mat2 matrix(1, 2, 4, 5);
    const auto result = matrix * Vec2(10, 20);
    EXPECT_EQ(result.x, 50);
    EXPECT_EQ(result.y, 140);
}

TEST(Mat2, mul_by_mat2) {
    //
    // 1  2     3  4     21 28
    // 3  4  X  9  12 =  45 60
    //
    const Mat2 matrix1(1, 2, 3, 4);
    const Mat2 matrix2(3, 4, 9, 12);
    // operator*(Mat2, Mat2)
    {
        const auto result = matrix1 * matrix2;
        EXPECT_EQ(result(0, 0), 21);
        EXPECT_EQ(result(0, 1), 28);
        EXPECT_EQ(result(1, 0), 45);
        EXPECT_EQ(result(1, 1), 60);
    }
    // operator*=(Mat2, Mat2)
    {
        Mat2        m(matrix1);
        const auto& result = m *= matrix2;
        EXPECT_EQ(m(0, 0), 21);
        EXPECT_EQ(m(0, 1), 28);
        EXPECT_EQ(m(1, 0), 45);
        EXPECT_EQ(m(1, 1), 60);

        EXPECT_EQ(result(0, 0), 21);
        EXPECT_EQ(result(0, 1), 28);
        EXPECT_EQ(result(1, 0), 45);
        EXPECT_EQ(result(1, 1), 60);

        EXPECT_EQ(&result, &m);
    }
}

TEST(Mat2, determinant) {

    // zero matrix
    EXPECT_EQ(Mat2::kZero.determinant(), 0);
    // identity matrix
    EXPECT_EQ(Mat2::kIdentity.determinant(), 1);

    // 1 2
    // 3 4
    EXPECT_EQ(Mat2(1, 2, 3, 4).determinant(), -2);

    // -1 -2
    // -3 -4
    EXPECT_EQ(Mat2(-1, -2, -3, -4).determinant(), -2);
}

TEST(Mat2, inverse) {
    // identity matrix
    EXPECT_EQ(Mat2::kIdentity.inverse(), Mat2::kIdentity);

    // 1 2
    // 3 4
    EXPECT_EQ(Mat2(1, 2, 3, 4).inverse(), Mat2(-2, 1, 3 / 2.f, -1 / 2.f));
}

TEST(Mat2, transpose) {
    // 1 2 => 1 3
    // 3 4    2 4
    const auto m = Mat2(1, 2, 3, 4).transpose();
    EXPECT_EQ(m(0, 0), 1);
    EXPECT_EQ(m(0, 1), 3);
    EXPECT_EQ(m(1, 0), 2);
    EXPECT_EQ(m(1, 1), 4);
}
