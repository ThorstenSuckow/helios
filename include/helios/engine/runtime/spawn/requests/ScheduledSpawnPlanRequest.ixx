/**
 * @file ScheduledSpawnPlanRequest.ixx
 * @brief Data structure for scheduled spawn plan requests.
 */
module;

export module helios.engine.runtime.spawn.requests.ScheduledSpawnPlanRequest;

import helios.engine.runtime.spawn.scheduling.ScheduledSpawnPlan;
import helios.engine.runtime.spawn.SpawnContext;

export namespace helios::engine::runtime::spawn::requests {

    /**
     * @brief Data structure representing a scheduled spawn plan request.
     *
     * @details ScheduledSpawnPlanRequest is a lightweight data transfer object
     * used by the ScheduledSpawnPlanCommandDispatcher to communicate spawn
     * parameters to the SpawnManager. It wraps a ScheduledSpawnPlan with
     * additional spawn context.
     *
     * @see ScheduledSpawnPlanCommand
     * @see ScheduledSpawnPlanCommandDispatcher
     * @see SpawnManager
     */
    struct ScheduledSpawnPlanRequest {

        /**
         * @brief The scheduled spawn plan containing profile, rule, and amount.
         */
        const helios::engine::runtime::spawn::scheduling::ScheduledSpawnPlan scheduledSpawnPlan;

        /**
         * @brief Context for spawn placement and initialization.
         *
         * Contains optional EmitterContext for emitter-relative spawning.
         */
        const helios::engine::runtime::spawn::SpawnContext spawnContext;

    };

}