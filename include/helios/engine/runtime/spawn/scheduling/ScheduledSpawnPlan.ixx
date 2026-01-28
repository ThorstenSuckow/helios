/**
 * @file ScheduledSpawnPlan.ixx
 * @brief A spawn plan that has been scheduled for execution.
 */
module;

export module helios.engine.runtime.spawn.scheduling.ScheduledSpawnPlan;

import helios.engine.runtime.spawn.SpawnContext;
import helios.engine.runtime.spawn.scheduling.SpawnPlan;
import helios.engine.core.data.SpawnProfileId;

export namespace helios::engine::runtime::spawn::scheduling {

    /**
     * @brief A spawn plan that has been scheduled for execution.
     *
     * @details ScheduledSpawnPlan pairs a SpawnPlan with its associated
     * SpawnProfileId. This allows the spawn manager to look up the correct
     * SpawnProfile (with its placer and initializer) when executing the spawn.
     *
     * Created by SpawnScheduler when a rule's conditions are met.
     *
     * @see SpawnPlan
     * @see SpawnScheduler
     * @see SpawnProfile
     */
    struct ScheduledSpawnPlan {

        /**
         * @brief The profile defining spawn behavior (placer, initializer).
         */
        const helios::engine::core::data::SpawnProfileId spawnProfileId;

        /**
         * @brief The spawn plan with rule, amount, and pool information.
         */
        const SpawnPlan spawnPlan;

        /**
         * @brief The spawn context of the plan.
         */
        const SpawnContext spawnContext;

    };

}