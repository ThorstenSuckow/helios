#include <gtest/gtest.h>
#include <ctime>

import helios.math;


using namespace helios::math;

namespace math = helios::math;

 struct test_data {
    math::vec2f vec_a;
    math::vec2f vec_b;
};

static test_data setup() {

    time_t seed = time(0);
    std::cout << "using seed: " << seed << std::endl;
    std::srand(seed);

    return test_data{
        .vec_a = math::vec2{
            static_cast<float>(std::rand()),
            static_cast<float>(std::rand())
        },
        .vec_b = math::vec2{
            static_cast<float>(std::rand()),
            static_cast<float>(std::rand())
        },
    };
}


TEST(Vec2Tests, accessor) {

    math::vec2f v = math::vec2f();

    EXPECT_FLOAT_EQ(v[0], 0.0f);
    EXPECT_FLOAT_EQ(v[1], 0.0f);

    v[0] = 3;

    EXPECT_FLOAT_EQ(v[0], 3.0f);

    float& v4 = v[0];
    v4 = 5;
    EXPECT_FLOAT_EQ(v[0], 5.0f);

#if !defined(NDEBUG)
    EXPECT_DEATH(v[-3], "vec2 - Index out of bounds.");
    EXPECT_DEATH(v[4], "vec2 - Index out of bounds.");
#endif

}

TEST(Vec2Tests, DefaultConstructorInitializesToZero) {
    vec2f v;
    EXPECT_EQ(v[0], 0.0f);
    EXPECT_EQ(v[1], 0.0f);
}

TEST(Vec2Tests, ParameterizedConstructorInitializesCorrectly) {
    vec2i v(3, 7);
    EXPECT_EQ(v[0], 3);
    EXPECT_EQ(v[1], 7);
}

TEST(Vec2Tests, ReadOnlyAccessWithinBounds) {
    vec2d v(1.5, 2.5);
    EXPECT_EQ(v[0], 1.5);
    EXPECT_EQ(v[1], 2.5);
}

TEST(Vec2Tests, ReadWriteAccessWithinBounds) {
    vec2i v(1, 2);
    v[0] = 5;
    v[1] = 10;
    EXPECT_EQ(v[0], 5);
    EXPECT_EQ(v[1], 10);
}

TEST(Vec2Tests, ScalarMultiplicationProducesCorrectResult) {
    vec2f v(2.0f, 3.0f);
    auto result = v * 2.0f;
    EXPECT_EQ(result[0], 4.0f);
    EXPECT_EQ(result[1], 6.0f);
}

TEST(Vec2Tests, DotProductComputesCorrectly) {
    vec2i v1(1, 2);
    vec2i v2(3, 4);
    EXPECT_EQ(dot(v1, v2), 11);
}

TEST(Vec2Tests, AccessOutOfBoundsThrowsAssertion) {
    vec2f v(1.0f, 2.0f);
    EXPECT_DEATH(v[2], "vec2 - Index out of bounds.");
}

TEST(Vec2Tests, normalizeZeroVector) {
    // Normalizing a zero vector should return a zero vector
    // (avoids division by zero and NaN results)
    math::vec2f zero_f(0.0f, 0.0f);
    auto normalized_f = zero_f.normalize();

    EXPECT_FLOAT_EQ(normalized_f[0], 0.0f);
    EXPECT_FLOAT_EQ(normalized_f[1], 0.0f);

    // Verify no NaN values
    EXPECT_FALSE(std::isnan(normalized_f[0]));
    EXPECT_FALSE(std::isnan(normalized_f[1]));

    // Test with double precision
    math::vec2d zero_d(0.0, 0.0);
    auto normalized_d = zero_d.normalize();

    EXPECT_DOUBLE_EQ(normalized_d[0], 0.0);
    EXPECT_DOUBLE_EQ(normalized_d[1], 0.0);
}

TEST(Vec2Tests, normalizeNearZeroVector) {
    // Test with very small but non-zero vector
    const float epsilon = 1e-10f;
    math::vec2f near_zero(epsilon, epsilon);
    auto normalized = near_zero.normalize();

    // Should still produce a valid normalized vector
    float magnitude = std::sqrt(normalized[0] * normalized[0] +
                               normalized[1] * normalized[1]);

    EXPECT_NEAR(magnitude, 1.0f, 1e-5f);
}

TEST(Vec2Tests, normalizeIntegerVector) {
    // Normalizing integer vector should return vec2<double>
    math::vec2i v(3, 4);
    auto normalized = v.normalize();

    const double expected_magnitude = 5.0; // sqrt(9 + 16) = sqrt(25) = 5
    EXPECT_DOUBLE_EQ(normalized[0], 3.0 / expected_magnitude);
    EXPECT_DOUBLE_EQ(normalized[1], 4.0 / expected_magnitude);

    // Verify the normalized vector has length 1
    double magnitude = std::sqrt(
        normalized[0] * normalized[0] +
        normalized[1] * normalized[1]
    );
    EXPECT_NEAR(magnitude, 1.0, 1e-10);
}

TEST(Vec2Tests, normalizeProducesUnitVector) {
    // Test that normalize() produces a vector with magnitude 1
    math::vec2f v(5.0f, 12.0f); // magnitude = 13
    auto normalized = v.normalize();

    float magnitude = std::sqrt(
        normalized[0] * normalized[0] +
        normalized[1] * normalized[1]
    );

    EXPECT_NEAR(magnitude, 1.0f, 1e-6f);

    // Verify direction is preserved
    EXPECT_NEAR(normalized[0], 5.0f / 13.0f, 1e-6f);
    EXPECT_NEAR(normalized[1], 12.0f / 13.0f, 1e-6f);
}

TEST(Vec2Tests, normalizePreservesDirection) {
    // Test various directions
    math::vec2f v1(1.0f, 0.0f); // Along x-axis
    auto n1 = v1.normalize();
    EXPECT_FLOAT_EQ(n1[0], 1.0f);
    EXPECT_FLOAT_EQ(n1[1], 0.0f);

    math::vec2f v2(0.0f, 1.0f); // Along y-axis
    auto n2 = v2.normalize();
    EXPECT_FLOAT_EQ(n2[0], 0.0f);
    EXPECT_FLOAT_EQ(n2[1], 1.0f);

    math::vec2f v3(1.0f, 1.0f); // 45 degrees
    auto n3 = v3.normalize();
    const float sqrt2_inv = 1.0f / std::sqrt(2.0f);
    EXPECT_NEAR(n3[0], sqrt2_inv, 1e-6f);
    EXPECT_NEAR(n3[1], sqrt2_inv, 1e-6f);
}
