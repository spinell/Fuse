#include "GTestUtils.h"

#include <math/Angle.h>
#include <math/Mat3.h>

#include <gtest/gtest.h>


using namespace fuse;
using namespace testing;

TEST(Mat3, traits) {
    static_assert(std::is_trivially_copy_constructible_v<Mat3>);
    static_assert(std::is_trivially_copy_assignable_v<Mat3>);
    static_assert(std::is_trivially_move_constructible_v<Mat3>);
    static_assert(std::is_trivially_move_assignable_v<Mat3>);
    static_assert(std::is_trivially_copyable_v<Mat3>);
}

TEST(Mat3, constants) {
    static_assert(Mat3::kZero == Mat3(0.F, 0.F, 0.F, 0.f, 0.F, 0.f, 0.F, 0.f, 0.f));
    static_assert(Mat3::kIdentity(0, 0) == 1.F);
    static_assert(Mat3::kIdentity(0, 1) == 0.F);
    static_assert(Mat3::kIdentity(0, 2) == 0.F);
    static_assert(Mat3::kIdentity(1, 0) == 0.F);
    static_assert(Mat3::kIdentity(1, 1) == 1.F);
    static_assert(Mat3::kIdentity(1, 2) == 0.F);
    static_assert(Mat3::kIdentity(2, 0) == 0.F);
    static_assert(Mat3::kIdentity(2, 1) == 0.F);
    static_assert(Mat3::kIdentity(2, 2) == 1.F);

    EXPECT_EQ(Mat3::kZero, Mat3(0.F, 0.F, 0.F, 0.f, 0.F, 0.f, 0.F, 0.f, 0.f));
    EXPECT_EQ(Mat3::kIdentity(0, 0), 1.F);
    EXPECT_EQ(Mat3::kIdentity(0, 1), 0.F);
    EXPECT_EQ(Mat3::kIdentity(0, 2), 0.F);
    EXPECT_EQ(Mat3::kIdentity(1, 0), 0.F);
    EXPECT_EQ(Mat3::kIdentity(1, 1), 1.F);
    EXPECT_EQ(Mat3::kIdentity(1, 2), 0.F);
    EXPECT_EQ(Mat3::kIdentity(2, 0), 0.F);
    EXPECT_EQ(Mat3::kIdentity(2, 1), 0.F);
    EXPECT_EQ(Mat3::kIdentity(2, 2), 1.F);
}

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

TEST(Mat3, ctor_by_vec3) {
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

    // operator+(Ma3,Ma3)
    {
        const Mat3 r = matrix + matrix;

        // clang-format off
        EXPECT_EQ(r(0,0),  0); EXPECT_EQ(r(0,1),  2); EXPECT_EQ(r(0,2),  4); // row 0
        EXPECT_EQ(r(1,0),  6); EXPECT_EQ(r(1,1),  8); EXPECT_EQ(r(1,2), 10); // row 1
        EXPECT_EQ(r(2,0), 12); EXPECT_EQ(r(2,1), 14); EXPECT_EQ(r(2,2), 16); // row 2
        // clang-format on
    }

    // operator+=(Ma3,Ma3)
    {
        Mat3        m(matrix);
        const Mat3& r = m += m;

        // clang-format off
        EXPECT_EQ(m(0,0),  0); EXPECT_EQ(m(0,1),  2); EXPECT_EQ(m(0,2),  4); // mow 0
        EXPECT_EQ(m(1,0),  6); EXPECT_EQ(m(1,1),  8); EXPECT_EQ(m(1,2), 10); // mow 1
        EXPECT_EQ(m(2,0), 12); EXPECT_EQ(m(2,1), 14); EXPECT_EQ(m(2,2), 16); // mow 2

        EXPECT_EQ(r(0,0),  0); EXPECT_EQ(r(0,1),  2); EXPECT_EQ(r(0,2),  4); // row 0
        EXPECT_EQ(r(1,0),  6); EXPECT_EQ(r(1,1),  8); EXPECT_EQ(r(1,2), 10); // row 1
        EXPECT_EQ(r(2,0), 12); EXPECT_EQ(r(2,1), 14); EXPECT_EQ(r(2,2), 16); // row 2
        // clang-format on

        EXPECT_EQ(&r, &m);
    }
}

