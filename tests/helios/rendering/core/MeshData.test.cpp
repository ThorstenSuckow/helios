#include <gtest/gtest.h>


import helios.rendering.core;

namespace rendering = helios::rendering;


TEST(MeshDataTest, data) {

    auto meshData = rendering::MeshData();//{{}};


    EXPECT_EQ(meshData.vertices.size(), 0);

    meshData.vertices.push_back(
        {{1.0f, 0.5f, 0.2f}}
    );

    EXPECT_EQ(meshData.vertices.size(), 1);

}


