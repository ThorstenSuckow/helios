#include <gtest/gtest.h>
#include <ctime>

import helios.math;

namespace math = helios::math;

 struct test_data {
    math::vec3f vec_a;
    math::vec3f vec_b;
};

static test_data setup() {

    time_t seed = time(0);
    std::cout << "using seed: " << seed << std::endl;
    std::srand(seed);

    return test_data{
        .vec_a = math::vec3f{
            static_cast<float>(std::rand()),
            static_cast<float>(std::rand()),
            static_cast<float>(std::rand())
        },
        .vec_b = math::vec3f{
            static_cast<float>(std::rand()),
            static_cast<float>(std::rand()),
            static_cast<float>(std::rand())
        },
    };
}


TEST(Vec3Tests, accessor) {

    math::vec3f v = math::vec3f();

    EXPECT_FLOAT_EQ(v[0], 0.0f);
    EXPECT_FLOAT_EQ(v[1], 0.0f);
    EXPECT_FLOAT_EQ(v[2], 0.0f);

    v[0] = 3;

    EXPECT_FLOAT_EQ(v[0], 3.0f);

    float& v4 = v[0];
    v4 = 5;
    EXPECT_FLOAT_EQ(v[0], 5.0f);

#if !defined(NDEBUG)
    EXPECT_DEATH(v[-3], "vec3 - Index out of bounds.");
    EXPECT_DEATH(v[4], "vec3 - Index out of bounds.");
#endif

}

TEST(Vec3Tests, normalize) {

    // norm
    EXPECT_FLOAT_EQ(
        math::vec3f(1, 2, 3 ).length(),
        std::sqrt( std::pow(1, 2) +  std::pow(2, 2) + std::pow(3, 2) )
    );

    // normalize
    math::vec3f v = (math::vec3f(1, 2, 3)).normalize();
    const float norm = math::vec3f( 1, 2, 3 ).length();
    EXPECT_FLOAT_EQ(v[0], 1 / norm);
    EXPECT_FLOAT_EQ(v[1], 2 / norm);
    EXPECT_FLOAT_EQ(v[2], 3 / norm);
}


TEST(CoreTest, cross) {
    test_data data = setup();

    auto vec_c = math::cross(data.vec_a, data.vec_b);

    EXPECT_FLOAT_EQ(vec_c[0], data.vec_a[1]*data.vec_b[2] - data.vec_a[2]*data.vec_b[1]);
    EXPECT_FLOAT_EQ(vec_c[1], data.vec_a[2]*data.vec_b[0] - data.vec_a[0]*data.vec_b[2]);
    EXPECT_FLOAT_EQ(vec_c[2], data.vec_a[0]*data.vec_b[1] - data.vec_a[1]*data.vec_b[0]);
}

TEST(CoreTest, subtraction) {

    auto vec_c = math::vec3f{1.0f, 2.0f, 3.0f} - math::vec3f{4.0f, 4.0f, 4.0f};

    EXPECT_FLOAT_EQ(vec_c[0], -3.0f);
    EXPECT_FLOAT_EQ(vec_c[1], -2.0f);
    EXPECT_FLOAT_EQ(vec_c[2], -1.0f);
}

TEST(CoreTest, dot) {
    test_data data = setup();

    auto adotb = math::dot(data.vec_a, data.vec_b);

    EXPECT_FLOAT_EQ(
        adotb,
        data.vec_a[0] * data.vec_b[0] +
        data.vec_a[1] * data.vec_b[1] +
        data.vec_a[2] * data.vec_b[2]
    );
}

TEST(Vec3Tests, normOfZeroVector) {
    // norm of zero vector should return 0
    math::vec3f zero_f(0.0f, 0.0f, 0.0f);
    EXPECT_FLOAT_EQ(zero_f.length(), 0.0f);

    math::vec3d zero_d(0.0, 0.0, 0.0);
    EXPECT_DOUBLE_EQ(zero_d.length(), 0.0);

    // For integer vectors, length() should return double
    math::vec3i zero_i(0, 0, 0);
    EXPECT_DOUBLE_EQ(zero_i.length(), 0.0);
}

TEST(Vec3Tests, normalizeZeroVector) {
    // Normalizing a zero vector should return a zero vector
    // (avoids division by zero and NaN results)
    math::vec3f zero_f(0.0f, 0.0f, 0.0f);
    auto normalized_f = zero_f.normalize();

    EXPECT_FLOAT_EQ(normalized_f[0], 0.0f);
    EXPECT_FLOAT_EQ(normalized_f[1], 0.0f);
    EXPECT_FLOAT_EQ(normalized_f[2], 0.0f);

    // Verify no NaN values
    EXPECT_FALSE(std::isnan(normalized_f[0]));
    EXPECT_FALSE(std::isnan(normalized_f[1]));
    EXPECT_FALSE(std::isnan(normalized_f[2]));
}

