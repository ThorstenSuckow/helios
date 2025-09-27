module;

#include <memory>

export module helios.rendering.model:Mesh;

import :MeshData;

export namespace helios::rendering::model {

    class Mesh {

    private:
        std::shared_ptr<const MeshData> meshData_;

    public:
        explicit Mesh(std::shared_ptr<const MeshData> meshData) noexcept
            : meshData_(std::move(meshData)) {}

        [[nodiscard]] const std::shared_ptr<const MeshData>& meshData() const noexcept {
            return meshData_;
        }
    };

}