/**
 * @file MeshData.ixx
 * @brief Plain mesh payload containing vertices and indices.
 */
module;

#include <vector>

export module helios.rendering.mesh.types.MeshData;

import helios.rendering.Vertex;

using namespace helios::rendering;

export namespace helios::rendering::mesh::types {

    /**
     * @brief Raw mesh data container.
     */
    struct MeshData {

        /**
         * @brief Vertex buffer payload.
         */
        std::vector<Vertex> vertices;

        /**
         * @brief Index buffer payload.
         */
        std::vector<unsigned int> indices;

    };
}