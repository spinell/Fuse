#include "GTestUtils.h"

#include <math/Angle.h>
#include <math/Quaternion.h>

#include <gtest/gtest.h>

using namespace fuse;

TEST(Quaternion, traits) {
    static_assert(std::is_trivially_copy_constructible_v<Quaternion>);
    static_assert(std::is_trivially_copy_assignable_v<Quaternion>);
    static_assert(std::is_trivially_move_constructible_v<Quaternion>);
    static_assert(std::is_trivially_move_assignable_v<Quaternion>);
    static_assert(std::is_trivially_copyable_v<Quaternion>);
}

TEST(Quaternion, constants) {}

TEST(Quaternion, ctor) {
    // Constructor with component
    {
        const Quaternion q(1, 2, 3, 4);
        EXPECT_EQ(q.x, 1.f);
        EXPECT_EQ(q.y, 2.f);
        EXPECT_EQ(q.z, 3.f);
        EXPECT_EQ(q.w, 4.f);
    }

    // Constructor with Axis/Angle
    {
        Quaternion q(Vec3(1, 1, 1).normalize(), fuse::degrees(45));
        EXPECT_EQ(q, Quaternion(0.2209424f, 0.2209424f, 0.2209424f, 0.9238795f));
    }
}

TEST(Quaternion, addition) {
    const Quaternion q1(1, 2, 3, 4);
    const Quaternion result = q1 + q1;
    EXPECT_EQ(result, Quaternion(2, 4, 6, 8));
}

TEST(Quaternion, substraction) {
    const Quaternion q1(1, 2, 3, 4);
    const Quaternion result = q1 - q1;
    EXPECT_EQ(result, Quaternion(0, 0, 0, 0));
}

TEST(Quaternion, multiply) {
    // operator*(float, Quaternion)
    {
        EXPECT_EQ(2.f * Quaternion(1, 2, 3, 4), Quaternion(2, 4, 6, 8));
    }

    // operator*(Quaternion, float)
    {
        EXPECT_EQ(Quaternion(1, 2, 3, 4) * 2.f, Quaternion(2, 4, 6, 8));
    }

    // operator*=(Quaternion, float)
    {
        Quaternion  q1(1, 2, 3, 4);
        const auto& q2 = q1 *= 2.f;
        EXPECT_EQ(q1, Quaternion(2, 4, 6, 8));
        EXPECT_EQ(q2, Quaternion(2, 4, 6, 8));
        EXPECT_EQ(&q1, &q2);
    }

    // operator*(Quaternion, Quaternion)
    {
        const Quaternion q1(3, 4, 3, -3);
        const Quaternion q2(2, 1, -4, 2);
        const Quaternion result = q1 * q2;
        EXPECT_EQ(result.x, -19.0f);
        EXPECT_EQ(result.y, 23.0f);
        EXPECT_EQ(result.z, 13.0f);
        EXPECT_EQ(result.w, -04.0f);
    }
    // operator*=(Quaternion, Quaternion)
    {
        Quaternion       q1(3, 4, 3, -3);
        const Quaternion q2(2, 1, -4, 2);
        const auto&      q3 = q1 *= q2;
        EXPECT_EQ(q1, Quaternion(-19.f, 23.f, 13.f, -4.f));
        EXPECT_EQ(q3, Quaternion(-19.f, 23.f, 13.f, -4.f));
        EXPECT_EQ(&q1, &q3);
    }
    //
    // Quat * Vec3
    //
    {
        Quaternion q;
        q.fromAxisAngle(Vec3::kUnitX, degrees(90));
        auto v = q * Vec3::kUnitY;
        EXPECT_FLOAT_EQ(v.x, 0.f);
        EXPECT_NEAR(v.y, 0.f, std::numeric_limits<float>::epsilon());
        EXPECT_FLOAT_EQ(v.z, 1.f);

        v = q * Vec3::kUnitYNeg;
        EXPECT_FLOAT_EQ(v.x, 0.f);
        EXPECT_NEAR(v.y, 0.f, std::numeric_limits<float>::epsilon());
        EXPECT_FLOAT_EQ(v.z, -1.f);

        q.fromAxisAngle(Vec3::kUnitY, degrees(90));
        v = q * Vec3::kUnitX;
        EXPECT_NEAR(v.x, 0.f, std::numeric_limits<float>::epsilon());
        EXPECT_FLOAT_EQ(v.y, 0.f);
        EXPECT_FLOAT_EQ(v.z, -1.f);

        v = q * Vec3::kUnitXNeg;
        EXPECT_NEAR(v.x, 0.f, std::numeric_limits<float>::epsilon());
        EXPECT_FLOAT_EQ(v.y, 0.f);
        EXPECT_FLOAT_EQ(v.z, 1.f);

        q.fromAxisAngle(Vec3::kUnitZ, degrees(90));
        v = q * Vec3::kUnitY;
        EXPECT_FLOAT_EQ(v.x, -1.f);
        EXPECT_FLOAT_EQ(v.y, 0.f);
        EXPECT_FLOAT_EQ(v.z, 0.f);

        v = q * Vec3::kUnitXNeg;
        EXPECT_FLOAT_EQ(v.x, 0.f);
        EXPECT_FLOAT_EQ(v.y, -1.f);
        EXPECT_FLOAT_EQ(v.z, 0.f);
    }

    {
        Quaternion q = Quaternion(1, 2, 3, 4).inverse();
        EXPECT_FLOAT_EQ(q.x, -1 / 30.f);
        EXPECT_FLOAT_EQ(q.y, -1 / 15.f);
        EXPECT_FLOAT_EQ(q.z, -1 / 10.f);
        EXPECT_FLOAT_EQ(q.w, 2 / 15.f);
    }
}

