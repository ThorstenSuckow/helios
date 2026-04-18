/**
 * @file RuleProcessor.ixx
 * @brief Abstract interface for processing spawn rules.
 */
module;


export module helios.runtime.spawn.scheduling.RuleProcessor;

import helios.runtime.world.UpdateContext;
import helios.runtime.world.GameWorld;
import helios.runtime.spawn.types.SpawnContext;
import helios.runtime.spawn.scheduling.SpawnPlan;
import helios.runtime.spawn.types.SpawnProfileId;
import helios.runtime.spawn.policy.SpawnRule;
import helios.runtime.spawn.policy.SpawnRuleState;


using namespace helios::runtime::spawn::policy;
using namespace helios::runtime::spawn::types;
using namespace helios::runtime::world;

export namespace helios::runtime::spawn::scheduling {

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
    template<typename THandle>
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
         * @param gameWorld The game world where the rule is processed.
         * @param updateContext Current frame context with delta time and world access.
         * @param spawnContext Context information for the spawn operation.
         * @param spawnProfileId The profile ID associated with this rule.
         * @param spawnRule The rule to evaluate.
         * @param spawnRuleState Mutable state for the rule (timers, counts).
         *
         * @return SpawnPlan with amount > 0 if spawn should occur, otherwise amount == 0.
         */
        virtual SpawnPlan processRule(
            const GameWorld& gameWorld,
            const UpdateContext& updateContext,
            const SpawnContext<THandle>& spawnContext,
            const SpawnProfileId spawnProfileId,
            SpawnRule<THandle>& spawnRule,
            SpawnRuleState& spawnRuleState
        ) noexcept = 0;


    };

}
