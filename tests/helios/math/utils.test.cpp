#include <gtest/gtest.h>
#include <numbers>

import helios.math;

namespace math = helios::math;

TEST(UtilsTest, radians) {
    EXPECT_FLOAT_EQ(math::radians(90.0f), std::numbers::pi / 2.0f);
}
