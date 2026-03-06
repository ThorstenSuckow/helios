/**
 * @file SpawnPlan.ixx
 * @brief Data structure describing a planned spawn operation.
 */
module;

export module helios.engine.runtime.spawn.scheduling.SpawnPlan;

import helios.engine.runtime.pooling.types.GameObjectPoolId;
import helios.engine.runtime.spawn.types.SpawnRuleId;

export namespace helios::engine::runtime::spawn::scheduling {

    /**
     * @brief Data structure describing a planned spawn operation.
     *
     * @details SpawnPlan captures the essential information for a spawn request:
     * which rule triggered it, how many entities to spawn, and from which pool.
     * SpawnPlans are created by the SpawnScheduler when conditions are met.
     *
     * @see SpawnScheduler
     * @see ScheduledSpawnPlan
     * @see SpawnRule
     */
    struct SpawnPlan {

        /**
         * @brief The spawn rule that triggered this plan.
         */
        const helios::engine::runtime::spawn::types::SpawnRuleId spawnRuleId;

        /**
         * @brief Number of entities to spawn.
         */
        const size_t amount;


    };

}