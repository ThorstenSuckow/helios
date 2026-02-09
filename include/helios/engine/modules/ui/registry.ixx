/**
 * @file registry.ixx
 * @brief Component registration for entire UI module.
 */
module;

export module helios.engine.modules.ui.registry;

import helios.engine.modules.ui.widgets.registry;
import helios.engine.modules.ui.transform.registry;

export namespace helios::engine::modules::ui {

    /**
     * @brief Registers all UI components with the ComponentReflector.
     */
    inline void registerComponents() {
        helios::engine::modules::ui::widgets::registerComponents();
        helios::engine::modules::ui::transform::registerComponents();
    }

}

