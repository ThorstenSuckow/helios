/**
 * @file MaterialHandle.ixx
 * @brief Entity-handle alias for material resources.
 */
module;

export module helios.rendering.material.types.MaterialHandle;

import helios.ecs.types;
import helios.rendering.material.types.MaterialDomainTag;

export namespace helios::rendering::material::types {

    /**
     * @brief Handle type used to reference material entities.
     */
    using MaterialHandle = helios::ecs::types::EntityHandle<MaterialDomainTag>;

}