TEST(Vec3Tests, normalizeNearZeroVector) {
    // Test with very small but non-zero vector
    const float epsilon = 1e-10f;
    math::vec3f near_zero(epsilon, epsilon, epsilon);
    auto normalized = near_zero.normalize();

    // Should still produce a valid normalized vector
    float norm = std::sqrt(normalized[0] * normalized[0] +
                          normalized[1] * normalized[1] +
                          normalized[2] * normalized[2]);

    EXPECT_NEAR(norm, 1.0f, 1e-5f);
}

TEST(Vec3Tests, normWithIntegerVector) {
    // Test that integer vectors return double for length()
    math::vec3i v(1, 1, 1);
    double norm = v.length();

    EXPECT_DOUBLE_EQ(norm, std::sqrt(3.0));

    // Verify no precision loss
    math::vec3i v2(3, 4, 0);
    EXPECT_DOUBLE_EQ(v2.length(), 5.0);
}

TEST(Vec3Tests, normalizeIntegerVector) {
    // Normalizing integer vector should return vec3<double>
    math::vec3i v(1, 2, 2);
    auto normalized = v.normalize();

    const double expected_norm = 3.0; // sqrt(1 + 4 + 4) = sqrt(9) = 3
    EXPECT_DOUBLE_EQ(normalized[0], 1.0 / expected_norm);
    EXPECT_DOUBLE_EQ(normalized[1], 2.0 / expected_norm);
    EXPECT_DOUBLE_EQ(normalized[2], 2.0 / expected_norm);

    // Verify the normalized vector has length 1
    double norm = std::sqrt(
        normalized[0] * normalized[0] +
        normalized[1] * normalized[1] +
        normalized[2] * normalized[2]
    );
    EXPECT_NEAR(norm, 1.0, 1e-10);
}

TEST(Vec3Tests, operatorEqualityIdenticalVectors) {
    // Test that identical vectors are equal
    math::vec3f v1(1.0f, 2.0f, 3.0f);
    math::vec3f v2(1.0f, 2.0f, 3.0f);

    EXPECT_TRUE(v1 == v2);
    EXPECT_TRUE(v2 == v1);

    // Test with zero vectors
    math::vec3f zero1(0.0f, 0.0f, 0.0f);
    math::vec3f zero2(0.0f, 0.0f, 0.0f);
    EXPECT_TRUE(zero1 == zero2);
}

TEST(Vec3Tests, operatorEqualityDifferentVectors) {
    // Test that different vectors are not equal
    math::vec3f v1(1.0f, 2.0f, 3.0f);
    math::vec3f v2(1.0f, 2.0f, 3.1f);
    math::vec3f v3(1.0f, 2.1f, 3.0f);
    math::vec3f v4(1.1f, 2.0f, 3.0f);

    EXPECT_FALSE(v1 == v2);
    EXPECT_FALSE(v1 == v3);
    EXPECT_FALSE(v1 == v4);
}

TEST(Vec3Tests, operatorEqualityIntegerVectors) {
    // Test with integer vectors
    math::vec3i v1(1, 2, 3);
    math::vec3i v2(1, 2, 3);
    math::vec3i v3(1, 2, 4);

    EXPECT_TRUE(v1 == v2);
    EXPECT_FALSE(v1 == v3);
}

TEST(Vec3Tests, sameMethodWithIdenticalVectors) {
    // Test that identical vectors are considered the same
    math::vec3f v1(1.0f, 2.0f, 3.0f);
    math::vec3f v2(1.0f, 2.0f, 3.0f);

    EXPECT_TRUE(v1.same(v2));
    EXPECT_TRUE(v2.same(v1));
}

TEST(Vec3Tests, sameMethodWithEpsilonDifference) {
    // Test vectors that differ by less than epsilon
    math::vec3f v1(1.0f, 2.0f, 3.0f);
    math::vec3f v2(1.00005f, 2.00005f, 3.00005f);

    // Should be the same with default epsilon (0.0001)
    EXPECT_TRUE(v1.same(v2));

    // Test with custom epsilon
    math::vec3f v3(1.0f, 2.0f, 3.0f);
    math::vec3f v4(1.001f, 2.001f, 3.001f);

    EXPECT_FALSE(v3.same(v4)); // Default epsilon too small
    EXPECT_TRUE(v3.same(v4, 0.01f)); // Custom epsilon large enough
}

