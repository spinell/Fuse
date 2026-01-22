#include "GTestUtils.h"

#include <fuse/math/Angle.h>
#include <fuse/math/Mat4.h>
#include <fuse/math/Vec3.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace fuse;
using namespace testing;

TEST(Mat4, traits) {
    static_assert(std::is_trivially_copy_constructible_v<Mat4>);
    static_assert(std::is_trivially_copy_assignable_v<Mat4>);
    static_assert(std::is_trivially_move_constructible_v<Mat4>);
    static_assert(std::is_trivially_move_assignable_v<Mat4>);
    static_assert(std::is_trivially_copyable_v<Mat4>);
}

TEST(Mat4, constants) {
    static_assert(
      Mat4::kZero ==
      Mat4(0.F, 0.F, 0.F, 0.f, 0.F, 0.f, 0.F, 0.f, 0.f, 0.F, 0.f, 0.f, 0.F, 0.f, 0.f, 0.f));
    static_assert(Mat4::kIdentity(0, 0) == 1.F);
    static_assert(Mat4::kIdentity(0, 1) == 0.F);
    static_assert(Mat4::kIdentity(0, 2) == 0.F);
    static_assert(Mat4::kIdentity(0, 3) == 0.F);
    static_assert(Mat4::kIdentity(1, 0) == 0.F);
    static_assert(Mat4::kIdentity(1, 1) == 1.F);
    static_assert(Mat4::kIdentity(1, 2) == 0.F);
    static_assert(Mat4::kIdentity(1, 3) == 0.F);
    static_assert(Mat4::kIdentity(2, 0) == 0.F);
    static_assert(Mat4::kIdentity(2, 1) == 0.F);
    static_assert(Mat4::kIdentity(2, 2) == 1.F);
    static_assert(Mat4::kIdentity(2, 3) == 0.F);
    static_assert(Mat4::kIdentity(3, 0) == 0.F);
    static_assert(Mat4::kIdentity(3, 1) == 0.F);
    static_assert(Mat4::kIdentity(3, 2) == 0.F);
    static_assert(Mat4::kIdentity(3, 3) == 1.F);

    EXPECT_EQ(Mat4::kZero,
              Mat4(0.F, 0.F, 0.F, 0.f, 0.F, 0.f, 0.F, 0.f, 0.f, 0.F, 0.f, 0.f, 0.F, 0.f, 0.f, 0.f));
    EXPECT_EQ(Mat4::kIdentity(0, 0), 1.F);
    EXPECT_EQ(Mat4::kIdentity(0, 1), 0.F);
    EXPECT_EQ(Mat4::kIdentity(0, 2), 0.F);
    EXPECT_EQ(Mat4::kIdentity(0, 3), 0.F);
    EXPECT_EQ(Mat4::kIdentity(1, 0), 0.F);
    EXPECT_EQ(Mat4::kIdentity(1, 1), 1.F);
    EXPECT_EQ(Mat4::kIdentity(1, 2), 0.F);
    EXPECT_EQ(Mat4::kIdentity(1, 3), 0.F);
    EXPECT_EQ(Mat4::kIdentity(2, 0), 0.F);
    EXPECT_EQ(Mat4::kIdentity(2, 1), 0.F);
    EXPECT_EQ(Mat4::kIdentity(2, 2), 1.F);
    EXPECT_EQ(Mat4::kIdentity(2, 3), 0.F);
    EXPECT_EQ(Mat4::kIdentity(3, 0), 0.F);
    EXPECT_EQ(Mat4::kIdentity(3, 1), 0.F);
    EXPECT_EQ(Mat4::kIdentity(3, 2), 0.F);
    EXPECT_EQ(Mat4::kIdentity(3, 3), 1.F);
}

TEST(Mat4, ctor_by_elements) {
    // clang-format off
    const Mat4 m(
        0,   1,  2,  3, // row 0
        4,   5,  6,  7, // row 1
        8,   9, 10, 11, // row 2
        12, 13, 14, 15  // row 3
    );

    auto* a = m.data();
    EXPECT_EQ(a[0],  0); EXPECT_EQ(a[4],  1); EXPECT_EQ(a[8],   2); EXPECT_EQ(a[12], 3);
    EXPECT_EQ(a[1],  4); EXPECT_EQ(a[5],  5); EXPECT_EQ(a[9],   6); EXPECT_EQ(a[13], 7);
    EXPECT_EQ(a[2],  8); EXPECT_EQ(a[6],  9); EXPECT_EQ(a[10], 10); EXPECT_EQ(a[14], 11);
    EXPECT_EQ(a[3], 12); EXPECT_EQ(a[7], 13); EXPECT_EQ(a[11], 14); EXPECT_EQ(a[15], 15);

    EXPECT_EQ(m(0,0), 0);  EXPECT_EQ(m(0,1), 1);  EXPECT_EQ(m(0,2), 2);  EXPECT_EQ(m(0,3), 3);  // row 0
    EXPECT_EQ(m(1,0), 4);  EXPECT_EQ(m(1,1), 5);  EXPECT_EQ(m(1,2), 6);  EXPECT_EQ(m(1,3), 7);  // row 1
    EXPECT_EQ(m(2,0), 8);  EXPECT_EQ(m(2,1), 9);  EXPECT_EQ(m(2,2), 10); EXPECT_EQ(m(2,3), 11); // row 2
    EXPECT_EQ(m(3,0), 12); EXPECT_EQ(m(3,1), 13); EXPECT_EQ(m(3,2), 14); EXPECT_EQ(m(3,3), 15); // row 3
    // clang-format on
}

