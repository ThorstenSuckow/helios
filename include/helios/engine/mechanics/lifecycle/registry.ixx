/**
 * @file registry.ixx
 * @brief Component registration for lifecycle module.
 */
module;

export module helios.engine.mechanics.lifecycle.registry;

import helios.engine.ecs.ComponentReflector;
import helios.engine.mechanics.lifecycle.components;

export namespace helios::engine::mechanics::lifecycle {

    /**
     * @brief Registers all lifecycle components with the ComponentReflector.
     */
    inline void registerComponents() {
        using R = helios::engine::ecs::ComponentReflector;

        R::registerType<components::Active>();
        R::registerType<components::DelayedComponentEnabler>();
    }

}

