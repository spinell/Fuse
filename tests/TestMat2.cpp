#include "GtestUtils.h"

#include <gtest/gtest.h>
#include <math/Mat2.h>


using namespace fuse;
using namespace testing;

TEST(Mat2, ctor_by_elements) {
    // clang-format off
    const Mat2 m(
        0, 1, // row0
        2, 3  // row1
    );

    auto* a = m.data();
    EXPECT_EQ(a[0], 0); EXPECT_EQ(a[2], 1);
    EXPECT_EQ(a[1], 2); EXPECT_EQ(a[3], 3);

    EXPECT_EQ(m(0,0), 0); EXPECT_EQ(m(0,1), 1);
    EXPECT_EQ(m(1,0), 2); EXPECT_EQ(m(1,1), 3);
    // clang-format on
}

TEST(Mat2, ctor_by_vec2) {
    // clang-format off
    const Mat2 m(
        Vec2(0, 2), // col 0
        Vec2(1, 3)  // col 1
    );

    EXPECT_EQ(m(0,0), 0);  EXPECT_EQ(m(0,1), 1);
    EXPECT_EQ(m(1,0), 2);  EXPECT_EQ(m(1,1), 3);
    // clang-format on
}

// ============================================
//                  Addition
// ============================================
TEST(Mat2, addition) {
    const Mat2 matrix(0, 1, 2, 3);
    {
        const Mat2 r = matrix + matrix;

        // clang-format off
        EXPECT_EQ(r(0, 0), 0); EXPECT_EQ(r(0, 1), 2);
        EXPECT_EQ(r(1, 0), 4); EXPECT_EQ(r(1, 1), 6);
        // clang-format on
    }
    {
        Mat2       m(matrix);
        const Mat2 r = m += m;

        // clang-format off
        EXPECT_EQ(m(0, 0), 0); EXPECT_EQ(m(0, 1), 2);
        EXPECT_EQ(m(1, 0), 4); EXPECT_EQ(m(1, 1), 6);

        EXPECT_EQ(r(0, 0), 0); EXPECT_EQ(r(0, 1), 2);
        EXPECT_EQ(r(1, 0), 4); EXPECT_EQ(r(1, 1), 6);
        // clang-format on
    }
}

// ============================================
//                  Substation
// ============================================
TEST(Mat2, substract) {
    const Mat2 matrix(0, 1, 2, 3);
    {
        const Mat2 r = matrix - matrix;

        // clang-format off
        EXPECT_EQ(r(0, 0), 0); EXPECT_EQ(r(0, 1), 0);
        EXPECT_EQ(r(1, 0), 0); EXPECT_EQ(r(1, 1), 0);
        // clang-format on
    }
    {
        Mat2       m(matrix);
        const Mat2 r = m -= m;

        // clang-format off
        EXPECT_EQ(m(0, 0), 0); EXPECT_EQ(m(0, 1), 0);
        EXPECT_EQ(m(1, 0), 0); EXPECT_EQ(m(1, 1), 0);

        EXPECT_EQ(r(0, 0), 0); EXPECT_EQ(r(0, 1), 0);
        EXPECT_EQ(r(1, 0), 0); EXPECT_EQ(r(1, 1), 0);
        // clang-format on
    }
}

// ============================================
//              Multiplication
// ============================================

TEST(Mat2, mul_by_scalar) {
    const Mat2 matrix(1, 2, 3, 4);

    {
        const Mat2 r = matrix * 2;

        // clang-format off
        EXPECT_EQ(r(0, 0),  2); EXPECT_EQ(r(0, 1), 4);
        EXPECT_EQ(r(1, 0),  6); EXPECT_EQ(r(1, 1), 8);
        // clang-format on

        const Mat2 r2 = 2 * matrix;
        // clang-format off
        EXPECT_EQ(r2(0, 0),  2); EXPECT_EQ(r2(0, 1), 4);
        EXPECT_EQ(r2(1, 0),  6); EXPECT_EQ(r2(1, 1), 8);
        // clang-format on
    }
    {
        Mat2       m(matrix);
        const Mat2 r = m *= 2;

        // clang-format off
        EXPECT_EQ(m(0, 0),  2); EXPECT_EQ(m(0, 1), 4);
        EXPECT_EQ(m(1, 0),  6); EXPECT_EQ(m(1, 1), 8);

        EXPECT_EQ(r(0, 0),  2); EXPECT_EQ(r(0, 1), 4);
        EXPECT_EQ(r(1, 0),  6); EXPECT_EQ(r(1, 1), 8);
        // clang-format on
    }
}

TEST(Mat2, mul_by_vec2) {
    //
    // 1  2     10    50
    // 4  5  X  20 =  140
    //
    const Mat2 matrix(
      // clang-format off
         1, 2,
         4, 5
      // clang-format on
    );

    const auto result = matrix * Vec2(10, 20);
    EXPECT_EQ(result.x, 50);
    EXPECT_EQ(result.y, 140);
}

TEST(Mat2, mul_by_mat2) {
    //
    // 1  2     3  4     21 28
    // 3  4  X  9  12 =  45 60
    //

    const Mat2 matrix1(
      // clang-format off
        1, 2,
        3, 4
      // clang-format on
    );
    const Mat2 matrix2(
      // clang-format off
        3,  4,
        9, 12
      // clang-format on
    );

    {
        const auto result = matrix1 * matrix2;
        // clang-format off
        EXPECT_EQ(result(0, 0), 21); EXPECT_EQ(result(0, 1), 28);
        EXPECT_EQ(result(1, 0), 45); EXPECT_EQ(result(1, 1), 60);
        // clang-format on
    }

    {
        Mat2       m(matrix1);
        const auto result = m *= matrix2;
        // clang-format off
        EXPECT_EQ(m(0, 0), 21); EXPECT_EQ(m(0, 1), 28);
        EXPECT_EQ(m(1, 0), 45); EXPECT_EQ(m(1, 1), 60);

        EXPECT_EQ(result(0, 0), 21); EXPECT_EQ(result(0, 1), 28);
        EXPECT_EQ(result(1, 0), 45); EXPECT_EQ(result(1, 1), 60);
        // clang-format on
    }
}