TEST(Quaternion, divide) {

    // operator/(Quaternion, float)
    {
        EXPECT_EQ(Quaternion(2, 4, 6, 8) / 2.f, Quaternion(1, 2, 3, 4));
    }

    // operator/=(Quaternion, float)
    {
        Quaternion  q1(2, 4, 6, 8);
        const auto& q2 = q1 /= 2.f;
        EXPECT_EQ(q1, Quaternion(1, 2, 3, 4));
        EXPECT_EQ(q2, Quaternion(1, 2, 3, 4));
        EXPECT_EQ(&q1, &q2);
    }
}

TEST(Quaternion, conjugate) {
    EXPECT_EQ(Quaternion(2, 4, 6, 8).conjugate(), Quaternion(-2, -4, -6, 8));
    EXPECT_EQ(Quaternion(2, 4, 6, -8).conjugate(), Quaternion(-2, -4, -6, -8));
}

TEST(Quaternion, dot) {
    const Quaternion q1(1, 2, 3, 4);
    const Quaternion q2(5, 6, 7, 8);
    EXPECT_EQ(q1.dot(q2), 70.f);
}

TEST(Quaternion, inverse) {
    constexpr Quaternion xAxis45(0.3826834f, 0, 0, 0.9238795f);  // 45  degrees around x-axis
    constexpr Quaternion xAxis90(0.7071068f, 0, 0, 0.7071068f);  // 90  degrees around x-axis
    constexpr Quaternion xAxis135(0.9238795f, 0, 0, 0.3826834f); // 135 degrees around x-axis
    {
        Quaternion q = xAxis45.inverse();
        EXPECT_FLOAT_EQ(q.x, -0.3826834f);
        EXPECT_FLOAT_EQ(q.y, 0);
        EXPECT_FLOAT_EQ(q.z, 0);
        EXPECT_FLOAT_EQ(q.w, 0.9238795f);
    }
    {
        Quaternion q = xAxis90.inverse();
        EXPECT_FLOAT_EQ(q.x, -0.7071068f);
        EXPECT_FLOAT_EQ(q.y, 0);
        EXPECT_FLOAT_EQ(q.z, 0);
        EXPECT_FLOAT_EQ(q.w, 0.7071068f);
    }
    {
        Quaternion q = xAxis135.inverse();
        EXPECT_FLOAT_EQ(q.x, -0.9238795f);
        EXPECT_FLOAT_EQ(q.y, 0);
        EXPECT_FLOAT_EQ(q.z, 0);
        EXPECT_FLOAT_EQ(q.w, 0.3826834f);
    }
    {
        Quaternion q = Quaternion(1, 2, 3, 4).inverse();
        EXPECT_FLOAT_EQ(q.x, -1 / 30.f);
        EXPECT_FLOAT_EQ(q.y, -1 / 15.f);
        EXPECT_FLOAT_EQ(q.z, -1 / 10.f);
        EXPECT_FLOAT_EQ(q.w, 2 / 15.f);
    }
}