// ============================================
//                  Substation
// ============================================
TEST(Mat3, substract) {
    const Mat3 matrix(0, 1, 2, 3, 4, 5, 6, 7, 8);

    // operator-(Ma3,Ma3)
    {
        const Mat3 r = matrix - matrix;

        // clang-format off
        EXPECT_EQ(r(0,0), 0);  EXPECT_EQ(r(0,1), 0); EXPECT_EQ(r(0,2), 0); // row 0
        EXPECT_EQ(r(1,0), 0);  EXPECT_EQ(r(1,1), 0); EXPECT_EQ(r(1,2), 0); // row 1
        EXPECT_EQ(r(2,0), 0);  EXPECT_EQ(r(2,1), 0); EXPECT_EQ(r(2,2), 0); // row 2
        // clang-format on
    }

    // operator-=(Ma3,Ma3)
    {
        Mat3        m(matrix);
        const Mat3& r = m -= m;

        // clang-format off
        EXPECT_EQ(m(0,0), 0);  EXPECT_EQ(m(0,1), 0); EXPECT_EQ(m(0,2), 0); // mow 0
        EXPECT_EQ(m(1,0), 0);  EXPECT_EQ(m(1,1), 0); EXPECT_EQ(m(1,2), 0); // mow 1
        EXPECT_EQ(m(2,0), 0);  EXPECT_EQ(m(2,1), 0); EXPECT_EQ(m(2,2), 0); // mow 2

        EXPECT_EQ(r(0,0), 0);  EXPECT_EQ(r(0,1), 0); EXPECT_EQ(r(0,2), 0); // row 0
        EXPECT_EQ(r(1,0), 0);  EXPECT_EQ(r(1,1), 0); EXPECT_EQ(r(1,2), 0); // row 1
        EXPECT_EQ(r(2,0), 0);  EXPECT_EQ(r(2,1), 0); EXPECT_EQ(r(2,2), 0); // row 2
        // clang-format on

        EXPECT_EQ(&r, &m);
    }
}

// ============================================
//              Multiplication
// ============================================

