#include <gtest/gtest.h>
#include <ctime>

import helios.math;

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

