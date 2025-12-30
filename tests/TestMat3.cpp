#include <gtest/gtest.h>
#include <math/Mat3.h>


using namespace fuse;
using namespace testing;

TEST(Mat3, ctor_by_elements) {
    // clang-format off
    const Mat3 m(
        0, 1, 2, // row 0
        3, 4, 5, // row 1
        6, 7, 8  // row 2
    );

    auto* a = m.data();
    EXPECT_EQ(a[0],  0); EXPECT_EQ(a[3], 1); EXPECT_EQ(a[6], 2);
    EXPECT_EQ(a[1],  3); EXPECT_EQ(a[4], 4); EXPECT_EQ(a[7], 5);
    EXPECT_EQ(a[2],  6); EXPECT_EQ(a[5], 7); EXPECT_EQ(a[8], 8);

    EXPECT_EQ(m(0,0), 0); EXPECT_EQ(m(0,1), 1); EXPECT_EQ(m(0,2), 2);
    EXPECT_EQ(m(1,0), 3); EXPECT_EQ(m(1,1), 4); EXPECT_EQ(m(1,2), 5);
    EXPECT_EQ(m(2,0), 6); EXPECT_EQ(m(2,1), 7); EXPECT_EQ(m(2,2), 8);
    // clang-format on
}

TEST(Mat4, ctor_by_vec3) {
    // clang-format off
    const Mat3 m(
        Vec3(0, 3, 6), // col 0
        Vec3(1, 4, 7), // col 1
        Vec3(2, 5, 8)  // col 2
    );

    EXPECT_EQ(m(0,0), 0);  EXPECT_EQ(m(0,1), 1);  EXPECT_EQ(m(0,2), 2); // row 0
    EXPECT_EQ(m(1,0), 3);  EXPECT_EQ(m(1,1), 4);  EXPECT_EQ(m(1,2), 5); // row 1
    EXPECT_EQ(m(2,0), 6);  EXPECT_EQ(m(2,1), 7);  EXPECT_EQ(m(2,2), 8); // row 2
    // clang-format on
}

// ============================================
//                  Addition
// ============================================
TEST(Mat3, addition) {
    const Mat3 matrix(0, 1, 2, 3, 4, 5, 6, 7, 8);

    {
        const Mat3 r = matrix + matrix;

        // clang-format off
        EXPECT_EQ(r(0,0),  0); EXPECT_EQ(r(0,1),  2); EXPECT_EQ(r(0,2),  4); // row 0
        EXPECT_EQ(r(1,0),  6); EXPECT_EQ(r(1,1),  8); EXPECT_EQ(r(1,2), 10); // row 1
        EXPECT_EQ(r(2,0), 12); EXPECT_EQ(r(2,1), 14); EXPECT_EQ(r(2,2), 16); // row 2
        // clang-format on
    }

    {
        Mat3       m(matrix);
        const Mat3 r = m += m;

        // clang-format off
        EXPECT_EQ(m(0,0),  0); EXPECT_EQ(m(0,1),  2); EXPECT_EQ(m(0,2),  4); // mow 0
        EXPECT_EQ(m(1,0),  6); EXPECT_EQ(m(1,1),  8); EXPECT_EQ(m(1,2), 10); // mow 1
        EXPECT_EQ(m(2,0), 12); EXPECT_EQ(m(2,1), 14); EXPECT_EQ(m(2,2), 16); // mow 2

        EXPECT_EQ(r(0,0),  0); EXPECT_EQ(r(0,1),  2); EXPECT_EQ(r(0,2),  4); // row 0
        EXPECT_EQ(r(1,0),  6); EXPECT_EQ(r(1,1),  8); EXPECT_EQ(r(1,2), 10); // row 1
        EXPECT_EQ(r(2,0), 12); EXPECT_EQ(r(2,1), 14); EXPECT_EQ(r(2,2), 16); // row 2
        // clang-format on
    }
}

// ============================================
//                  Substation
// ============================================
TEST(Mat3, substract) {
    const Mat3 matrix(0, 1, 2, 3, 4, 5, 6, 7, 8);
    {
        const Mat3 r = matrix - matrix;

        // clang-format off
        EXPECT_EQ(r(0,0), 0);  EXPECT_EQ(r(0,1), 0); EXPECT_EQ(r(0,2), 0); // row 0
        EXPECT_EQ(r(1,0), 0);  EXPECT_EQ(r(1,1), 0); EXPECT_EQ(r(1,2), 0); // row 1
        EXPECT_EQ(r(2,0), 0);  EXPECT_EQ(r(2,1), 0); EXPECT_EQ(r(2,2), 0); // row 2
        // clang-format on
    }

    {
        Mat3       m(matrix);
        const Mat3 r = m -= m;

        // clang-format off
        EXPECT_EQ(m(0,0), 0);  EXPECT_EQ(m(0,1), 0); EXPECT_EQ(m(0,2), 0); // mow 0
        EXPECT_EQ(m(1,0), 0);  EXPECT_EQ(m(1,1), 0); EXPECT_EQ(m(1,2), 0); // mow 1
        EXPECT_EQ(m(2,0), 0);  EXPECT_EQ(m(2,1), 0); EXPECT_EQ(m(2,2), 0); // mow 2

        EXPECT_EQ(r(0,0), 0);  EXPECT_EQ(r(0,1), 0); EXPECT_EQ(r(0,2), 0); // row 0
        EXPECT_EQ(r(1,0), 0);  EXPECT_EQ(r(1,1), 0); EXPECT_EQ(r(1,2), 0); // row 1
        EXPECT_EQ(r(2,0), 0);  EXPECT_EQ(r(2,1), 0); EXPECT_EQ(r(2,2), 0); // row 2
        // clang-format on
    }
}

