/**
 * @file registry.ixx
 * @brief Component registration for UI transform module.
 */
module;

export module helios.engine.modules.ui.transform.registry;

import helios.engine.ecs.ComponentReflector;
import helios.engine.modules.ui.transform.components;

export namespace helios::engine::modules::ui::transform {

    /**
     * @brief Registers all UI transform components with the ComponentReflector.
     */
    inline void registerComponents() {
        using R = helios::engine::ecs::ComponentReflector;

        R::registerType<components::UiTransformComponent>();
    }

}

