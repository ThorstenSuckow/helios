module;

export module helios.rendering.model.config.MeshConfig;

import helios.rendering.model.config.PrimitiveTopology;


export namespace helios::rendering::model::config {

    /**
     * @brief Configuration for how a Mesh's data should be interpreted and rendered.
     *
     * This class defines rendering-specific properties for meshes, e.g. to be used
     * with draw calls.
     */
    struct MeshConfig {

        /**
         * @brief Topology config for the draw call, e.g. setting the draw mode.
         */
        PrimitiveTopology primitiveTopology = PrimitiveTopology::Triangles;

    };
}
