#include <gtest/gtest.h>
#include <helios/helios_config.h>

import helios.math;

namespace math = helios::math;

using namespace helios::math;

TEST(Vec4Tests, DefaultConstructorInitializesToZero) {
    vec4f v;
    EXPECT_EQ(v[0], 0.0f);
    EXPECT_EQ(v[1], 0.0f);
    EXPECT_EQ(v[2], 0.0f);
    EXPECT_EQ(v[3], 0.0f);
}

TEST(Vec4Tests, ParameterizedConstructorInitializesCorrectly) {
    vec4i v(1, 2, 3, 4);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], 4);
}

TEST(Vec4Tests, ConstructorFromVec3AndWInitializesCorrectly) {
    vec3f vec3(1.0f, 2.0f, 3.0f);
    vec4f v(vec3, 4.0f);
    EXPECT_EQ(v[0], 1.0f);
    EXPECT_EQ(v[1], 2.0f);
    EXPECT_EQ(v[2], 3.0f);
    EXPECT_EQ(v[3], 4.0f);
}

TEST(Vec4Tests, ConstructorFromVec3SetsWToOne) {
    vec3d vec3(1.5, 2.5, 3.5);
    vec4d v(vec3);
    EXPECT_EQ(v[0], 1.5);
    EXPECT_EQ(v[1], 2.5);
    EXPECT_EQ(v[2], 3.5);
    EXPECT_EQ(v[3], 1.0);
}

TEST(Vec4Tests, ReadOnlyAccessWithinBounds) {
    vec4i v(1, 2, 3, 4);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], 4);
}

TEST(Vec4Tests, ReadWriteAccessWithinBounds) {
    vec4f v(1.0f, 2.0f, 3.0f, 4.0f);
    v[0] = 5.0f;
    v[1] = 6.0f;
    v[2] = 7.0f;
    v[3] = 8.0f;
    EXPECT_EQ(v[0], 5.0f);
    EXPECT_EQ(v[1], 6.0f);
    EXPECT_EQ(v[2], 7.0f);
    EXPECT_EQ(v[3], 8.0f);
}

TEST(Vec4Tests, AccessOutOfBoundsThrowsAssertion) {
    vec4d v(1.0, 2.0, 3.0, 4.0);

    #ifdef HELIOS_DEBUG
    EXPECT_DEATH(v[4], "vec4 - Index out of bounds.");
    #endif
}

TEST(Vec4Tests, ValuePtrReturnsCorrectPointer) {
    vec4i v(1, 2, 3, 4);
    const int* ptr = value_ptr(v);
    EXPECT_EQ(ptr[0], 1);
    EXPECT_EQ(ptr[1], 2);
    EXPECT_EQ(ptr[2], 3);
    EXPECT_EQ(ptr[3], 4);
}
