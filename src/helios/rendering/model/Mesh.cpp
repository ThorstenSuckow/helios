module;

#include <memory>
#include <stdexcept>

module helios.rendering.model.Mesh;

namespace helios::rendering::model {

    Mesh::Mesh(std::shared_ptr<const MeshData> meshData)
        : meshData_(std::move(meshData)) {
        if (!meshData_) {
            const std::string msg = "Mesh constructor received a null MeshData shared pointer.";
            logger_.error(msg);
            throw std::invalid_argument(msg);
        }
    }

    [[nodiscard]] const MeshData& Mesh::meshData() const noexcept {
        return *meshData_;
    }

    [[nodiscard]] const int Mesh::indexCount() const noexcept {
        return meshData_->indices().size();
    }

;

}