TEST(Mat3, mul_by_scalar) {
    const Mat3 matrix(1, 2, 3, 4, 5, 6, 7, 8, 9);
    // operator*(scalar, Ma3)
    {
        const Mat3 r = 2 * matrix;
        // clang-format off
        EXPECT_EQ(r(0, 0),  2); EXPECT_EQ(r(0, 1),  4); EXPECT_EQ(r(0, 2),  6);
        EXPECT_EQ(r(1, 0),  8); EXPECT_EQ(r(1, 1), 10); EXPECT_EQ(r(1, 2), 12);
        EXPECT_EQ(r(2, 0), 14); EXPECT_EQ(r(2, 1), 16); EXPECT_EQ(r(2, 2), 18);
        // clang-format on
    }
    // operator*(Ma3,scalar)
    {
        const Mat3 r = matrix * 2;

        // clang-format off
        EXPECT_EQ(r(0, 0),  2); EXPECT_EQ(r(0, 1),  4); EXPECT_EQ(r(0, 2),  6);
        EXPECT_EQ(r(1, 0),  8); EXPECT_EQ(r(1, 1), 10); EXPECT_EQ(r(1, 2), 12);
        EXPECT_EQ(r(2, 0), 14); EXPECT_EQ(r(2, 1), 16); EXPECT_EQ(r(2, 2), 18);
        // clang-format on
    }
    // operator*=(Ma3,scalar)
    {
        Mat3        m(matrix);
        const Mat3& r = m *= 2;

        // clang-format off
        EXPECT_EQ(m(0, 0),  2); EXPECT_EQ(m(0, 1),  4); EXPECT_EQ(m(0, 2),  6);
        EXPECT_EQ(m(1, 0),  8); EXPECT_EQ(m(1, 1), 10); EXPECT_EQ(m(1, 2), 12);
        EXPECT_EQ(m(2, 0), 14); EXPECT_EQ(m(2, 1), 16); EXPECT_EQ(m(2, 2), 18);

        EXPECT_EQ(r(0, 0),  2); EXPECT_EQ(r(0, 1),  4); EXPECT_EQ(r(0, 2),  6);
        EXPECT_EQ(r(1, 0),  8); EXPECT_EQ(r(1, 1), 10); EXPECT_EQ(r(1, 2), 12);
        EXPECT_EQ(r(2, 0), 14); EXPECT_EQ(r(2, 1), 16); EXPECT_EQ(r(2, 2), 18);
        // clang-format on

        EXPECT_EQ(&r, &m);
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

    // column vector multiplication
    EXPECT_EQ(matrix * Vec3(10, 20, 30), Vec3(140, 320, 500));

    // row vector multiplication
    EXPECT_EQ(Vec3(10, 20, 30) * matrix, Vec3(300, 360, 420));
}

TEST(Mat3, mul_by_mat3) {
    //
    // 1  2  3     3  4  7     45  43  29
    // 4  5  6  X  9  12 2 =  105 106  74
    // 7  8  9     8  5  6    165 169 119
    //
    const Mat3 matrix1(1, 2, 3, 4, 5, 6, 7, 8, 9);
    const Mat3 matrix2(3, 4, 7, 9, 12, 2, 8, 5, 6);

    // operator*(Ma3,Ma3)
    {
        const auto result = matrix1 * matrix2;
        EXPECT_EQ(result(0, 0), 45);
        EXPECT_EQ(result(0, 1), 43);
        EXPECT_EQ(result(0, 2), 29);
        EXPECT_EQ(result(1, 0), 105);
        EXPECT_EQ(result(1, 1), 106);
        EXPECT_EQ(result(1, 2), 74);
        EXPECT_EQ(result(2, 0), 165);
        EXPECT_EQ(result(2, 1), 169);
        EXPECT_EQ(result(2, 2), 119);
    }

    // operator*=(Ma3,Ma3)
    {
        Mat3        m(matrix1);
        const auto& result = m *= matrix2;
        EXPECT_EQ(m(0, 0), 45);
        EXPECT_EQ(m(0, 1), 43);
        EXPECT_EQ(m(0, 2), 29);
        EXPECT_EQ(m(1, 0), 105);
        EXPECT_EQ(m(1, 1), 106);
        EXPECT_EQ(m(1, 2), 74);
        EXPECT_EQ(m(2, 0), 165);
        EXPECT_EQ(m(2, 1), 169);
        EXPECT_EQ(m(2, 2), 119);

        EXPECT_EQ(result(0, 0), 45);
        EXPECT_EQ(result(0, 1), 43);
        EXPECT_EQ(result(0, 2), 29);
        EXPECT_EQ(result(1, 0), 105);
        EXPECT_EQ(result(1, 1), 106);
        EXPECT_EQ(result(1, 2), 74);
        EXPECT_EQ(result(2, 0), 165);
        EXPECT_EQ(result(2, 1), 169);
        EXPECT_EQ(result(2, 2), 119);

        EXPECT_EQ(&result, &m);
    }
}

TEST(Mat3, determinant_and_inverse) {
    // identity matrix
    {
        EXPECT_EQ(Mat3::kIdentity.determinant(), 1);
        EXPECT_EQ(Mat3::kIdentity.inverse(), Mat3::kIdentity);
    }
    {
        // 1 2 3
        // 4 5 6
        // 7 8 9
        EXPECT_EQ(Mat3(1, 2, 3, 4, 5, 6, 7, 8, 9).determinant(), 0);
    }

    const Mat3 m(1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f);
    EXPECT_EQ(m.inverse(), Mat3(0.375f, -0.5f, 0.875f, -1.0f, 1.0f, -1.0f, 0.875f, -0.5f, 0.375f));

    {
        // 1 5 4
        // 8 2 1
        // 4 6 7
        const Mat3 matrix(1, 5, 4, 8, 2, 1, 4, 6, 7);
        EXPECT_EQ(matrix.determinant(), -92);

        const auto inverse = matrix.inverse();
        EXPECT_FLOAT_EQ(inverse(0, 0), -0.0869565f);
        EXPECT_FLOAT_EQ(inverse(0, 1), 0.11956522f);
        EXPECT_FLOAT_EQ(inverse(0, 2), 0.0326087f);
        EXPECT_FLOAT_EQ(inverse(1, 0), 0.56521738f);
        EXPECT_FLOAT_EQ(inverse(1, 1), 0.0978261f);
        EXPECT_FLOAT_EQ(inverse(1, 2), -0.33695653f);
        EXPECT_FLOAT_EQ(inverse(2, 0), -0.43478262f);
        EXPECT_FLOAT_EQ(inverse(2, 1), -0.15217392f);
        EXPECT_FLOAT_EQ(inverse(2, 2), 0.4130435f);

        EXPECT_EQ(inverse, 1 / 92.f * Mat3(-8, 11, 3, 52, 9, -31, -40, -14, 38));
    }
    {
        // -1  5 4
        //  8 -2 5
        // -4  6 1
        const Mat3 matrix(-1, 5, 4, 8, -2, 5, -4, 6, 1);
        EXPECT_EQ(matrix.determinant(), 52);

        const auto inverse = matrix.inverse();
        EXPECT_EQ(inverse, 1 / 52.f * Mat3(-32, 19, 33, -28, 15, 37, 40, -14, -38));
    }
    {
        // 1 2 3
        // 3 2 1
        // 2 1 3
        const Mat3 matrix(1, 2, 3, 3, 2, 1, 2, 1, 3);
        EXPECT_EQ(matrix.determinant(), -12);

        const auto inverse = matrix.inverse();
        EXPECT_FLOAT_EQ(inverse(0, 0), -0.4166666865f);
        EXPECT_FLOAT_EQ(inverse(0, 1), 0.25f);
        EXPECT_FLOAT_EQ(inverse(0, 2), 0.3333333433f);
        EXPECT_FLOAT_EQ(inverse(1, 0), 0.5833333731f);
        EXPECT_FLOAT_EQ(inverse(1, 1), 0.25f);
        EXPECT_FLOAT_EQ(inverse(1, 2), -0.6666666865f);
        EXPECT_FLOAT_EQ(inverse(2, 0), 0.083333336f);
        EXPECT_FLOAT_EQ(inverse(2, 1), -0.25f);
        EXPECT_FLOAT_EQ(inverse(2, 2), 0.3333333433f);

        EXPECT_EQ(inverse, 1 / 12.f * Mat3(-5, 3, 4, 7, 3, -8, 1, -3, 4));
    }
    {
        // 2 5 1
        // 4 8 9
        // 7 6 3
        const Mat3 matrix(2, 5, 1, 4, 8, 9, 7, 6, 3);
        EXPECT_EQ(matrix.determinant(), 163);

        const auto inverse = matrix.inverse();
        EXPECT_FLOAT_EQ(inverse(0, 0), -0.18404907f);
        EXPECT_FLOAT_EQ(inverse(0, 1), -0.05521472f);
        EXPECT_FLOAT_EQ(inverse(0, 2), 0.22699386f);
        EXPECT_FLOAT_EQ(inverse(1, 0), 0.31288344f);
        EXPECT_FLOAT_EQ(inverse(1, 1), -0.006134969f);
        EXPECT_FLOAT_EQ(inverse(1, 2), -0.08588957f);
        EXPECT_FLOAT_EQ(inverse(2, 0), -0.19631901f);
        EXPECT_FLOAT_EQ(inverse(2, 1), 0.1411043f);
        EXPECT_FLOAT_EQ(inverse(2, 2), -0.024539877f);

        EXPECT_EQ(inverse, 1 / 163.f * Mat3(-30, -9, 37, 51, -1, -14, -32, 23, -4));
    }
}

TEST(Mat3, transpose) {
    // 1 2 3 => 1 4 7
    // 4 5 6    2 5 8
    // 7 8 9    3 6 9
    const Mat3 m(1, 2, 3, 4, 5, 6, 7, 8, 9);
    const Mat3 t(1, 4, 7, 2, 5, 8, 3, 6, 9);
    EXPECT_EQ(m.transpose(), t);
}

/// ===================================================
///                 Transform functions
/// ===================================================
TEST(Mat3, createScaling) {
    // scale on x,y,z axis
    EXPECT_EQ(Mat3::CreateScaling({2, 0, 0}) * Vec3(1, 1, 1), Vec3(2, 0, 0));
    EXPECT_EQ(Mat3::CreateScaling({0, 2, 0}) * Vec3(1, 1, 1), Vec3(0, 2, 0));
    EXPECT_EQ(Mat3::CreateScaling({0, 0, 2}) * Vec3(1, 1, 1), Vec3(0, 0, 2));

    // scale axis
    EXPECT_EQ(Mat3::CreateScaling(2, Vec3::kUnitX), Mat3::CreateScaling({2, 1, 1}));
    EXPECT_EQ(Mat3::CreateScaling(2, Vec3::kUnitY), Mat3::CreateScaling({1, 2, 1}));
    EXPECT_EQ(Mat3::CreateScaling(2, Vec3::kUnitZ), Mat3::CreateScaling({1, 1, 2}));
}

// Test the CreateRotationX function (rotation around X-axis)
TEST(Mat3, CreateRotationX) {
    // rotate [0,0,1] by 90 degrees => [0,-1,0]
    {
        const auto r = Mat3::CreateRotationX(fuse::degrees(90.f)) * Vec3::kUnitZ;
        EXPECT_FLOAT_EQ(r.x, 0.0f);
        EXPECT_FLOAT_EQ(r.y, -1.0f);
        EXPECT_NEAR(r.z, 0.0f, std::numeric_limits<float>::epsilon());
    }
    // rotate [0, 0,-1] by 90 degrees => [0, 1, 0]
    {
        const auto result = Mat3::CreateRotationX(fuse::degrees(90.f)) * Vec3(0, 0, -1);
        EXPECT_FLOAT_EQ(result.x, 0.0f);
        EXPECT_FLOAT_EQ(result.y, 1.0f);
        EXPECT_NEAR(result.z, 0.0f, std::numeric_limits<float>::epsilon());
    }

    // rotate [0,1,0] by 90 degrees => [0,0,1]
    {
        const auto result = Mat3::CreateRotationX(fuse::degrees(90.f)) * Vec3(0, 1, 0);
        EXPECT_FLOAT_EQ(result.x, 0.0f);
        EXPECT_NEAR(result.y, 0.0f, std::numeric_limits<float>::epsilon());
        EXPECT_FLOAT_EQ(result.z, 1.0f);
    }
}

// Test the CreateRotationY function (rotation around y-axis)
TEST(Mat3, CreateRotationY) {
    // rotate [0, 0, -1] by 90 degrees => [-1,0,0]
    {
        const auto result = Mat3::CreateRotationY(fuse::degrees(90.f)) * Vec3(0, 0, -1);
        EXPECT_FLOAT_EQ(result.x, -1.0f);
        EXPECT_FLOAT_EQ(result.y, 0.0f);
        EXPECT_NEAR(result.z, 0.0f, std::numeric_limits<float>::epsilon());
    }
    // rotate [1, 0, 0] by 90 degrees => [0,0,-1]
    {
        const auto result = Mat3::CreateRotationY(fuse::degrees(90.f)) * Vec3(1, 0, 0);
        EXPECT_NEAR(result.x, 0.0f, std::numeric_limits<float>::epsilon());
        EXPECT_FLOAT_EQ(result.y, 0.0f);
        EXPECT_FLOAT_EQ(result.z, -1.0f);
    }
}

// Test the CreateRotationZ function (rotation around z-axis)
TEST(Mat3, CreateRotationZ) {
    // rotate [1, 0, 0, 2] by 90 degrees => [0,1,0,2]
    {
        const auto result = Mat3::CreateRotationZ(fuse::degrees(90.f)) * Vec3(1, 0, 0);
        EXPECT_NEAR(result.x, 0.0f, std::numeric_limits<float>::epsilon());
        EXPECT_FLOAT_EQ(result.y, 1.0f);
        EXPECT_FLOAT_EQ(result.z, 0.0f);
    }
    // rotate [0, 1, 0, 0] by 90 degrees => [-1,0,0,0]
    {
        const auto result = Mat3::CreateRotationZ(fuse::degrees(90.f)) * Vec3(0, 1, 0);
        EXPECT_FLOAT_EQ(result.x, -1.0f);
        EXPECT_NEAR(result.y, 0.0f, std::numeric_limits<float>::epsilon());
        EXPECT_FLOAT_EQ(result.z, 0.0f);
    }
}

TEST(Mat3, CreateRotation_angleAxis) {
    //
    // Test with a angle around x-axis
    // The rotation matrix must match the one create by CreateRotationX()
    //
    {
        const auto angle  = fuse::degrees(45);
        const auto result = Mat3::CreateRotationX(angle);
        const auto rot    = Mat3::CreateRotation(angle, Vec3::kUnitX);
        EXPECT_EQ(rot, result);
    }

    //
    // Test with a angle around y-axis
    // The rotation matrix must match the one create by CreateRotationY()
    //
    {
        const auto angle  = fuse::degrees(45);
        const auto result = Mat3::CreateRotationY(angle);
        const auto rot    = Mat3::CreateRotation(angle, Vec3::kUnitY);
        EXPECT_EQ(rot, result);
    }

    //
    // Test with a angle around Z-axis
    // The rotation matrix must match the one create by CreateRotationZ()
    //
    {
        const auto angle  = fuse::degrees(45);
        const auto result = Mat3::CreateRotationZ(angle);
        const auto rot    = Mat3::CreateRotation(angle, Vec3::kUnitZ);
        EXPECT_EQ(rot, result);
    }
}

TEST(Mat3, CreateReflection) {
    // reflect around xz plane (normal = Y)
    {
        const auto reflection = Mat3::CreateReflection(Vec3::kUnitY);
        EXPECT_EQ(reflection * Vec3(5, 5, 5), Vec3(5, -5, 5));
        EXPECT_EQ(reflection * Vec3(0, 5, 0), Vec3(0, -5, 0));
        EXPECT_EQ(reflection * Vec3(1, 1, 0), Vec3(1, -1, 0));
        EXPECT_EQ(reflection * Vec3(-1, -1, 0), Vec3(-1, 1, 0));
    }
    // reflect around yz plane (normal = X)
    {
        const auto reflection = Mat3::CreateReflection(Vec3::kUnitX);
        EXPECT_EQ(reflection * Vec3(5, 5, 5), Vec3(-5, 5, 5));
    }
    // reflect around xy plane (normal = Z)
    {
        const auto reflection = Mat3::CreateReflection(Vec3::kUnitZ);
        EXPECT_EQ(reflection * Vec3(5, 5, 5), Vec3(5, 5, -5));
    }
}
