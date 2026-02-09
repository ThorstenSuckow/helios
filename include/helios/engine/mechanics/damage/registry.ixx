/**
 * @file registry.ixx
 * @brief Component registration for damage module.
 */
module;

export module helios.engine.mechanics.damage.registry;

import helios.engine.ecs.ComponentReflector;
import helios.engine.mechanics.damage.components;

export namespace helios::engine::mechanics::damage {

    /**
     * @brief Registers all damage components with the ComponentReflector.
     */
    inline void registerComponents() {
        using R = helios::engine::ecs::ComponentReflector;

        R::registerType<components::DamageDealerComponent>();
    }

}