TEST(Mat4, ctor_by_vec4) {
    // 0   1  2  3
    // 4   5  6  7
    // 8   9 10 11
    // 12 13 14 15

    // clang-format off
    const Mat4 m(
        Vec4(0, 4,  8, 12), // col 0
        Vec4(1, 5,  9, 13), // col 1
        Vec4(2, 6, 10, 14), // col 2
        Vec4(3, 7, 11, 15)  // col 3
    );

    EXPECT_EQ(m(0,0), 0);  EXPECT_EQ(m(0,1), 1);  EXPECT_EQ(m(0,2), 2);  EXPECT_EQ(m(0,3), 3);  // row 0
    EXPECT_EQ(m(1,0), 4);  EXPECT_EQ(m(1,1), 5);  EXPECT_EQ(m(1,2), 6);  EXPECT_EQ(m(1,3), 7);  // row 1
    EXPECT_EQ(m(2,0), 8);  EXPECT_EQ(m(2,1), 9);  EXPECT_EQ(m(2,2), 10); EXPECT_EQ(m(2,3), 11); // row 2
    EXPECT_EQ(m(3,0), 12); EXPECT_EQ(m(3,1), 13); EXPECT_EQ(m(3,2), 14); EXPECT_EQ(m(3,3), 15); // row 3
    // clang-format on
}

