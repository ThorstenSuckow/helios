/**
 * @file registry.ixx
 * @brief Component registration for health module.
 */
module;

export module helios.engine.mechanics.health.registry;

import helios.engine.ecs.ComponentReflector;
import helios.engine.mechanics.health.components;

export namespace helios::engine::mechanics::health {

    /**
     * @brief Registers all health components with the ComponentReflector.
     */
    inline void registerComponents() {
        using R = helios::engine::ecs::ComponentReflector;

        R::registerType<components::HealthComponent>();
    }

}

