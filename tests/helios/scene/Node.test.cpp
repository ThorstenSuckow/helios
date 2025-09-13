#include <gtest/gtest.h>


import helios.scene;
import helios.rendering.core;

namespace scene = helios::scene;
namespace rendering = helios::rendering;
struct test_data {

};

static test_data setup() {
    return test_data{};
}

using namespace helios::rendering::core;

TEST(NodeTest, Constructor) {

    // init data
    test_data data = setup();

    auto mesh_data = MeshData();
    mesh_data.vertices.push_back(
        {{1.0f, 0.5f, 0.2f}}
    );
    auto mesh_data_ptr = std::make_shared<const MeshData>(mesh_data);
    const auto mesh = Mesh{mesh_data_ptr};

    EXPECT_EQ(mesh_data_ptr, mesh.mesh_data());


    auto scene_node = scene::Node();
    const auto guid = scene_node.guid();
    EXPECT_GT(guid.value(), 0);
    EXPECT_EQ(scene_node.mesh(), nullptr);

    scene_node.set_mesh(std::make_shared<Mesh>(mesh));


}


