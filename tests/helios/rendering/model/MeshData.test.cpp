#include <gtest/gtest.h>
#include <vector>

import helios.rendering.Vertex;
import helios.rendering.model;

using namespace helios::rendering::model;
using namespace helios::rendering;

TEST(MeshDataTest, data) {


    std::vector<Vertex> vertices{{{1.0f, 2.0f, 3.0f}}};
    auto vertices_ptr = std::make_shared<std::vector<Vertex>>(vertices);
    auto indices_ptr =  std::make_shared<std::vector<unsigned int>>(
        std::vector<unsigned int>{1, 2, 3}
    );

    auto meshData = MeshData(std::move(vertices_ptr), std::move(indices_ptr));

    EXPECT_EQ(meshData.vertices().size(), 1);

}


