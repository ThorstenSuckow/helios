/**
 * @brief Mesh configuration for drawing/topology
 */
module;

export module helios.rendering.model.config.MeshConfig;

import helios.rendering.model.config.PrimitiveType;


export namespace helios::rendering::model::config {

    /**
     * @brief Configuration for how a Mesh's data should be interpreted and rendered.
     *
     * This struct defines rendering-specific properties for meshes, for example
     * the primitive topology used by draw calls.
     */
    struct MeshConfig {

        /**
         * @brief Topology config for the draw call, e.g. setting the draw mode.
         */
        PrimitiveType primitiveType = PrimitiveType::Triangles;

    };
}
