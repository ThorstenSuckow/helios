/**
 * @file registry.ixx
 * @brief Component registration for bounds module.
 */
module;

export module helios.engine.mechanics.bounds.registry;

import helios.engine.ecs.ComponentReflector;
import helios.engine.mechanics.bounds.components;

export namespace helios::engine::mechanics::bounds {

    /**
     * @brief Registers all bounds components with the ComponentReflector.
     */
    inline void registerComponents() {
        using R = helios::engine::ecs::ComponentReflector;

        R::registerType<components::LevelBoundsBehaviorComponent>();
    }

}