TEST(Quaternion, length) {
    EXPECT_FLOAT_EQ(Quaternion(0, 0, 0, 0).length(), 0.f);
    EXPECT_FLOAT_EQ(Quaternion(1, 0, 0, 0).length(), 1.f);
    EXPECT_FLOAT_EQ(Quaternion(0, 1, 0, 0).length(), 1.f);
    EXPECT_FLOAT_EQ(Quaternion(0, 0, 1, 0).length(), 1.f);
    EXPECT_FLOAT_EQ(Quaternion(0, 0, 0, 1).length(), 1.f);
    EXPECT_FLOAT_EQ(Quaternion(1, 1, 0, 0).length(), 1.4142135f);
    EXPECT_FLOAT_EQ(Quaternion(1, 1, 1, 0).length(), 1.7320508f);
    EXPECT_FLOAT_EQ(Quaternion(1, 1, 1, 1).length(), 2.f);
}

TEST(Quaternion, lengthSquared) {
    EXPECT_FLOAT_EQ(Quaternion(0, 0, 0, 0).lengthSquared(), 0.f);
    EXPECT_FLOAT_EQ(Quaternion(1, 0, 0, 0).lengthSquared(), 1.f);
    EXPECT_FLOAT_EQ(Quaternion(0, 1, 0, 0).lengthSquared(), 1.f);
    EXPECT_FLOAT_EQ(Quaternion(0, 0, 1, 0).lengthSquared(), 1.f);
    EXPECT_FLOAT_EQ(Quaternion(0, 0, 0, 1).lengthSquared(), 1.f);
    EXPECT_FLOAT_EQ(Quaternion(1, 1, 0, 0).lengthSquared(), 2.f);
    EXPECT_FLOAT_EQ(Quaternion(1, 1, 1, 0).lengthSquared(), 3.f);
    EXPECT_FLOAT_EQ(Quaternion(1, 1, 1, 1).lengthSquared(), 4.f);
}

TEST(Quaternion, normalize) {
    // they are already normalized, nothing should change.
    EXPECT_EQ(Quaternion(1, 0, 0, 0.f).normalize(), Quaternion(1.f, 0.f, 0.f, 0.f));
    EXPECT_EQ(Quaternion(0, 1, 0, 0.f).normalize(), Quaternion(0.f, 1.f, 0.f, 0.f));
    EXPECT_EQ(Quaternion(0, 0, 1, 0.f).normalize(), Quaternion(0.f, 0.f, 1.f, 0.f));
    EXPECT_EQ(Quaternion(0, 0, 0, 1.f).normalize(), Quaternion(0.f, 0.f, 0.f, 1.f));

    // they are already normalized, nothing should change.
    EXPECT_EQ(Quaternion(-1, 0, 0, 0.f).normalize(), Quaternion(-1.f, 0.f, 0.f, 0.f));
    EXPECT_EQ(Quaternion(0, -1, 0, 0.f).normalize(), Quaternion(0.f, -1.f, 0.f, 0.f));
    EXPECT_EQ(Quaternion(0, 0, -1, 0.f).normalize(), Quaternion(0.f, 0.f, -1.f, 0.f));
    EXPECT_EQ(Quaternion(0, 0, 0, -1.f).normalize(), Quaternion(0.f, 0.f, 0.f, -1.f));

    const auto q = Quaternion(1, 2, 3, 4).normalize();
    EXPECT_FLOAT_EQ(q.x, 0.18257418f);
    EXPECT_FLOAT_EQ(q.y, 0.36514837f);
    EXPECT_FLOAT_EQ(q.z, 0.54772258f);
    EXPECT_FLOAT_EQ(q.w, 0.73029673f);
}

