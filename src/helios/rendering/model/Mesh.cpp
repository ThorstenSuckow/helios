module;

#include <memory>
#include <stdexcept>
#include <limits>

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

    const helios::math::aabbf& Mesh::aabb() const noexcept  {
        if (needsUpdate_) {
            float minX = INFINITY;
            float minY = INFINITY;
            float minZ = INFINITY;
            float maxX = -INFINITY;
            float maxY = -INFINITY;
            float maxZ = -INFINITY;

            for (helios::rendering::Vertex v: *vertices_) {
                if (minX > v.position[0]) {
                    minX = v.position[0];
                }
                if (minY > v.position[1]) {
                    minY = v.position[1];
                }
                if (minZ > v.position[2]) {
                    minZ = v.position[2];
                }

                if (maxX < v.position[0]) {
                    maxX = v.position[0];
                }
                if (maxY < v.position[1]) {
                    maxY = v.position[1];
                }
                if (maxZ < v.position[2]) {
                    maxZ = v.position[2];
                }
            }

            aabb_ = helios::math::aabbf(minX, minY, minZ, maxX, maxY, maxZ);

            needsUpdate_ = false;
        }

        return aabb_;
    };
}