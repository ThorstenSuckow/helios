/**
 * @file registry.ixx
 * @brief Component registration for runtime pooling module.
 */
module;

export module helios.runtime.pooling.registry;

import helios.ecs.ComponentReflector;
import helios.runtime.pooling.components;

export namespace helios::runtime::pooling {

    /**
     * @brief Registers all runtime pooling components with the ComponentReflector.
     */
    template<typename TEntityManager>
    inline void registerComponents() {
        using R = helios::ecs::ComponentReflector<TEntityManager>;

        R::template registerType<components::PrefabIdComponent<typename TEntityManager::Handle_type>>();
    }

}


