/**
 * @file registry.ixx
 * @brief Component registration for health module.
 */
module;

export module helios.engine.mechanics.health.registry;

import helios.ecs.ComponentReflector;
import helios.engine.mechanics.health.components;

export namespace helios::engine::mechanics::health {

    /**
     * @brief Registers all health components with the ComponentReflector.
     */
    template<typename TEntityManager>
    inline void registerComponents() {
        using R = helios::ecs::ComponentReflector<TEntityManager>;

        R::template registerType<components::HealthComponent<typename TEntityManager::Handle_type>>();
    }

}

