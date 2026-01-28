/**
 * @file RuleProcessor.ixx
 * @brief Abstract interface for processing spawn rules.
 */
module;


export module helios.engine.runtime.spawn.scheduling.RuleProcessor;

import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.spawn.SpawnContext;
import helios.engine.runtime.spawn.scheduling.SpawnPlan;
import helios.engine.core.data.SpawnProfileId;
import helios.engine.runtime.spawn.policy.SpawnRule;
import helios.engine.runtime.spawn.policy.SpawnRuleState;


export namespace helios::engine::runtime::spawn::scheduling {

    /**
     * @brief Abstract interface for processing spawn rules into spawn plans.
     *
     * @details RuleProcessor defines the contract for evaluating a single spawn
     * rule and producing a SpawnPlan. This abstraction allows different processing
     * strategies to be plugged into schedulers.
     *
     * ## Responsibilities
     *
     * - Retrieve pool and profile information from the game world
     * - Update the rule's state (timers, counts)
     * - Evaluate the rule's conditions
     * - Produce a SpawnPlan if conditions are met
     *
     * @see DefaultRuleProcessor
     * @see SpawnScheduler
     * @see SpawnRule
     */
    class RuleProcessor {

    public:

        /**
         * @brief Virtual destructor for proper cleanup.
         */
        virtual ~RuleProcessor() = default;

        /**
         * @brief Processes a spawn rule and produces a spawn plan.
         *
         * @details Evaluates the given rule against current game state and
         * produces a SpawnPlan indicating how many entities should be spawned.
         * A plan with `amount == 0` indicates the rule's conditions were not met.
         *
         * @param updateContext Current frame context with delta time and world access.
         * @param spawnContext Context information for the spawn operation.
         * @param spawnProfileId The profile ID associated with this rule.
         * @param spawnRule The rule to evaluate.
         * @param spawnRuleState Mutable state for the rule (timers, counts).
         *
         * @return SpawnPlan with amount > 0 if spawn should occur, otherwise amount == 0.
         */
        virtual SpawnPlan processRule(
            const helios::engine::runtime::world::UpdateContext& updateContext,
            const helios::engine::runtime::spawn::SpawnContext& spawnContext,
            const helios::engine::core::data::SpawnProfileId spawnProfileId,
            helios::engine::runtime::spawn::policy::SpawnRule& spawnRule,
            helios::engine::runtime::spawn::policy::SpawnRuleState& spawnRuleState
        ) noexcept = 0;


    };

}
