/**
 * @file registry.ixx
 * @brief Component registration for AI module.
 */
module;

export module helios.gameplay.ai.registry;

import helios.ecs.ComponentReflector;
import helios.gameplay.ai.components;

export namespace helios::gameplay::ai {

    /**
     * @brief Registers all AI components with the ComponentReflector.
     */
    template<typename TEntityManager>
    inline void registerComponents() {
        using R = helios::ecs::ComponentReflector<TEntityManager>;

        R::template registerType<components::ChaseComponent<typename TEntityManager::Handle_type>>();
    }

}

