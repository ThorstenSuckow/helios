/**
 * @file registry.ixx
 * @brief Component registration for model module.
 */
module;

export module helios.engine.modules.rendering.model.registry;

import helios.engine.ecs.ComponentReflector;
import helios.engine.modules.rendering.model.components;

export namespace helios::engine::modules::rendering::model {

    /**
     * @brief Registers all model components with the ComponentReflector.
     */
    inline void registerComponents() {
        using R = helios::engine::ecs::ComponentReflector;

        R::registerType<components::ModelAabbComponent>();
    }

}

