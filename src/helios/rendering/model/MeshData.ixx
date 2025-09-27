module;

#include <memory>
#include <vector>

export module helios.rendering.model:MeshData;

import :Vertex;

export namespace helios::rendering::model {

    class MeshData {
    private:
        std::shared_ptr<std::vector<Vertex>> vertices_;

    public:
        static MeshData from(std::initializer_list<math::vec3> vs) {
            auto vertices = std::make_unique<std::vector<Vertex>>();
            for (auto& v:vs) {
                vertices->emplace_back(v);
            }

            return MeshData(std::move(vertices));
        }

        MeshData() = default;

        MeshData(std::unique_ptr<std::vector<Vertex>> vertices) : vertices_(std::move(vertices)) {}

        const std::vector<Vertex>& vertices() const {
            return *vertices_;
        }
    };

}
