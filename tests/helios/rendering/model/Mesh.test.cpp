#include <gtest/gtest.h>
#include <vector>

import helios.rendering.Vertex;
import helios.rendering.model;

using namespace helios::rendering::model;
using namespace helios::rendering;

import helios.rendering.model;

using namespace helios::rendering::model;
using namespace helios::rendering::model::config;
using namespace helios::rendering;

class MyMesh : public Mesh {

public:
    explicit MyMesh(
        std::shared_ptr<const std::vector<helios::rendering::Vertex>> vertices,
        std::shared_ptr<const std::vector<unsigned int>> indices,
        std::shared_ptr<const helios::rendering::mesh::MeshConfig> meshConfig
        ) noexcept
            : Mesh(
                std::move(vertices),
                std::move(indices),
                std::move(meshConfig)
            ) {}

protected:
    void init() override {

    }

};

TEST(MeshTest, data) {


    std::vector<Vertex> vertices{{{1.0f, 2.0f, 3.0f}}};
    auto vertices_ptr = std::make_shared<std::vector<Vertex>>(vertices);
    auto indices_ptr =  std::make_shared<std::vector<unsigned int>>(
        std::vector<unsigned int>{1, 2, 3}
    );
    auto meshConfig = std::make_shared<const MeshConfig>();

    auto mesh = MyMesh{vertices_ptr, indices_ptr, meshConfig};

    EXPECT_EQ(meshConfig.get(), &mesh.meshConfig());
}

