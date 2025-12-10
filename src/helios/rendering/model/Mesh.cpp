module;

#include <memory>
#include <stdexcept>
#include <cmath>

module helios.rendering.model.Mesh;

import helios.rendering.model.config.MeshConfig;
import helios.rendering.asset.shape.Shape;

namespace helios::rendering::model {

    using namespace helios::rendering::model::config;

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

    unsigned int Mesh::indexCount() const noexcept {
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

    const helios::math::aabbf& Mesh::aabb() const noexcept  {
        if (needsUpdate_) {
            if (!vertices_->empty()) {
                float minX = std::numeric_limits<float>::max();
                float minY = std::numeric_limits<float>::max();
                float minZ = std::numeric_limits<float>::max();
                float maxX = std::numeric_limits<float>::lowest();
                float maxY = std::numeric_limits<float>::lowest();
                float maxZ = std::numeric_limits<float>::lowest();

                for (const auto& v: *vertices_) {
                    // min
                    minX = std::min(minX, v.position[0]);
                    minY = std::min(minY, v.position[1]);
                    minZ = std::min(minZ, v.position[2]);

                    // max
                    maxX = std::max(maxX, v.position[0]);
                    maxY = std::max(maxY, v.position[1]);
                    maxZ = std::max(maxZ, v.position[2]);
                }

                aabb_ = helios::math::aabbf(minX, minY, minZ, maxX, maxY, maxZ);
            } else {
                aabb_ = helios::math::aabbf(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
            }

            needsUpdate_ = false;
        }

        return aabb_;
    };
}