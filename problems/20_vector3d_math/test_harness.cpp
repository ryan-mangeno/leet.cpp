#include <test_framework.h>
#include "submission.h"

#include <cmath>

using namespace leet_cpp;
using namespace test_framework;

TEST(Construction) {
    Vector3D v(1.0, 2.0, 3.0);
    ASSERT_EQ(v.x(), 1.0);
    ASSERT_EQ(v.y(), 2.0);
    ASSERT_EQ(v.z(), 3.0);
}

TEST(Addition) {
    Vector3D v1(1, 2, 3);
    Vector3D v2(4, 5, 6);
    Vector3D result = v1 + v2;
    ASSERT_EQ(result.x(), 5.0);
    ASSERT_EQ(result.y(), 7.0);
    ASSERT_EQ(result.z(), 9.0);
}

TEST(Subtraction) {
    Vector3D v1(5, 7, 9);
    Vector3D v2(1, 2, 3);
    Vector3D result = v1 - v2;
    ASSERT_EQ(result.x(), 4.0);
    ASSERT_EQ(result.y(), 5.0);
    ASSERT_EQ(result.z(), 6.0);
}

TEST(ScalarMultiplication) {
    Vector3D v(1, 2, 3);
    Vector3D result = v * 2.0;
    ASSERT_EQ(result.x(), 2.0);
    ASSERT_EQ(result.y(), 4.0);
    ASSERT_EQ(result.z(), 6.0);
}

TEST(DotProduct_Orthogonal) {
    Vector3D v1(1, 0, 0);
    Vector3D v2(0, 1, 0);
    ASSERT_EQ(v1.dot(v2), 0.0);
}

TEST(DotProduct_General) {
    Vector3D v1(1, 2, 3);
    Vector3D v2(4, 5, 6);
    ASSERT_EQ(v1.dot(v2), 32.0); // 1*4 + 2*5 + 3*6
}

TEST(CrossProduct_Basis) {
    Vector3D v1(1, 0, 0);
    Vector3D v2(0, 1, 0);
    Vector3D cross = v1.cross(v2);
    ASSERT_EQ(cross.x(), 0.0);
    ASSERT_EQ(cross.y(), 0.0);
    ASSERT_EQ(cross.z(), 1.0);
}

TEST(Magnitude) {
    Vector3D v(3, 4, 0);
    ASSERT_EQ(v.magnitude(), 5.0);
}

TEST(Normalize) {
    Vector3D v(3, 4, 0);
    Vector3D unit = v.normalize();
    ASSERT_EQ(unit.magnitude(), 1.0);
    ASSERT_EQ(unit.x(), 0.6);
    ASSERT_EQ(unit.y(), 0.8);
}

TEST(Normalize_ZeroVector) {
    Vector3D zero(0, 0, 0);
    ASSERT_THROWS([&](){ zero.normalize(); });
}

int main() {
    SECTION("Part 1: Basic Operations");
    SECTION("Part 2: Advanced Operations");
    SECTION("Part 3: Edge Cases");
    RUN_ALL_TESTS();
    return 0;
}
