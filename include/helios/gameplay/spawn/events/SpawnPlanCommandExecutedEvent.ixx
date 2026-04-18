/**
 * @file SpawnPlanCommandExecutedEvent.ixx
 * @brief Event indicating a spawn plan has been executed.
 */
module;

export module helios.gameplay.spawn.events.SpawnPlanCommandExecutedEvent;

import helios.gameplay.spawn.types.SpawnRuleId;

export namespace helios::gameplay::spawn::events {

    /**
     * @brief Event indicating a spawn plan command has been executed.
     *
     * @details SpawnPlanCommandExecutedEvent is pushed to the frame event bus
     * after a spawn command completes. The GameObjectSpawnSystem reads these
     * events in the next frame to commit spawn counts to the SpawnScheduler.
     *
     * @see SpawnScheduler::commit
     * @see GameObjectSpawnSystem
     */
    struct SpawnPlanCommandExecutedEvent {

        /**
         * @brief The rule that triggered the spawn.
         */
        const helios::gameplay::spawn::types::SpawnRuleId spawnRuleId;

        /**
         * @brief Number of entities actually spawned.
         */
        const size_t spawnCount;

    };

}
