/**
 * @file SceneHandle.ixx
 * @brief Handle alias for scene entities.
 */
module;

export module helios.scene.types.SceneHandle;


import helios.ecs.types;

import helios.scene.types.SceneDomainTag;

export namespace helios::scene::types {

    /**
     * @brief ECS handle type referencing scene entities.
     */
    using SceneHandle = helios::ecs::types::EntityHandle<SceneDomainTag>;

}
