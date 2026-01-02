#include "GTestUtils.h"

#include <gtest/gtest.h>
#include <math/Mat4.h>


using namespace fuse;
using namespace testing;

constexpr Mat4 zero4x4{
  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

constexpr Mat4 identity4x4{
  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

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

    {
        const Mat4 r = matrix + matrix;

        // clang-format off
        EXPECT_EQ(r(0, 0),  2); EXPECT_EQ(r(0, 1),  4); EXPECT_EQ(r(0, 2),  6); EXPECT_EQ(r(0, 3),  8);
        EXPECT_EQ(r(1, 0), 10); EXPECT_EQ(r(1, 1), 12); EXPECT_EQ(r(1, 2), 14); EXPECT_EQ(r(1, 3), 16);
        EXPECT_EQ(r(2, 0), 18); EXPECT_EQ(r(2, 1), 20); EXPECT_EQ(r(2, 2), 22); EXPECT_EQ(r(2, 3), 24);
        EXPECT_EQ(r(3, 0), 26); EXPECT_EQ(r(3, 1), 28); EXPECT_EQ(r(3, 2), 30); EXPECT_EQ(r(3, 3), 32);
        // clang-format on
    }
    {
        Mat4       m(matrix);
        const Mat4 r = m += m;

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
    }
}

// ============================================
//                  Substation
// ============================================
TEST(Mat4, substract) {
    const Mat4 matrix(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);

    {
        const Mat4 r = matrix - matrix;

        // clang-format off
        EXPECT_EQ(r(0, 0), 0); EXPECT_EQ(r(0, 1), 0); EXPECT_EQ(r(0, 2), 0); EXPECT_EQ(r(0, 3), 0);
        EXPECT_EQ(r(1, 0), 0); EXPECT_EQ(r(1, 1), 0); EXPECT_EQ(r(1, 2), 0); EXPECT_EQ(r(1, 3), 0);
        EXPECT_EQ(r(2, 0), 0); EXPECT_EQ(r(2, 1), 0); EXPECT_EQ(r(2, 2), 0); EXPECT_EQ(r(2, 3), 0);
        EXPECT_EQ(r(3, 0), 0); EXPECT_EQ(r(3, 1), 0); EXPECT_EQ(r(3, 2), 0); EXPECT_EQ(r(3, 3), 0);
        // clang-format on
    }

    {
        Mat4       m(matrix);
        const Mat4 r = m -= m;

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
    }
}

// ============================================
//              Multiplication
// ============================================

TEST(Mat4, mul_by_scalar) {
    const Mat4 matrix(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);

    {
        const Mat4 r = matrix * 2;

        // clang-format off
        EXPECT_EQ(r(0, 0),  2); EXPECT_EQ(r(0, 1),  4); EXPECT_EQ(r(0, 2),  6); EXPECT_EQ(r(0, 3),  8);
        EXPECT_EQ(r(1, 0), 10); EXPECT_EQ(r(1, 1), 12); EXPECT_EQ(r(1, 2), 14); EXPECT_EQ(r(1, 3), 16);
        EXPECT_EQ(r(2, 0), 18); EXPECT_EQ(r(2, 1), 20); EXPECT_EQ(r(2, 2), 22); EXPECT_EQ(r(2, 3), 24);
        EXPECT_EQ(r(3, 0), 26); EXPECT_EQ(r(3, 1), 28); EXPECT_EQ(r(3, 2), 30); EXPECT_EQ(r(3, 3), 32);
        // clang-format on

        const Mat4 r2 = 2 * matrix;
        // clang-format off
        EXPECT_EQ(r2(0, 0),  2); EXPECT_EQ(r2(0, 1),  4); EXPECT_EQ(r2(0, 2),  6); EXPECT_EQ(r2(0, 3),  8);
        EXPECT_EQ(r2(1, 0), 10); EXPECT_EQ(r2(1, 1), 12); EXPECT_EQ(r2(1, 2), 14); EXPECT_EQ(r2(1, 3), 16);
        EXPECT_EQ(r2(2, 0), 18); EXPECT_EQ(r2(2, 1), 20); EXPECT_EQ(r2(2, 2), 22); EXPECT_EQ(r2(2, 3), 24);
        EXPECT_EQ(r2(3, 0), 26); EXPECT_EQ(r2(3, 1), 28); EXPECT_EQ(r2(3, 2), 30); EXPECT_EQ(r2(3, 3), 32);
        // clang-format on
    }
    {
        Mat4       m(matrix);
        const Mat4 r = m *= 2;

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
    }
}

TEST(Mat4, mul_by_vec4) {
    //
    // 1  2  3  4      10    300
    // 5  6  7  8   X  20 =  700
    // 9  10 11 12     30    1100
    // 13 14 15 16     40    1500
    //
    // clang-format off
    const Mat4  matrix(
         1,  2,  3,  4,
         5,  6,  7,  8,
         9, 10, 11, 12,
        13, 14, 15, 16
    );

    const auto  result = matrix * Vec4(10, 20, 30, 40);
    EXPECT_EQ(result.x, 300);
    EXPECT_EQ(result.y, 700);
    EXPECT_EQ(result.z, 1100);
    EXPECT_EQ(result.w, 1500);
    // clang-format on
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

    {
        const Mat4 result = matrix1 * matrix2;
        // clang-format off
        EXPECT_EQ(result(0, 0), 105); EXPECT_EQ(result(0, 1), 131); EXPECT_EQ(result(0, 2), 133); EXPECT_EQ(result(0, 3), 70);
        EXPECT_EQ(result(1, 0), 245); EXPECT_EQ(result(1, 1), 303); EXPECT_EQ(result(1, 2), 297); EXPECT_EQ(result(1, 3), 170);
        EXPECT_EQ(result(2, 0), 385); EXPECT_EQ(result(2, 1), 475); EXPECT_EQ(result(2, 2), 461); EXPECT_EQ(result(2, 3), 270);
        EXPECT_EQ(result(3, 0), 525); EXPECT_EQ(result(3, 1), 647); EXPECT_EQ(result(3, 2), 625); EXPECT_EQ(result(3, 3), 370);
        // clang-format on
    }

    {
        Mat4       m(matrix1);
        const Mat4 result = m *= matrix2;
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
    }
}

TEST(Mat4, determinant) {
    EXPECT_EQ(zero4x4.determinant(), 0.0f);
    EXPECT_EQ(identity4x4.determinant(), 1.0f);

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
