/**
 * @file registry.ixx
 * @brief Component registration for ecs module.
 */
module;

export module helios.engine.ecs.registry;

import helios.engine.ecs.ComponentReflector;
import helios.engine.ecs.components;

export namespace helios::engine::ecs {

    /**
     * @brief Registers all lifecycle components with the ComponentReflector.
     */
    inline void registerComponents() {
        using R = helios::engine::ecs::ComponentReflector;

        R::registerType<components::HierarchyComponent>();
    }

}

