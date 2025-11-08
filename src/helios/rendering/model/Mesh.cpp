module;

#include <memory>
#include <stdexcept>

module helios.rendering.model.Mesh;

import helios.rendering.model.config.MeshConfig;
import helios.rendering.asset.shape.Shape;

using namespace helios::rendering::model::config;

namespace helios::rendering::model {

    Mesh::Mesh(
        std::shared_ptr<const std::vector<Vertex>> vertices,
        std::shared_ptr<const std::vector<unsigned int>> indices,
        std::shared_ptr<const MeshConfig> meshConfig
    ) : vertices_(std::move(vertices)),
        indices_(std::move(indices)),
        meshConfig_(std::move(meshConfig)) {

        if (!vertices_ || !indices_ || !meshConfig_) {
            const std::string msg = "Mesh constructor received a null shared pointer.";
            logger_.error(msg);
            throw std::invalid_argument(msg);
        }
    }


    Mesh::Mesh(
    const helios::rendering::asset::shape::Shape& shape,
    std::shared_ptr<const MeshConfig> meshConfig
    ) :
        vertices_(shape.vertices),
        indices_(shape.indices),
        meshConfig_(std::move(meshConfig)) {

        if (!vertices_ || !indices_ || !meshConfig_) {
            const std::string msg = "Mesh constructor received a Shape with null shared pointer.";
            logger_.error(msg);
            throw std::invalid_argument(msg);
        }
    }

    const unsigned int Mesh::indexCount() const noexcept {
        return indices_->size();
    }

    const std::vector<Vertex>& Mesh::vertices() const noexcept {
        return *vertices_;
    }


    const std::vector<unsigned int>& Mesh::indices() const noexcept  {
        return *indices_;
    };

    const MeshConfig& Mesh::meshConfig() const noexcept  {
        return *meshConfig_;
    };

}