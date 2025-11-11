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
