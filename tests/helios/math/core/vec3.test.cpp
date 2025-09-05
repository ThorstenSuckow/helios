#include <gtest/gtest.h>
#include <ctime>

import helios.math;

namespace math = helios::math;

 struct test_data {
    math::vec3 vec_a;
    math::vec3 vec_b;
};

static test_data setup() {

    time_t seed = time(0);
    std::cout << "using seed: " << seed << std::endl;
    std::srand(seed);

    return test_data{
        .vec_a = math::vec3{
            static_cast<float>(std::rand()),
            static_cast<float>(std::rand()),
            static_cast<float>(std::rand())
        },
        .vec_b = math::vec3{
            static_cast<float>(std::rand()),
            static_cast<float>(std::rand()),
            static_cast<float>(std::rand())
        },
    };
}


TEST(Vec3Tests, accessor) {

    math::vec3 v = math::vec3();

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
        math::vec3(1, 2, 3 ).norm(),
        std::sqrt( std::pow(1, 2) +  std::pow(2, 2) + std::pow(3, 2) )
    );

    // normalize
    math::vec3 v = (math::vec3(1, 2, 3)).normalize();
    const float norm = math::vec3( 1, 2, 3 ).norm();
    EXPECT_FLOAT_EQ(v[0], 1 / norm);
    EXPECT_FLOAT_EQ(v[1], 2 / norm);
    EXPECT_FLOAT_EQ(v[2], 3 / norm);
}


TEST(CoreTest, cross) {

    // init data
    test_data data = setup();

    auto vec_c = math::cross(data.vec_a, data.vec_b);

    EXPECT_FLOAT_EQ(vec_c[0], data.vec_a[1]*data.vec_b[2] - data.vec_a[2]*data.vec_b[1]);
    EXPECT_FLOAT_EQ(vec_c[1], data.vec_a[2]*data.vec_b[0] - data.vec_a[0]*data.vec_b[2]);
    EXPECT_FLOAT_EQ(vec_c[2], data.vec_a[0]*data.vec_b[1] - data.vec_a[1]*data.vec_b[0]);


}
