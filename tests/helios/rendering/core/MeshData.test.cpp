#include <gtest/gtest.h>


import helios.rendering.core;

using namespace helios::rendering::core;

TEST(MeshDataTest, data) {

    auto meshData = MeshData();//{{}};
    EXPECT_EQ(meshData.vertices().size(), 0);

    meshData = MeshData::from({{1.0f, 0.5f, 0.2f}});

    EXPECT_EQ(meshData.vertices().size(), 1);

}


