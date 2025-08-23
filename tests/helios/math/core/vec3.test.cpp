#include <gtest/gtest.h>

import helios.math;

namespace math = helios::math;

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