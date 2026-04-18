/**
 * @file registry.ixx
 * @brief Component registration for UI widgets module.
 */
module;

export module helios.ui.widgets.registry;

import helios.ecs.ComponentReflector;
import helios.ui.widgets.components;

export namespace helios::ui::widgets {

    /**
     * @brief Registers all UI widget components with the ComponentReflector.
     */
    template<typename TEntityManager>
    inline void registerComponents() {
        using R = helios::ecs::ComponentReflector<TEntityManager>;

        R::template registerType<components::UiTextComponent<typename TEntityManager::Handle_type>>();
        R::template registerType<components::MenuComponent<typename TEntityManager::Handle_type>>();
    }

}