// ============================================
//                  Addition
// ============================================
TEST(Mat4, addition) {
    const Mat4 matrix(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
    // operator+(Mat4,Mat4)
    {
        const Mat4 r = matrix + matrix;

        // clang-format off
        EXPECT_EQ(r(0, 0),  2); EXPECT_EQ(r(0, 1),  4); EXPECT_EQ(r(0, 2),  6); EXPECT_EQ(r(0, 3),  8);
        EXPECT_EQ(r(1, 0), 10); EXPECT_EQ(r(1, 1), 12); EXPECT_EQ(r(1, 2), 14); EXPECT_EQ(r(1, 3), 16);
        EXPECT_EQ(r(2, 0), 18); EXPECT_EQ(r(2, 1), 20); EXPECT_EQ(r(2, 2), 22); EXPECT_EQ(r(2, 3), 24);
        EXPECT_EQ(r(3, 0), 26); EXPECT_EQ(r(3, 1), 28); EXPECT_EQ(r(3, 2), 30); EXPECT_EQ(r(3, 3), 32);
        // clang-format on
    }
    // operator+=(Mat4,Mat4)
    {
        Mat4        m(matrix);
        const Mat4& r = m += m;

        // clang-format off
        EXPECT_EQ(m(0, 0),  2); EXPECT_EQ(m(0, 1),  4);  EXPECT_EQ(m(0, 2),  6); EXPECT_EQ(m(0, 3),  8);
        EXPECT_EQ(m(1, 0), 10); EXPECT_EQ(m(1, 1), 12);  EXPECT_EQ(m(1, 2), 14); EXPECT_EQ(m(1, 3), 16);
        EXPECT_EQ(m(2, 0), 18); EXPECT_EQ(m(2, 1), 20);  EXPECT_EQ(m(2, 2), 22); EXPECT_EQ(m(2, 3), 24);
        EXPECT_EQ(m(3, 0), 26); EXPECT_EQ(m(3, 1), 28);  EXPECT_EQ(m(3, 2), 30); EXPECT_EQ(m(3, 3), 32);

        EXPECT_EQ(r(0, 0),  2); EXPECT_EQ(r(0, 1),  4); EXPECT_EQ(r(0, 2),  6); EXPECT_EQ(r(0, 3),  8);
        EXPECT_EQ(r(1, 0), 10); EXPECT_EQ(r(1, 1), 12); EXPECT_EQ(r(1, 2), 14); EXPECT_EQ(r(1, 3), 16);
        EXPECT_EQ(r(2, 0), 18); EXPECT_EQ(r(2, 1), 20); EXPECT_EQ(r(2, 2), 22); EXPECT_EQ(r(2, 3), 24);
        EXPECT_EQ(r(3, 0), 26); EXPECT_EQ(r(3, 1), 28); EXPECT_EQ(r(3, 2), 30); EXPECT_EQ(r(3, 3), 32);

        EXPECT_EQ(&r, &m);
        // clang-format on
    }
}

// ============================================
//                  Substation
// ============================================
TEST(Mat4, substract) {
    const Mat4 matrix(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
    // operator-(Mat4,Mat4)
    {
        const Mat4 r = matrix - matrix;

        // clang-format off
        EXPECT_EQ(r(0, 0), 0); EXPECT_EQ(r(0, 1), 0); EXPECT_EQ(r(0, 2), 0); EXPECT_EQ(r(0, 3), 0);
        EXPECT_EQ(r(1, 0), 0); EXPECT_EQ(r(1, 1), 0); EXPECT_EQ(r(1, 2), 0); EXPECT_EQ(r(1, 3), 0);
        EXPECT_EQ(r(2, 0), 0); EXPECT_EQ(r(2, 1), 0); EXPECT_EQ(r(2, 2), 0); EXPECT_EQ(r(2, 3), 0);
        EXPECT_EQ(r(3, 0), 0); EXPECT_EQ(r(3, 1), 0); EXPECT_EQ(r(3, 2), 0); EXPECT_EQ(r(3, 3), 0);
        // clang-format on
    }
    // operator-=(Mat4,Mat4)
    {
        Mat4        m(matrix);
        const Mat4& r = m -= m;

        // clang-format off
        EXPECT_EQ(m(0, 0), 0); EXPECT_EQ(m(0, 1), 0); EXPECT_EQ(m(0, 2), 0); EXPECT_EQ(m(0, 3), 0);
        EXPECT_EQ(m(1, 0), 0); EXPECT_EQ(m(1, 1), 0); EXPECT_EQ(m(1, 2), 0); EXPECT_EQ(m(1, 3), 0);
        EXPECT_EQ(m(2, 0), 0); EXPECT_EQ(m(2, 1), 0); EXPECT_EQ(m(2, 2), 0); EXPECT_EQ(m(2, 3), 0);
        EXPECT_EQ(m(3, 0), 0); EXPECT_EQ(m(3, 1), 0); EXPECT_EQ(m(3, 2), 0); EXPECT_EQ(m(3, 3), 0);

        EXPECT_EQ(r(0, 0), 0); EXPECT_EQ(r(0, 1), 0); EXPECT_EQ(r(0, 2), 0); EXPECT_EQ(r(0, 3), 0);
        EXPECT_EQ(r(1, 0), 0); EXPECT_EQ(r(1, 1), 0); EXPECT_EQ(r(1, 2), 0); EXPECT_EQ(r(1, 3), 0);
        EXPECT_EQ(r(2, 0), 0); EXPECT_EQ(r(2, 1), 0); EXPECT_EQ(r(2, 2), 0); EXPECT_EQ(r(2, 3), 0);
        EXPECT_EQ(r(3, 0), 0); EXPECT_EQ(r(3, 1), 0); EXPECT_EQ(r(3, 2), 0); EXPECT_EQ(r(3, 3), 0);
        // clang-format on

        EXPECT_EQ(&r, &m);
    }
}

// ============================================
//              Multiplication
// ============================================

TEST(Mat4, mul_by_scalar) {
    const Mat4 matrix(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
    // operator*(scalar,Mat4)
    {
        const Mat4 r = 2 * matrix;
        // clang-format off
        EXPECT_EQ(r(0, 0),  2); EXPECT_EQ(r(0, 1),  4); EXPECT_EQ(r(0, 2),  6); EXPECT_EQ(r(0, 3),  8);
        EXPECT_EQ(r(1, 0), 10); EXPECT_EQ(r(1, 1), 12); EXPECT_EQ(r(1, 2), 14); EXPECT_EQ(r(1, 3), 16);
        EXPECT_EQ(r(2, 0), 18); EXPECT_EQ(r(2, 1), 20); EXPECT_EQ(r(2, 2), 22); EXPECT_EQ(r(2, 3), 24);
        EXPECT_EQ(r(3, 0), 26); EXPECT_EQ(r(3, 1), 28); EXPECT_EQ(r(3, 2), 30); EXPECT_EQ(r(3, 3), 32);
        // clang-format on
    }
    // operator*(Mat4,scalar)
    {
        const Mat4 r = matrix * 2;

        // clang-format off
        EXPECT_EQ(r(0, 0),  2); EXPECT_EQ(r(0, 1),  4); EXPECT_EQ(r(0, 2),  6); EXPECT_EQ(r(0, 3),  8);
        EXPECT_EQ(r(1, 0), 10); EXPECT_EQ(r(1, 1), 12); EXPECT_EQ(r(1, 2), 14); EXPECT_EQ(r(1, 3), 16);
        EXPECT_EQ(r(2, 0), 18); EXPECT_EQ(r(2, 1), 20); EXPECT_EQ(r(2, 2), 22); EXPECT_EQ(r(2, 3), 24);
        EXPECT_EQ(r(3, 0), 26); EXPECT_EQ(r(3, 1), 28); EXPECT_EQ(r(3, 2), 30); EXPECT_EQ(r(3, 3), 32);
        // clang-format on
    }
    // operator*=(Mat4,scalar)
    {
        Mat4        m(matrix);
        const Mat4& r = m *= 2;

        // clang-format off
        EXPECT_EQ(m(0, 0),  2); EXPECT_EQ(m(0, 1),  4);  EXPECT_EQ(m(0, 2),  6); EXPECT_EQ(m(0, 3),  8);
        EXPECT_EQ(m(1, 0), 10); EXPECT_EQ(m(1, 1), 12);  EXPECT_EQ(m(1, 2), 14); EXPECT_EQ(m(1, 3), 16);
        EXPECT_EQ(m(2, 0), 18); EXPECT_EQ(m(2, 1), 20);  EXPECT_EQ(m(2, 2), 22); EXPECT_EQ(m(2, 3), 24);
        EXPECT_EQ(m(3, 0), 26); EXPECT_EQ(m(3, 1), 28);  EXPECT_EQ(m(3, 2), 30); EXPECT_EQ(m(3, 3), 32);

        EXPECT_EQ(r(0, 0),  2); EXPECT_EQ(r(0, 1),  4); EXPECT_EQ(r(0, 2),  6); EXPECT_EQ(r(0, 3),  8);
        EXPECT_EQ(r(1, 0), 10); EXPECT_EQ(r(1, 1), 12); EXPECT_EQ(r(1, 2), 14); EXPECT_EQ(r(1, 3), 16);
        EXPECT_EQ(r(2, 0), 18); EXPECT_EQ(r(2, 1), 20); EXPECT_EQ(r(2, 2), 22); EXPECT_EQ(r(2, 3), 24);
        EXPECT_EQ(r(3, 0), 26); EXPECT_EQ(r(3, 1), 28); EXPECT_EQ(r(3, 2), 30); EXPECT_EQ(r(3, 3), 32);
        // clang-format on

        EXPECT_EQ(&r, &m);
    }
}

TEST(Mat4, mul_by_vec4) {
    const Mat4 matrix(
      // clang-format off
         1,  2,  3,  4,
         5,  6,  7,  8,
         9, 10, 11, 12,
        13, 14, 15, 16
      // clang-format on
    );
    // Test col vector multiplication
    EXPECT_EQ(matrix * Vec4(10, 20, 30, 40), Vec4(300, 700, 1100, 1500));

    // Test row vector multiplication
    EXPECT_EQ(Vec4(10, 20, 30, 40) * matrix, Vec4(900, 1000, 1100, 1200));
}

TEST(Mat4, mul_by_mat4) {
    //
    // 1  2  3  4      3  4  7  1    105 131 133 70
    // 5  6  7  8   X  9  12 2  7 =  245 303 297 170
    // 9  10 11 12     8  5  6 13    385 475 461 270
    // 13 14 15 16     15 22 26 4    525 647 625 370
    //
    const Mat4 matrix1(
      // clang-format off
         1,  2,  3,  4,
         5,  6,  7,  8,
         9, 10, 11, 12,
        13, 14, 15, 16
      // clang-format on
    );
    const Mat4 matrix2(
      // clang-format off
        3,   4,  7,  1,
        9,  12,  2,  7,
        8,   5,  6, 13,
        15, 22, 26,  4
      // clang-format on
    );
    // operator*(Mat4,Mat4)
    {
        const Mat4 result = matrix1 * matrix2;
        // clang-format off
        EXPECT_EQ(result(0, 0), 105); EXPECT_EQ(result(0, 1), 131); EXPECT_EQ(result(0, 2), 133); EXPECT_EQ(result(0, 3), 70);
        EXPECT_EQ(result(1, 0), 245); EXPECT_EQ(result(1, 1), 303); EXPECT_EQ(result(1, 2), 297); EXPECT_EQ(result(1, 3), 170);
        EXPECT_EQ(result(2, 0), 385); EXPECT_EQ(result(2, 1), 475); EXPECT_EQ(result(2, 2), 461); EXPECT_EQ(result(2, 3), 270);
        EXPECT_EQ(result(3, 0), 525); EXPECT_EQ(result(3, 1), 647); EXPECT_EQ(result(3, 2), 625); EXPECT_EQ(result(3, 3), 370);
        // clang-format on
    }
    // operator*=(Mat4,Mat4)
    {
        Mat4        m(matrix1);
        const Mat4& result = m *= matrix2;
        // clang-format off
        EXPECT_EQ(m(0, 0), 105); EXPECT_EQ(m(0, 1), 131); EXPECT_EQ(m(0, 2), 133); EXPECT_EQ(m(0, 3), 70);
        EXPECT_EQ(m(1, 0), 245); EXPECT_EQ(m(1, 1), 303); EXPECT_EQ(m(1, 2), 297); EXPECT_EQ(m(1, 3), 170);
        EXPECT_EQ(m(2, 0), 385); EXPECT_EQ(m(2, 1), 475); EXPECT_EQ(m(2, 2), 461); EXPECT_EQ(m(2, 3), 270);
        EXPECT_EQ(m(3, 0), 525); EXPECT_EQ(m(3, 1), 647); EXPECT_EQ(m(3, 2), 625); EXPECT_EQ(m(3, 3), 370);

        EXPECT_EQ(result(0, 0), 105); EXPECT_EQ(result(0, 1), 131); EXPECT_EQ(result(0, 2), 133); EXPECT_EQ(result(0, 3), 70);
        EXPECT_EQ(result(1, 0), 245); EXPECT_EQ(result(1, 1), 303); EXPECT_EQ(result(1, 2), 297); EXPECT_EQ(result(1, 3), 170);
        EXPECT_EQ(result(2, 0), 385); EXPECT_EQ(result(2, 1), 475); EXPECT_EQ(result(2, 2), 461); EXPECT_EQ(result(2, 3), 270);
        EXPECT_EQ(result(3, 0), 525); EXPECT_EQ(result(3, 1), 647); EXPECT_EQ(result(3, 2), 625); EXPECT_EQ(result(3, 3), 370);
        // clang-format on

        EXPECT_EQ(&result, &m);
    }
}

TEST(Mat4, determinant) {
    EXPECT_EQ(Mat4::kZero.determinant(), 0.0f);
    EXPECT_EQ(Mat4::kIdentity.determinant(), 1.0f);

    const Mat4 matrix1(
      // clang-format off
         1,  2,  3,  4,
         5,  6,  7,  8,
         9, 10, 11, 12,
        13, 14, 15, 16
      // clang-format on
    );
    EXPECT_EQ(matrix1.determinant(), 0.0f);

    const Mat4 matrix2(
      // clang-format off
         2,  2,  3,  4,
         5,  6,  7,  8,
         9,  3, 11, 12,
        13, 14, 15, 16
      // clang-format on
    );
    EXPECT_EQ(matrix2.determinant(), -56.f);

    const Mat4 matrix3(
      // clang-format off
         2,  1,  2,  3,
         4,  5,  6,  7,
         8,  3, 10, 11,
        12, 13, 14, 15
      // clang-format on
    );
    EXPECT_EQ(matrix3.determinant(), -96.f);
}

TEST(Mat4, inverse) {
    {
        // | 2  2  3  4|      |  56 -84  0  28|
        // | 5  6  7  8|   => |   0   4 -8   4| * (1 / 56)
        // | 9  3 11 12|      |-168 132 16 -36|
        // |13 14 15   |      | 112 -59 -8  11|
        const auto matrix = Mat4(2, 2, 3, 4, 5, 6, 7, 8, 9, 3, 11, 12, 13, 14, 15, 16);
        const auto inverse =
          Mat4(56, -84, 0, 28, 0, 4, -8, 4, -168, 132, 16, -36, 112, -59, -8, 11) * (1 / 56.f);
        EXPECT_EQ(matrix.determinant(), -56.f);
        EXPECT_EQ(matrix.inverse(), inverse);
    }
    {
        //  | 2  1  2  3|    | 12 -18  0  6|
        //  | 4  5  6  7| => |  0   2 -4  2| * (1/24)
        //  | 8  3 10 11|    |-36   5  8 -1|
        //  |12 13 14 15|    | 24   8 -4 -4|
        const Mat4 matrix(2, 1, 2, 3, 4, 5, 6, 7, 8, 3, 10, 11, 12, 13, 14, 15);
        const auto inverse =
          Mat4(12, -18, 0, 6, 0, 2, -4, 2, -36, 5, 8, -1, 24, 8, -4, -4) * (1 / 24.f);
        EXPECT_EQ(matrix.determinant(), -96.f);
        EXPECT_EQ(matrix.inverse(), inverse);
    }
}

TEST(Mat4, transpose) {
    const Mat4 m(
      // clang-format off
       1,  2,  3,  4,
       5,  6,  7,  8,
       9, 10, 11, 12,
      13, 14, 15, 16
      // clang-format on
    );
    const Mat4 t(
      // clang-format off
      1, 5,  9, 13,
      2, 6, 10, 14,
      3, 7, 11, 15,
      4, 8, 12, 16
      // clang-format on
    );
    EXPECT_EQ(m.transpose(), t);
}

/// ===================================================
///                 Transform functions
/// ===================================================

TEST(Mat4, createTranslation) {
    EXPECT_EQ(Mat4::CreateTranslation({1, 0, 0}) * Vec4(1, 1, 1, 1), Vec4(2, 1, 1, 1));
    EXPECT_EQ(Mat4::CreateTranslation({0, 1, 0}) * Vec4(1, 1, 1, 1), Vec4(1, 2, 1, 1));
    EXPECT_EQ(Mat4::CreateTranslation({0, 0, 1}) * Vec4(1, 1, 1, 1), Vec4(1, 1, 2, 1));
}

TEST(Mat4, createScaling) {
    // scale on x,y,z axis
    EXPECT_EQ(Mat4::CreateScaling({2, 0, 0}) * Vec4(1, 1, 1, 1), Vec4(2, 0, 0, 1));
    EXPECT_EQ(Mat4::CreateScaling({0, 2, 0}) * Vec4(1, 1, 1, 1), Vec4(0, 2, 0, 1));
    EXPECT_EQ(Mat4::CreateScaling({0, 0, 2}) * Vec4(1, 1, 1, 1), Vec4(0, 0, 2, 1));

    // scale axis
    EXPECT_EQ(Mat4::CreateScaling(2, Vec3::kUnitX), Mat4::CreateScaling({2, 1, 1}));
    EXPECT_EQ(Mat4::CreateScaling(2, Vec3::kUnitY), Mat4::CreateScaling({1, 2, 1}));
    EXPECT_EQ(Mat4::CreateScaling(2, Vec3::kUnitZ), Mat4::CreateScaling({1, 1, 2}));
}

// Test the CreateRotationX function (rotation around X-axis)
TEST(Mat4, CreateRotationX) {
    // rotate [0,0,1,0] by 90 degrees => [0,-1,0,0]
    {
        const auto r = Mat4::CreateRotationX(fuse::degrees(90.f)) * Vec4::kUnitZ;
        EXPECT_FLOAT_EQ(r.x, 0.0f);
        EXPECT_FLOAT_EQ(r.y, -1.0f);
        EXPECT_NEAR(r.z, 0.0f, std::numeric_limits<float>::epsilon());
        EXPECT_FLOAT_EQ(r.w, 0.0f);
    }
    // rotate [0, 0,-1, 1] by 90 degrees => [0, 1, 0, 1]
    {
        const auto result = Mat4::CreateRotationX(fuse::degrees(90.f)) * Vec4(0, 0, -1, 1);
        EXPECT_FLOAT_EQ(result.x, 0.0f);
        EXPECT_FLOAT_EQ(result.y, 1.0f);
        EXPECT_NEAR(result.z, 0.0f, std::numeric_limits<float>::epsilon());
        EXPECT_FLOAT_EQ(result.w, 1.0f);
    }

    // rotate [0,1,0,5] by 90 degrees => [0,0,1,5]
    {
        const auto result = Mat4::CreateRotationX(fuse::degrees(90.f)) * Vec4(0, 1, 0, 5);
        EXPECT_FLOAT_EQ(result.x, 0.0f);
        EXPECT_NEAR(result.y, 0.0f, std::numeric_limits<float>::epsilon());
        EXPECT_FLOAT_EQ(result.z, 1.0f);
        EXPECT_FLOAT_EQ(result.w, 5.0f);
    }
}

// Test the CreateRotationY function (rotation around y-axis)
TEST(Mat4, CreateRotationY) {
    // rotate [0, 0, -1, 5] by 90 degrees => [-1,0,0,5]
    {
        const auto result = Mat4::CreateRotationY(fuse::degrees(90.f)) * Vec4(0, 0, -1, 5);
        EXPECT_FLOAT_EQ(result.x, -1.0f);
        EXPECT_FLOAT_EQ(result.y, 0.0f);
        EXPECT_NEAR(result.z, 0.0f, std::numeric_limits<float>::epsilon());
        EXPECT_FLOAT_EQ(result.w, 5.0f);
    }
    // rotate [1, 0, 0, 0] by 90 degrees => [0,0,-1,0]
    {
        const auto result = Mat4::CreateRotationY(fuse::degrees(90.f)) * Vec4(1, 0, 0, 0);
        EXPECT_NEAR(result.x, 0.0f, std::numeric_limits<float>::epsilon());
        EXPECT_FLOAT_EQ(result.y, 0.0f);
        EXPECT_FLOAT_EQ(result.z, -1.0f);
        EXPECT_FLOAT_EQ(result.w, 0.0f);
    }
}

// Test the CreateRotationZ function (rotation around z-axis)
TEST(Mat4, CreateRotationZ) {
    // rotate [1, 0, 0, 2] by 90 degrees => [0,1,0,2]
    {
        const auto result = Mat4::CreateRotationZ(fuse::degrees(90.f)) * Vec4(1, 0, 0, 2);
        EXPECT_NEAR(result.x, 0.0f, std::numeric_limits<float>::epsilon());
        EXPECT_FLOAT_EQ(result.y, 1.0f);
        EXPECT_FLOAT_EQ(result.z, 0.0f);
        EXPECT_FLOAT_EQ(result.w, 2.0f);
    }
    // rotate [0, 1, 0, 0] by 90 degrees => [-1,0,0,0]
    {
        const auto result = Mat4::CreateRotationZ(fuse::degrees(90.f)) * Vec4(0, 1, 0, 0);
        EXPECT_FLOAT_EQ(result.x, -1.0f);
        EXPECT_NEAR(result.y, 0.0f, std::numeric_limits<float>::epsilon());
        EXPECT_FLOAT_EQ(result.z, 0.0f);
        EXPECT_FLOAT_EQ(result.w, 0.0f);
    }
}

TEST(Mat4, CreateRotation_angleAxis) {
    //
    // Test with a angle around x-axis
    // The rotation matrix must match the one create by CreateRotationX()
    //
    {
        const auto angle  = fuse::degrees(45);
        const auto result = Mat4::CreateRotationX(angle);
        const auto rot    = Mat4::CreateRotation(angle, Vec3::kUnitX);
        EXPECT_EQ(rot, result);
    }

    //
    // Test with a angle around y-axis
    // The rotation matrix must match the one create by CreateRotationY()
    //
    {
        const auto angle  = fuse::degrees(45);
        const auto result = Mat4::CreateRotationY(angle);
        const auto rot    = Mat4::CreateRotation(angle, Vec3::kUnitY);
        EXPECT_EQ(rot, result);
    }

    //
    // Test with a angle around Z-axis
    // The rotation matrix must match the one create by CreateRotationZ()
    //
    {
        const auto angle  = fuse::degrees(45);
        const auto result = Mat4::CreateRotationZ(angle);
        const auto rot    = Mat4::CreateRotation(angle, Vec3::kUnitZ);
        EXPECT_EQ(rot, result);
    }
}

TEST(Mat4, CreateReflection) {
    // reflect around xz plane (normal = Y)
    {
        const auto reflection = Mat4::CreateReflection(Vec3::kUnitY);
        EXPECT_EQ(reflection * Vec4(5, 5, 5, 0), Vec4(5, -5, 5, 0));
    }
    // reflect around yz plane (normal = X)
    {
        const auto reflection = Mat4::CreateReflection(Vec3::kUnitX);
        EXPECT_EQ(reflection * Vec4(5, 5, 5, 0), Vec4(-5, 5, 5, 0));
    }
    // reflect around xy plane (normal = Z)
    {
        const auto reflection = Mat4::CreateReflection(Vec3::kUnitZ);
        EXPECT_EQ(reflection * Vec4(5, 5, 5, 0), Vec4(5, 5, -5, 0));
    }
}

/// ===================================================
///                 View
/// ===================================================
TEST(Mat4, CreateViewLookAt) {
    // position (0, 0, 0)
    // target   (0, 0,-1)
    // up       (0, 1, 0)
    {
        const auto view = Mat4::CreateViewLookAt(Vec3::kZero, Vec3::kUnitZNeg, Vec3::kUnitY);
        EXPECT_EQ(view, Mat4::kIdentity);
    }
    // move the camera.... but keel looking down the -z to avoid rotation
    // position (5, 1,  3)
    // target   (5, 1, -3)
    // up       (0, 1,  0)
    {
        const auto view  = Mat4::CreateViewLookAt(Vec3(5, 1, 3), Vec3(5, 1, -3), Vec3::kUnitY);
        const Vec3 viewX = Vec3(view(0, 0), view(0, 1), view(0, 2));
        const Vec3 viewY = Vec3(view(1, 0), view(1, 1), view(1, 2));
        const Vec3 viewZ = Vec3(view(2, 0), view(2, 1), view(2, 2));
        const Vec3 pos   = Vec3(view(0, 3), view(1, 3), view(2, 3));
        EXPECT_EQ(viewX, Vec3::kUnitX);
        EXPECT_EQ(viewY, Vec3::kUnitY);
        EXPECT_EQ(viewZ, Vec3::kUnitZ);
        EXPECT_EQ(pos, Vec3(-5, -1, -3));
    }

    // position ( 0, 0, 0)
    // target   (-1, 0, 0) (rotation 90 degrees around Y)
    // up       ( 0, 1, 0)
    {
        const auto view  = Mat4::CreateViewLookAt(Vec3::kZero, Vec3::kUnitXNeg, Vec3::kUnitY);
        const Vec3 viewX = Vec3(view(0, 0), view(0, 1), view(0, 2));
        const Vec3 viewY = Vec3(view(1, 0), view(1, 1), view(1, 2));
        const Vec3 viewZ = Vec3(view(2, 0), view(2, 1), view(2, 2));
        EXPECT_EQ(viewX, Vec3::kUnitZNeg);
        EXPECT_EQ(viewY, Vec3::kUnitY);
        EXPECT_EQ(viewZ, Vec3::kUnitX);
    }

    // position ( 5, 1, 3)
    // target   (-1, 0, 0) (rotation 90 degrees around Y)
    // up       ( 0, 1, 0)
    {
        const auto view  = Mat4::CreateViewLookAt(Vec3(5, 0, 0), Vec3::kUnitXNeg, Vec3::kUnitY);
        const Vec3 viewX = Vec3(view(0, 0), view(0, 1), view(0, 2));
        const Vec3 viewY = Vec3(view(1, 0), view(1, 1), view(1, 2));
        const Vec3 viewZ = Vec3(view(2, 0), view(2, 1), view(2, 2));
        const Vec3 pos   = Vec3(view(0, 3), view(1, 3), view(2, 3));
        EXPECT_EQ(viewX, Vec3::kUnitZNeg);
        EXPECT_EQ(viewY, Vec3::kUnitY);
        EXPECT_EQ(viewZ, Vec3::kUnitX);
        EXPECT_EQ(pos, Vec3(0, 0, -5));
    }
}

TEST(Mat4, CreateViewLookTo) {
    // position  (0, 0, 0)
    // direction (0, 0,-1)
    // up        (0, 1, 0)
    {
        const auto view = Mat4::CreateViewLookTo(Vec3::kZero, Vec3::kUnitZNeg, Vec3::kUnitY);
        EXPECT_EQ(view, Mat4::kIdentity);
    }
    // move the camera.... but keel looking down the -z to avoid rotation
    // position  (5, 1, 3)
    // direction (0, 0,-1)
    // up        (0, 1, 0)
    {
        const auto view  = Mat4::CreateViewLookTo(Vec3(5, 1, 3), Vec3::kUnitZNeg, Vec3::kUnitY);
        const Vec3 viewX = Vec3(view(0, 0), view(0, 1), view(0, 2));
        const Vec3 viewY = Vec3(view(1, 0), view(1, 1), view(1, 2));
        const Vec3 viewZ = Vec3(view(2, 0), view(2, 1), view(2, 2));
        const Vec3 pos   = Vec3(view(0, 3), view(1, 3), view(2, 3));
        EXPECT_EQ(viewX, Vec3::kUnitX);
        EXPECT_EQ(viewY, Vec3::kUnitY);
        EXPECT_EQ(viewZ, Vec3::kUnitZ);
        EXPECT_EQ(pos, Vec3(-5, -1, -3));
    }

    // position ( 0, 0, 0)
    // target   (-1, 0, 0) (rotation 90 degrees around Y)
    // up       ( 0, 1, 0)
    {
        const auto view  = Mat4::CreateViewLookTo(Vec3::kZero, Vec3::kUnitXNeg, Vec3::kUnitY);
        const Vec3 viewX = Vec3(view(0, 0), view(0, 1), view(0, 2));
        const Vec3 viewY = Vec3(view(1, 0), view(1, 1), view(1, 2));
        const Vec3 viewZ = Vec3(view(2, 0), view(2, 1), view(2, 2));
        EXPECT_EQ(viewX, Vec3::kUnitZNeg);
        EXPECT_EQ(viewY, Vec3::kUnitY);
        EXPECT_EQ(viewZ, Vec3::kUnitX);
    }

    // position ( 5, 1, 3)
    // target   (-2, 0, 0) (rotation 90 degrees around Y)
    // up       ( 0, 1, 0)
    {
        const auto view  = Mat4::CreateViewLookTo(Vec3(5, 0, 0), Vec3::kUnitXNeg * 2, Vec3::kUnitY);
        const Vec3 viewX = Vec3(view(0, 0), view(0, 1), view(0, 2));
        const Vec3 viewY = Vec3(view(1, 0), view(1, 1), view(1, 2));
        const Vec3 viewZ = Vec3(view(2, 0), view(2, 1), view(2, 2));
        const Vec3 pos   = Vec3(view(0, 3), view(1, 3), view(2, 3));
        EXPECT_EQ(viewX, Vec3::kUnitZNeg);
        EXPECT_EQ(viewY, Vec3::kUnitY);
        EXPECT_EQ(viewZ, Vec3::kUnitX);
        EXPECT_EQ(pos, Vec3(0, 0, -5));
    }
}

/// ===================================================
///                 Projection
/// ===================================================
TEST(Mat4, ProjOrtho) {
    ///
    /// @todo Test ProjOrtho
    ///
}

TEST(Mat4, ProjOrthoOffCenter) {
    // view volume unit cube x (-1, 1) y (-1, 1) z (-1, 1)
    // should be a no operation except flipping Z because the matrix use OpenGL
    // standard (+Z pointing into the screen).
    {
        const auto proj = Mat4::ProjOrthoOffCenter(-1, 1, -1, 1, -1, 1);
        // clang-format off
        EXPECT_THAT(proj * Vec4( 0,  0,  0, 1), Eq(Vec4( 0,  0,  0.f, 1.f)));
        EXPECT_THAT(proj * Vec4( 1,  0,  0, 1), Eq(Vec4( 1,  0,  0.f, 1.f)));
        EXPECT_THAT(proj * Vec4(-1,  0,  0, 1), Eq(Vec4(-1,  0,  0.f, 1.f)));
        EXPECT_THAT(proj * Vec4( 0,  1,  0, 1), Eq(Vec4( 0,  1,  0.f, 1.f)));
        EXPECT_THAT(proj * Vec4( 0, -1,  0, 1), Eq(Vec4( 0, -1,  0.f, 1.f)));
        EXPECT_THAT(proj * Vec4( 0,  0,  1, 1), Eq(Vec4( 0,  0, -1.f, 1.f)));
        EXPECT_THAT(proj * Vec4( 0,  0, -1, 1), Eq(Vec4( 0,  0,  1.f, 1.f)));
        // clang-format on
    }
    // view volume x (0, 10) y (0, 5) z (0, 8)
    // standard (+Z pointing into the screen).
    {
        // clang-format off
        const auto proj = Mat4::ProjOrthoOffCenter(0, 10, 0, 5, 0, 4);
        EXPECT_THAT(proj * Vec4( 0, 0, 0, 1), Eq(Vec4(-1, -1, -1.f,  1.f)));
        EXPECT_THAT(proj * Vec4( 5, 0, 0, 1), Eq(Vec4( 0, -1, -1.f,  1.f)));
        EXPECT_THAT(proj * Vec4(10, 0, 0, 1), Eq(Vec4( 1, -1, -1.f,  1.f)));
        EXPECT_THAT(proj * Vec4(11, 0, 0, 1), Eq(Vec4(1.2f,-1, -1.f,  1.f)));
        EXPECT_THAT(proj * Vec4( 0, 5, 0, 1), Eq(Vec4(-1,  1, -1.f,  1.f)));
        EXPECT_THAT(proj * Vec4( 0, 0,-2, 1), Eq(Vec4(-1, -1,  0.0f, 1.f)));
        EXPECT_THAT(proj * Vec4( 0, 0,-3, 1), Eq(Vec4(-1, -1,  0.5f, 1.f)));
        EXPECT_THAT(proj * Vec4( 0, 0,-4, 1), Eq(Vec4(-1, -1,  1.f,  1.f)));
        EXPECT_THAT(proj * Vec4( 0, 0,-5, 1), Eq(Vec4(-1, -1,  1.5f, 1.f)));
        EXPECT_THAT(proj * Vec4( 0, 0,-8, 1), Eq(Vec4(-1, -1,  3.f,  1.f)));
        // clang-format on
    }
}

TEST(Mat4, CreateProjectionPerspectiveOffCenter) {
    ///
    /// @todo Test CreateProjectionPerspectiveOffCenter
    ///
}

TEST(Mat4, CreateProjectionPerspectiveFOVX) {
    ///
    /// @todo Test CreateProjectionPerspectiveFOVX
    ///
}

TEST(Mat4, CreateProjectionPerspectiveFOVY) {
    ///
    /// @todo Test CreateProjectionPerspectiveFOVY
    ///
}
