/**
 * @file registry.ixx
 * @brief Component registration for lifecycle module.
 */
module;

export module helios.engine.mechanics.lifecycle.registry;

import helios.ecs.ComponentReflector;
import helios.engine.mechanics.lifecycle.components;

export namespace helios::engine::mechanics::lifecycle {

    /**
     * @brief Registers all lifecycle components with the ComponentReflector.
     */
    template<typename TEntityManager>
    inline void registerComponents() {
        using R = helios::ecs::ComponentReflector<TEntityManager>;

        R::template registerType<components::DelayedComponentEnabler<typename TEntityManager::Handle_type>>();
    }

}

