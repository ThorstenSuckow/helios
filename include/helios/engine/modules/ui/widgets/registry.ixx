/**
 * @file registry.ixx
 * @brief Component registration for UI widgets module.
 */
module;

export module helios.engine.modules.ui.widgets.registry;

import helios.engine.ecs.ComponentReflector;
import helios.engine.modules.ui.widgets.components;

export namespace helios::engine::modules::ui::widgets {

    /**
     * @brief Registers all UI widget components with the ComponentReflector.
     */
    inline void registerComponents() {
        using R = helios::engine::ecs::ComponentReflector;

        R::registerType<components::UiTextComponent>();
        R::registerType<components::MenuComponent>();
    }

}

