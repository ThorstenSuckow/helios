/**
 * @file registry.ixx
 * @brief Component registration for physics collision module.
 */
module;

export module helios.engine.modules.physics.collision.registry;

import helios.engine.ecs.ComponentReflector;
import helios.engine.modules.physics.collision.components;

export namespace helios::engine::modules::physics::collision {

    /**
     * @brief Registers all collision components with the ComponentReflector.
     */
    inline void registerComponents() {
        using R = helios::engine::ecs::ComponentReflector;

        R::registerType<components::CollisionStateComponent>();
        R::registerType<components::CollisionComponent>();
        R::registerType<components::AabbColliderComponent>();
    }

}

