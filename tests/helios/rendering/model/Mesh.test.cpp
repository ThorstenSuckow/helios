#include <gtest/gtest.h>
#include <vector>

import helios.rendering.Vertex;
import helios.rendering.model;

using namespace helios::rendering::model;
using namespace helios::rendering;

import helios.rendering.model;

using namespace helios::rendering::model;
using namespace helios::rendering;

class MyMesh : public Mesh {

public:
    explicit MyMesh(std::shared_ptr<const MeshData> meshData) noexcept
            : Mesh(std::move(meshData)) {}

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

    auto meshData_ptr = std::make_shared<const MeshData>(vertices_ptr, indices_ptr);

    auto mesh = MyMesh{meshData_ptr};

    EXPECT_EQ(meshData_ptr.get(), &mesh.meshData());
}

