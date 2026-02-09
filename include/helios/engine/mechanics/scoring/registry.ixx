/**
 * @file registry.ixx
 * @brief Component registration for scoring module.
 */
module;

export module helios.engine.mechanics.scoring.registry;

import helios.engine.ecs.ComponentReflector;
import helios.engine.mechanics.scoring.components;
import helios.engine.mechanics.scoring.types;

export namespace helios::engine::mechanics::scoring {

    /**
     * @brief Registers all scoring components with the ComponentReflector.
     */
    inline void registerComponents() {
        using R = helios::engine::ecs::ComponentReflector;

        R::registerType<components::ScorePoolComponent>();
        R::registerType<components::ScoreObserverComponent>();
        R::registerType<components::ScoreValueComponent<types::KillReward>>();
    }

}

