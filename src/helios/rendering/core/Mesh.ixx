module;

#include <memory>

export module helios.rendering.core:Mesh;

import :MeshData;

export namespace helios::rendering {

    class Mesh {

    private:
        std::shared_ptr<const MeshData> mesh_data_;

    public:
        explicit Mesh(std::shared_ptr<const MeshData> meshData) noexcept
            : mesh_data_(std::move(meshData)) {}

        [[nodiscard]] const std::shared_ptr<const MeshData>& mesh_data() const noexcept {
            return mesh_data_;
        }
    };

}