TEST(Vec3Tests, sameMethodWithLargeDifference) {
    // Test vectors that differ by more than epsilon
    math::vec3f v1(1.0f, 2.0f, 3.0f);
    math::vec3f v2(1.1f, 2.0f, 3.0f);
    math::vec3f v3(1.0f, 2.1f, 3.0f);
    math::vec3f v4(1.0f, 2.0f, 3.1f);

    EXPECT_FALSE(v1.same(v2));
    EXPECT_FALSE(v1.same(v3));
    EXPECT_FALSE(v1.same(v4));
}

TEST(Vec3Tests, sameMethodWithZeroVectors) {
    // Test with zero vectors
    math::vec3f zero1(0.0f, 0.0f, 0.0f);
    math::vec3f zero2(0.0f, 0.0f, 0.0f);
    math::vec3f near_zero(0.00005f, 0.00005f, 0.00005f);

    EXPECT_TRUE(zero1.same(zero2));
    EXPECT_TRUE(zero1.same(near_zero)); // Within default epsilon
}

TEST(Vec3Tests, sameMethodVsOperatorEquality) {
    // Test that same() is more lenient than operator==
    math::vec3f v1(1.0f, 2.0f, 3.0f);
    math::vec3f v2(1.00005f, 2.00005f, 3.00005f);

    EXPECT_FALSE(v1 == v2); // Strict equality
    EXPECT_TRUE(v1.same(v2)); // Epsilon-based equality
}

TEST(Vec3Tests, sameMethodWithDoubleVectors) {
    // Test with double precision vectors
    math::vec3d v1(1.0, 2.0, 3.0);
    math::vec3d v2(1.00005, 2.00005, 3.00005);

    EXPECT_TRUE(v1.same(v2));

    math::vec3d v3(1.0, 2.0, 3.0);
    math::vec3d v4(1.0001, 2.0001, 3.0001);

    EXPECT_FALSE(v3.same(v4)); // Just outside default epsilon
}

TEST(Vec3Tests, sameMethodEdgeCases) {
    // Test edge cases where only one component differs
    math::vec3f v1(1.0f, 2.0f, 3.0f);
    math::vec3f v2_x(1.001f, 2.0f, 3.0f);
    math::vec3f v2_y(1.0f, 2.001f, 3.0f);
    math::vec3f v2_z(1.0f, 2.0f, 3.001f);

    EXPECT_FALSE(v1.same(v2_x));
    EXPECT_FALSE(v1.same(v2_y));
    EXPECT_FALSE(v1.same(v2_z));

    // But should pass with larger epsilon
    EXPECT_TRUE(v1.same(v2_x, 0.01f));
    EXPECT_TRUE(v1.same(v2_y, 0.01f));
    EXPECT_TRUE(v1.same(v2_z, 0.01f));
}

TEST(Vec3Tests, zeroVectorNormalization) {
    math::vec3f zeroVec = math::vec3f(0.0f, 0.0f, 0.0f);
    math::vec3f normalized = zeroVec.normalize();

    EXPECT_FLOAT_EQ(normalized[0], 0.0f);
    EXPECT_FLOAT_EQ(normalized[1], 0.0f);
    EXPECT_FLOAT_EQ(normalized[2], 0.0f);
}

TEST(Vec3Tests, addition) {
    math::vec3f vec1 = math::vec3f(1.0f, 2.0f, 3.0f);
    math::vec3f vec2 = math::vec3f(4.0f, 5.0f, 6.0f);

    auto result = vec1 + vec2;

    EXPECT_FLOAT_EQ(result[0], 5.0f);
    EXPECT_FLOAT_EQ(result[1], 7.0f);
    EXPECT_FLOAT_EQ(result[2], 9.0f);
}

TEST(Vec3Tests, scalarMultiplication) {
    math::vec3f vec = math::vec3f(1.0f, -2.0f, 3.0f);
    float scalar = 2.0f;

    auto result = vec * scalar;

    EXPECT_FLOAT_EQ(result[0], 2.0f);
    EXPECT_FLOAT_EQ(result[1], -4.0f);
    EXPECT_FLOAT_EQ(result[2], 6.0f);
}

TEST(Vec3Tests, scalarDivision) {
    math::vec3f vec = math::vec3f(2.0f, -4.0f, 6.0f);
    float scalar = 2.0f;

    auto result = vec / scalar;

    EXPECT_FLOAT_EQ(result[0], 1.0f);
    EXPECT_FLOAT_EQ(result[1], -2.0f);
    EXPECT_FLOAT_EQ(result[2], 3.0f);
}

TEST(Vec3Tests, scalarDivisionByZero) {
    math::vec3f vec = math::vec3f(2.0f, -4.0f, 6.0f);

#if !defined(NDEBUG)
    EXPECT_DEATH(vec / 0.0f, "s must not be 0");
#endif
}
