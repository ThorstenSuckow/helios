/**
 * @file registry.ixx
 * @brief Component registration for UI transform module.
 */
module;

export module helios.ui.transform.registry;

import helios.ecs.ComponentReflector;
import helios.ui.transform.components;

export namespace helios::ui::transform {

    /**
     * @brief Registers all UI transform components with the ComponentReflector.
     */
    template<typename TEntityManager>
    inline void registerComponents() {
        using R = helios::ecs::ComponentReflector<TEntityManager>;

        R::template registerType<components::UiTransformComponent<typename TEntityManager::Handle_type>>();
    }

}