// test the axis(), axisX(), axisY(), axisZ() function
TEST(Quaternion, axis) {
    // List of quaternion with there expected axis.
    const std::vector<std::tuple<Quaternion, Vec3, Vec3, Vec3>> data = {
      // clang-format off
      {Quaternion::kIdentity, // Zero quaternion should produce standard axis.
        Vec3::kUnitX,
        Vec3::kUnitY,
        Vec3::kUnitZ},
      {Quaternion(0, 0, 0, 0), // Zero quaternion should produce standard axis.
        Vec3::kUnitX,
        Vec3::kUnitY,
        Vec3::kUnitZ},
      {Quaternion(1, 0, 0, 0),
        Vec3::kUnitX,
        Vec3::kUnitYNeg,
        Vec3::kUnitZNeg},
      {Quaternion(0, 1, 0, 0),
        Vec3::kUnitXNeg,
        Vec3::kUnitY,
        Vec3::kUnitZNeg},
      {Quaternion(0, 0, 1, 0),
        Vec3::kUnitXNeg,
        Vec3::kUnitYNeg,
        Vec3::kUnitZ},
      {Quaternion(0, 0, 1, 0),
        Vec3::kUnitXNeg,
        Vec3::kUnitYNeg,
        Vec3::kUnitZ},
      {Quaternion(0.1736126f, 0.1736126f, 0.1736126f, 0.953717f),
        Vec3(0.87943465f, 0.39143723f, -0.27087188f),
        Vec3(-0.27087188f, 0.87943465f, 0.39143723f),
        Vec3(0.39143723f, -0.27087188f, 0.87943465f)}
      // clang-format on
    };

    for (const auto& [q, expectedX, expectedY, expectedZ] : data) {
        std::string msg = std::format("Quaternion {}", q);
        SCOPED_TRACE(msg);

        const auto [xAxis, yAxis, zAxis] = q.axis();
        EXPECT_EQ(xAxis, expectedX);
        EXPECT_EQ(xAxis, q.axisX());

        EXPECT_EQ(yAxis, expectedY);
        EXPECT_EQ(yAxis, q.axisY());

        EXPECT_EQ(zAxis, expectedZ);
        EXPECT_EQ(zAxis, q.axisZ());
    }
}

TEST(Quaternion, setMatrix) {
    Quaternion q;

    // Identity Quaternion should be Identity matrix
    q.setMatrix(Mat3::kIdentity);
    EXPECT_EQ(q, Quaternion::kIdentity);

    // test the four situation
    // 1- trace is > 0 (axis does not matter)
    q.setMatrix(Mat3::CreateRotationX(fuse::degrees(180.f)));
    EXPECT_QUAT_NEAR(q, Quaternion(1.f, 0.f, 0.f, 0.0f));
    // 2- trace is < 0 and m00 > m11 && m00 > m22
    q.setMatrix(Mat3::CreateRotationX(fuse::degrees(121)));
    EXPECT_QUAT_NEAR(q, Quaternion(0.8703557f, 0.f, 0.f, 0.4924236f));
    // 3-  m11 > m22
    q.setMatrix(Mat3::CreateRotationY(fuse::degrees(121)));
    EXPECT_QUAT_NEAR(q, Quaternion(0.f, 0.8703557f, 0.f, 0.4924236f));
    // 4- Other
    q.setMatrix(Mat3::CreateRotationZ(fuse::degrees(121)));
    EXPECT_QUAT_NEAR(q, Quaternion(0.f, 0.f, 0.8703557f, 0.4924236f));
}

