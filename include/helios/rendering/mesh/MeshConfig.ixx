/**
 * @file MeshConfig.ixx
 * @brief Mesh configuration including primitive type.
 */
module;

export module helios.rendering.mesh.MeshConfig;

import helios.rendering.mesh.PrimitiveType;


export namespace helios::rendering::mesh {

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
