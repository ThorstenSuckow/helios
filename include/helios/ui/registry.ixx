/**
 * @file registry.ixx
 * @brief Component registration for entire UI module.
 */
module;

export module helios.ui.registry;

import helios.ui.widgets.registry;
import helios.ui.transform.registry;

export namespace helios::ui {

    /**
     * @brief Registers all UI components with the ComponentReflector.
     */
    template<typename TEntityManager>
    inline void registerComponents() {
        helios::ui::widgets::registerComponents<TEntityManager>();
        helios::ui::transform::registerComponents<TEntityManager>();
    }

}