TEST(Quaternion, asMatrix) {
    // Identity Quaternion should be Identity matrix
    EXPECT_EQ(Quaternion::kIdentity.asMatrix(), Mat3::kIdentity);

    // 180 degrees rotation around x-axis.
    {
        const auto mat = Quaternion(1, 0, 0, 0).asMatrix();
        EXPECT_EQ(mat, Mat3(1, 0, 0, 0, -1, 0, 0, 0, -1));
    }
    // 180 degrees rotation around y-axis.
    {
        const auto mat = Quaternion(0, 1, 0, 0).asMatrix();
        EXPECT_EQ(mat, Mat3(-1, 0, 0, 0, 1, 0, 0, 0, -1));
    }
    // 180 degrees rotation around z-axis.
    {
        const auto mat = Quaternion(0, 0, 1, 0).asMatrix();
        EXPECT_EQ(mat, Mat3(-1, 0, 0, 0, -1, 0, 0, 0, 1));
    }

    // 220 degrees rotation around [1,1,1].
    {
        const auto mat = Quaternion(0.5425318f, 0.5425318f, 0.5425318f, -0.3420201f).asMatrix();
        EXPECT_FLOAT_EQ(mat(0, 0), -0.17736292f);
        EXPECT_FLOAT_EQ(mat(0, 1), 0.9597951f);
        EXPECT_FLOAT_EQ(mat(0, 2), 0.2175679f);

        EXPECT_FLOAT_EQ(mat(1, 0), 0.2175679f);
        EXPECT_FLOAT_EQ(mat(1, 1), -0.17736292f);
        EXPECT_FLOAT_EQ(mat(1, 2), 0.9597951f);

        EXPECT_FLOAT_EQ(mat(2, 0), 0.9597951f);
        EXPECT_FLOAT_EQ(mat(2, 1), 0.2175679f);
        EXPECT_FLOAT_EQ(mat(2, 2), -0.17736292f);
    }
}

// test fromAxisAngle() / toAxisAngle()
TEST(Quaternion, axisAngle) {
    // List of quaternion with there expected axis.
    const std::vector<std::tuple<Vec3, Angle, Quaternion>> data = {
      // clang-format off
      {Vec3::kZero,  fuse::degrees(0), Quaternion(0, 0, 0, 1)},
      {Vec3::kUnitX, fuse::degrees(45), Quaternion(0.38268346f, 0, 0, 0.9238795f)},
      {Vec3::kUnitY, fuse::degrees(45), Quaternion(0, 0.38268346f, 0, 0.9238795f)},
      {Vec3::kUnitZ, fuse::degrees(45), Quaternion(0, 0, 0.38268346f, 0.9238795f)},
      {Vec3(1,1,0).normalize(), fuse::degrees(45), Quaternion(0.27059805f, 0.27059805f, 0, 0.9238795f)},
      {Vec3(1,1,1).normalize(), fuse::degrees(45), Quaternion(0.2209424f, 0.2209424f, 0.2209424f, 0.9238795f)},
      {Vec3(1,1,1).normalize(), fuse::degrees(180), Quaternion(0.57735026f, 0.57735026f, 0.57735026f, 0)},
      {Vec3(1,1,1).normalize(), fuse::degrees(220), Quaternion(0.5425318f, 0.5425318f, 0.5425318f, -0.3420201f)}
      // clang-format on
    };

    for (const auto& [orgAxis, orgAngle, expectedQuat] : data) {
        std::string msg = std::format("Axis {} Angle {}", orgAxis, orgAngle);
        SCOPED_TRACE(msg);

        Quaternion q;
        q.fromAxisAngle(orgAxis, orgAngle);
        EXPECT_QUAT_NEAR(q, expectedQuat);

        Vec3  axis{};
        Angle angle{};
        q.toAxisAngle(axis, angle);
        EXPECT_NEAR(axis.x, orgAxis.x, 1e-6);
        EXPECT_NEAR(axis.y, orgAxis.y, 1e-6);
        EXPECT_NEAR(axis.z, orgAxis.z, 1e-6);
        EXPECT_ANGLE_NEAR(angle, orgAngle);
    }
}

