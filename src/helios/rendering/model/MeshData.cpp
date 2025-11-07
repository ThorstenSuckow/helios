module;

#include <memory>
#include <string>
#include <vector>
#include <stdexcept>

module helios.rendering.model.MeshData;

import helios.rendering.model.config.MeshConfig;
import helios.rendering.asset.shape.Shape;

using namespace helios::rendering::model::config;

#define HELIOS_LOG_SCOPE "helios::rendering::model::MeshData"
namespace helios::rendering::model {


    MeshData::MeshData(
        std::shared_ptr<const std::vector<Vertex>> vertices,
        std::shared_ptr<const std::vector<unsigned int>> indices,
        std::shared_ptr<const MeshConfig> meshConfig
    ) : vertices_(std::move(vertices)),
        indices_(std::move(indices)),
        meshConfig_(std::move(meshConfig)) {

        if (!vertices_ || !indices_ || !meshConfig_) {
            const std::string msg = "MeshData constructor received a null shared pointer.";
            logger_.error(msg);
            throw std::invalid_argument(msg);
        }
    }


    MeshData::MeshData(
    const helios::rendering::asset::shape::Shape& shape,
    std::shared_ptr<const MeshConfig> meshConfig
    ) :
        vertices_(shape.vertices),
        indices_(shape.indices),
        meshConfig_(std::move(meshConfig)) {

        if (!vertices_ || !indices_ || !meshConfig_) {
            const std::string msg = "MeshData constructor received a Shape with null shared pointer.";
            logger_.error(msg);
            throw std::invalid_argument(msg);
        }
    }


    const std::vector<Vertex>& MeshData::vertices() const noexcept {
        return *vertices_;
    }


    const std::vector<unsigned int>& MeshData::indices() const noexcept  {
        return *indices_;
    };

    const MeshConfig& MeshData::meshConfig() const noexcept  {
        return *meshConfig_;
    };


}
