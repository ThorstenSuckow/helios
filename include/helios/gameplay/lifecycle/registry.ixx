/**
 * @file registry.ixx
 * @brief Component registration for lifecycle module.
 */
module;

export module helios.gameplay.lifecycle.registry;

import helios.ecs.ComponentReflector;
import helios.gameplay.lifecycle.components;

export namespace helios::gameplay::lifecycle {

    /**
     * @brief Registers all lifecycle components with the ComponentReflector.
     */
    template<typename TEntityManager>
    inline void registerComponents() {
        using R = helios::ecs::ComponentReflector<TEntityManager>;

        R::template registerType<components::DeadTagComponent<typename TEntityManager::Handle_type>>();
        R::template registerType<components::DelayedComponentEnabler<typename TEntityManager::Handle_type>>();
        R::template registerType<components::DestroyedComponent<typename TEntityManager::Handle_type>>();
        R::template registerType<components::UninitializedComponent<typename TEntityManager::Handle_type>>();
    }

}

