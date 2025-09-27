#include <gtest/gtest.h>


import helios.rendering.model;

using namespace helios::rendering::model;

TEST(MeshTest, data) {

    auto mesh_data = MeshData::from({{1.0f, 0.5f, 0.2f}});

    auto mesh_data_ptr = std::make_shared<const MeshData>(mesh_data);

    const auto mesh = Mesh{mesh_data_ptr};

    EXPECT_EQ(mesh_data_ptr, mesh.meshData());
}

