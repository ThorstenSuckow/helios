module;

#include <memory>
#include <string>
#include <vector>
#include <stdexcept>

module helios.rendering.model.MeshData;

import helios.rendering.asset.shape.Shape;

#define HELIOS_LOG_SCOPE "helios::rendering::model::MeshData"
namespace helios::rendering::model {


    MeshData::MeshData(
        std::shared_ptr<const std::vector<Vertex>> vertices,
        std::shared_ptr<const std::vector<unsigned int>> indices
    ) : vertices_(std::move(vertices)),
        indices_(std::move(indices)) {

        if (!vertices_ || !indices_) {
            const std::string msg = "MeshData constructor received a null shared pointer.";
            logger_.error(msg);
            throw std::invalid_argument(msg);
        }
    }


    MeshData::MeshData(const helios::rendering::shape::Shape& shape) :
        vertices_(shape.vertices),
        indices_(shape.indices) {

        if (!vertices_ || !indices_) {
            const std::string msg = "MeshData constructor received a Shape with null shared pointer.";
            logger_.error(msg);
            throw std::invalid_argument(msg);
        }
    }


    [[nodiscard]] const std::vector<Vertex>& MeshData::vertices() const noexcept {
        return *vertices_;
    }


    [[nodiscard]] const std::vector<unsigned int>& MeshData::indices() const noexcept  {
        return *indices_;
    };

}