TEST(Quaternion, MakeRotationX) {
    EXPECT_QUAT_NEAR(Quaternion::MakeRotationX(degrees(0)), Quaternion(0, 0, 0, 1));
    EXPECT_QUAT_NEAR(Quaternion::MakeRotationX(degrees(45)),
                     Quaternion(0.38268346f, 0, 0, 0.9238795f));
    EXPECT_QUAT_NEAR(Quaternion::MakeRotationX(degrees(90)),
                     Quaternion(0.70710677f, 0, 0, 0.70710677f));
    EXPECT_QUAT_NEAR(Quaternion::MakeRotationX(degrees(135)),
                     Quaternion(0.9238795f, 0, 0, 0.38268343f));
    EXPECT_QUAT_NEAR(Quaternion::MakeRotationX(degrees(180)), Quaternion(1, 0, 0, 0));
    EXPECT_QUAT_NEAR(Quaternion::MakeRotationX(degrees(225)),
                     Quaternion(0.92387956f, 0, 0, -0.3826834f));
    EXPECT_QUAT_NEAR(Quaternion::MakeRotationX(degrees(270)),
                     Quaternion(0.70710677f, 0, 0, -0.70710677f));
    EXPECT_QUAT_NEAR(Quaternion::MakeRotationX(degrees(315)),
                     Quaternion(0.3826835f, 0, 0, -0.9238795f));
    EXPECT_QUAT_NEAR(Quaternion::MakeRotationX(degrees(360)), Quaternion(0, 0, 0, -1));
}

TEST(Quaternion, MakeRotationY) {
    EXPECT_QUAT_NEAR(Quaternion::MakeRotationY(degrees(0)), Quaternion(0, 0, 0, 1));
    EXPECT_QUAT_NEAR(Quaternion::MakeRotationY(degrees(45)),
                     Quaternion(0, 0.38268346f, 0, 0.9238795f));
    EXPECT_QUAT_NEAR(Quaternion::MakeRotationY(degrees(90)),
                     Quaternion(0, 0.70710677f, 0, 0.70710677f));
    EXPECT_QUAT_NEAR(Quaternion::MakeRotationY(degrees(135)),
                     Quaternion(0, 0.9238795f, 0, 0.38268343f));
    EXPECT_QUAT_NEAR(Quaternion::MakeRotationY(degrees(180)), Quaternion(0, 1, 0, 0));
    EXPECT_QUAT_NEAR(Quaternion::MakeRotationY(degrees(225)),
                     Quaternion(0, 0.92387956f, 0, -0.3826834f));
    EXPECT_QUAT_NEAR(Quaternion::MakeRotationY(degrees(270)),
                     Quaternion(0, 0.70710677f, 0, -0.70710677f));
    EXPECT_QUAT_NEAR(Quaternion::MakeRotationY(degrees(315)),
                     Quaternion(0, 0.3826835f, 0, -0.9238795f));
    EXPECT_QUAT_NEAR(Quaternion::MakeRotationY(degrees(360)), Quaternion(0, 0, 0, -1));
}

TEST(Quaternion, MakeRotationZ) {
    EXPECT_QUAT_NEAR(Quaternion::MakeRotationZ(degrees(0)), Quaternion(0, 0, 0, 1));
    EXPECT_QUAT_NEAR(Quaternion::MakeRotationZ(degrees(45)),
                     Quaternion(0, 0, 0.38268346f, 0.9238795f));
    EXPECT_QUAT_NEAR(Quaternion::MakeRotationZ(degrees(90)),
                     Quaternion(0, 0, 0.70710677f, 0.70710677f));
    EXPECT_QUAT_NEAR(Quaternion::MakeRotationZ(degrees(135)),
                     Quaternion(0, 0, 0.9238795f, 0.38268343f));
    EXPECT_QUAT_NEAR(Quaternion::MakeRotationZ(degrees(180)), Quaternion(0, 0, 1, 0));
    EXPECT_QUAT_NEAR(Quaternion::MakeRotationZ(degrees(225)),
                     Quaternion(0, 0, 0.92387956f, -0.3826834f));
    EXPECT_QUAT_NEAR(Quaternion::MakeRotationZ(degrees(270)),
                     Quaternion(0, 0, 0.70710677f, -0.70710677f));
    EXPECT_QUAT_NEAR(Quaternion::MakeRotationZ(degrees(315)),
                     Quaternion(0, 0, 0.3826835f, -0.9238795f));
    EXPECT_QUAT_NEAR(Quaternion::MakeRotationZ(degrees(360)), Quaternion(0, 0, 0, -1));
}

TEST(Quaternion, std_format) {
    ASSERT_EQ(std::format("{}", Quaternion(1, 2, 3, 4)), "[1, 2, 3, 4]");
}
