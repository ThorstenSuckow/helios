#include <gtest/gtest.h>


import helios.rendering.core;

using namespace helios::rendering::core;

TEST(VertextTest, position) {

    const auto v = Vertex{
        .position = {1.0f, 0.5f, 0.2f}
    };

    EXPECT_FLOAT_EQ(v.position[0], 1.0f);
    EXPECT_FLOAT_EQ(v.position[1], 0.5f);
    EXPECT_FLOAT_EQ(v.position[2], 0.2f);
}


