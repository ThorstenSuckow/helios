/**
 * @file SpawnPlanRequestExecutedEvent.ixx
 * @brief Event indicating a spawn plan has been executed.
 */
module;

export module helios.engine.runtime.spawn.events.SpawnPlanRequestExecutedEvent;

import helios.engine.core.data.SpawnRuleId;

export namespace helios::engine::runtime::spawn::events {

    /**
     * @brief Event indicating a spawn plan has been executed.
     *
     * @details SpawnPlanRequestExecutedEvent is pushed to the frame event bus
     * after a spawn request completes. The GameObjectSpawnSystem reads these
     * events in the next frame to commit spawn counts to the SpawnScheduler.
     *
     * @see SpawnScheduler::commit
     * @see GameObjectSpawnSystem
     */
    struct SpawnPlanRequestExecutedEvent {

        /**
         * @brief The rule that triggered the spawn.
         */
        const helios::engine::core::data::SpawnRuleId spawnRuleId;

        /**
         * @brief Number of entities actually spawned.
         */
        const size_t spawnCount;

    };

}