// ============================================
//              Multiplication
// ============================================

TEST(Mat3, mul_by_scalar) {
    const Mat3 matrix(1, 2, 3, 4, 5, 6, 7, 8, 9);

    {
        const Mat3 r = matrix * 2;

        // clang-format off
        EXPECT_EQ(r(0, 0),  2); EXPECT_EQ(r(0, 1),  4); EXPECT_EQ(r(0, 2),  6);
        EXPECT_EQ(r(1, 0),  8); EXPECT_EQ(r(1, 1), 10); EXPECT_EQ(r(1, 2), 12);
        EXPECT_EQ(r(2, 0), 14); EXPECT_EQ(r(2, 1), 16); EXPECT_EQ(r(2, 2), 18);
        // clang-format on

        const Mat3 r2 = 2 * matrix;
        // clang-format off
        EXPECT_EQ(r2(0, 0),  2); EXPECT_EQ(r2(0, 1),  4); EXPECT_EQ(r2(0, 2),  6);
        EXPECT_EQ(r2(1, 0),  8); EXPECT_EQ(r2(1, 1), 10); EXPECT_EQ(r2(1, 2), 12);
        EXPECT_EQ(r2(2, 0), 14); EXPECT_EQ(r2(2, 1), 16); EXPECT_EQ(r2(2, 2), 18);
        // clang-format on
    }
    {
        Mat3       m(matrix);
        const Mat3 r = m *= 2;

        // clang-format off
        EXPECT_EQ(m(0, 0),  2); EXPECT_EQ(m(0, 1),  4); EXPECT_EQ(m(0, 2),  6);
        EXPECT_EQ(m(1, 0),  8); EXPECT_EQ(m(1, 1), 10); EXPECT_EQ(m(1, 2), 12);
        EXPECT_EQ(m(2, 0), 14); EXPECT_EQ(m(2, 1), 16); EXPECT_EQ(m(2, 2), 18);

        EXPECT_EQ(r(0, 0),  2); EXPECT_EQ(r(0, 1),  4); EXPECT_EQ(r(0, 2),  6);
        EXPECT_EQ(r(1, 0),  8); EXPECT_EQ(r(1, 1), 10); EXPECT_EQ(r(1, 2), 12);
        EXPECT_EQ(r(2, 0), 14); EXPECT_EQ(r(2, 1), 16); EXPECT_EQ(r(2, 2), 18);
        // clang-format on
    }
}


TEST(Mat3, mul_by_vec3) {
    //
    // 1  2 3     10    140
    // 4  5 6  X  20 =  320
    // 7  8 9     30    500
    //
    const Mat3 matrix(
      // clang-format off
         1, 2, 3,
         4, 5, 6,
         7, 8, 9
      // clang-format on
    );

    const auto result = matrix * Vec3(10, 20, 30);
    EXPECT_EQ(result.x, 140);
    EXPECT_EQ(result.y, 320);
    EXPECT_EQ(result.z, 500);
}

TEST(Mat3, mul_by_mat3) {
    //
    // 1  2  3     3  4  7     45  43  29
    // 4  5  6  X  9  12 2 =  105 106  74
    // 7  8  9     8  5  6    165 169 119
    //
    const Mat3 matrix1(
      // clang-format off
        1, 2, 3,
        4, 5, 6,
        7, 8, 9
      // clang-format on
    );
    const Mat3 matrix2(
      // clang-format off
        3,   4,  7,
        9,  12,  2,
        8,   5,  6
      // clang-format on
    );

    {
        const auto result = matrix1 * matrix2;
        // clang-format off
        EXPECT_EQ(result(0, 0), 45);  EXPECT_EQ(result(0, 1), 43);  EXPECT_EQ(result(0, 2), 29);
        EXPECT_EQ(result(1, 0), 105); EXPECT_EQ(result(1, 1), 106); EXPECT_EQ(result(1, 2), 74);
        EXPECT_EQ(result(2, 0), 165); EXPECT_EQ(result(2, 1), 169); EXPECT_EQ(result(2, 2), 119);
        // clang-format on
    }

    {
        Mat3 m (matrix1);
        const auto  result = m *= matrix2;
        EXPECT_EQ(m(0, 0),  45); EXPECT_EQ(m(0, 1),  43); EXPECT_EQ(m(0, 2),  29);
        EXPECT_EQ(m(1, 0), 105); EXPECT_EQ(m(1, 1), 106); EXPECT_EQ(m(1, 2),  74);
        EXPECT_EQ(m(2, 0), 165); EXPECT_EQ(m(2, 1), 169); EXPECT_EQ(m(2, 2), 119);

        EXPECT_EQ(result(0, 0),  45); EXPECT_EQ(result(0, 1),  43); EXPECT_EQ(result(0, 2),  29);
        EXPECT_EQ(result(1, 0), 105); EXPECT_EQ(result(1, 1), 106); EXPECT_EQ(result(1, 2),  74);
        EXPECT_EQ(result(2, 0), 165); EXPECT_EQ(result(2, 1), 169); EXPECT_EQ(result(2, 2), 119);
    }
}
