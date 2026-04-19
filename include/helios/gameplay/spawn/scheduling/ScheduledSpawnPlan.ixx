/**
 * @file ScheduledSpawnPlan.ixx
 * @brief A spawn plan that has been scheduled for execution.
 */
module;

export module helios.gameplay.spawn.scheduling.ScheduledSpawnPlan;

import helios.gameplay.spawn.types.SpawnContext;
import helios.gameplay.spawn.scheduling.SpawnPlan;
import helios.gameplay.spawn.types.SpawnProfileId;


using namespace helios::gameplay::spawn::types;
export namespace helios::gameplay::spawn::scheduling {

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
    template<typename THandle>
    struct ScheduledSpawnPlan {

        /**
         * @brief The profile defining spawn behavior (placer, initializer).
         */
        const helios::gameplay::spawn::types::SpawnProfileId spawnProfileId;

        /**
         * @brief The spawn plan with rule, amount, and pool information.
         */
        const SpawnPlan spawnPlan;

        /**
         * @brief The spawn context of the plan.
         */
        const SpawnContext<THandle> spawnContext;

    };

}