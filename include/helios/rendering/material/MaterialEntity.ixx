/**
 * @file MaterialEntity.ixx
 * @brief Entity alias for material-managed ECS entities.
 */
module;

export module helios.rendering.material.MaterialEntity;

import helios.rendering.material.MaterialEntityManager;
import helios.ecs.Entity;

using namespace helios::ecs;
export namespace helios::rendering::material {

    /** @brief ECS entity alias bound to `MaterialEntityManager`. */
    using MaterialEntity = Entity<MaterialEntityManager>;


}