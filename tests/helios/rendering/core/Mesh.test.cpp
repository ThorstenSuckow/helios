#include <gtest/gtest.h>


import helios.rendering.core;

namespace rendering = helios::rendering;


TEST(MeshTest, data) {

    auto mesh_data = rendering::MeshData();

    mesh_data.vertices.push_back(
        {{1.0f, 0.5f, 0.2f}}
    );

    auto mesh_data_ptr = std::make_shared<const rendering::MeshData>(mesh_data);

    const auto mesh = rendering::Mesh{mesh_data_ptr};

    EXPECT_EQ(mesh_data_ptr, mesh.mesh_data());
}

