/**
 * @file registry.ixx
 * @brief Component registration for AI module.
 */
module;

export module helios.engine.modules.ai.registry;

import helios.engine.ecs.ComponentReflector;
import helios.engine.modules.ai.components;

export namespace helios::engine::modules::ai {

    /**
     * @brief Registers all AI components with the ComponentReflector.
     */
    inline void registerComponents() {
        using R = helios::engine::ecs::ComponentReflector;

        R::registerType<components::ChaseComponent>();
    }

}

