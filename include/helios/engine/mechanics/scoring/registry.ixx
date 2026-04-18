/**
 * @file registry.ixx
 * @brief Component registration for scoring module.
 */
module;

export module helios.engine.mechanics.scoring.registry;

import helios.ecs.ComponentReflector;
import helios.engine.mechanics.scoring.components;
import helios.engine.mechanics.scoring.types;

export namespace helios::engine::mechanics::scoring {

    /**
     * @brief Registers all scoring components with the ComponentReflector.
     */
    template<typename TEntityManager>
    inline void registerComponents() {
        using R = helios::ecs::ComponentReflector<TEntityManager>;

        R::template registerType<components::ScorePoolComponent<typename TEntityManager::Handle_type>>();
        R::template registerType<components::ScoreObserverComponent<typename TEntityManager::Handle_type>>();
        R::template registerType<components::ScoreValueComponent<typename TEntityManager::Handle_type, types::KillReward>>();
    }

}

