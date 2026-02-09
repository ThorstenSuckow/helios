/**
 * @file registry.ixx
 * @brief Component registration for spawn module.
 */
module;

export module helios.engine.mechanics.spawn.registry;

import helios.engine.ecs.ComponentReflector;
import helios.engine.mechanics.spawn.components;

export namespace helios::engine::mechanics::spawn {

    /**
     * @brief Registers all spawn components with the ComponentReflector.
     */
    inline void registerComponents() {
        using R = helios::engine::ecs::ComponentReflector;

        R::registerType<components::SpawnedByProfileComponent>();
        R::registerType<components::EmittedByComponent>();
    }

}

