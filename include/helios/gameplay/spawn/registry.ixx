/**
 * @file registry.ixx
 * @brief Component registration for spawn module.
 */
module;

export module helios.gameplay.spawn.registry;

import helios.ecs.ComponentReflector;
import helios.gameplay.spawn.components;

export namespace helios::gameplay::spawn {

    /**
     * @brief Registers all spawn components with the ComponentReflector.
     */
    template<typename TEntityManager>
    inline void registerComponents() {
        using R = helios::ecs::ComponentReflector<TEntityManager>;

        R::template registerType<components::SpawnedByProfileComponent<typename TEntityManager::Handle_type>>();
        R::template registerType<components::EmittedByComponent<typename TEntityManager::Handle_type>>();
    }

}

