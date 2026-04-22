/**
 * @file SchedulerConfig.ixx
 * @brief Fluent configuration for adding spawn rules to a scheduler.
 */
module;

#include <memory>
#include <cstddef>
#include <memory>

export module helios.gameplay.builder.spawnSystem.builders.configs.SchedulerConfig;

import helios.gameplay.spawn.types.SpawnProfileId;
import helios.gameplay.spawn.types.SpawnRuleId;

import helios.gameplay.spawn.policy.SpawnRule;
import helios.gameplay.spawn.policy.SpawnCondition;
import helios.gameplay.spawn.policy.SpawnConditionAll;
import helios.gameplay.spawn.policy.conditions;
import helios.gameplay.spawn.policy.amount;

export namespace helios::gameplay::builder::spawnSystem::builders::configs {

    /**
     * @brief Fluent configuration for a single spawn rule.
     *
     * Provides convenience methods for assembling a SpawnRule
     * from condition and amount providers.
     */
    template<typename THandle>
    class SchedulerConfig {

        /**
         * @brief Profile this rule is bound to.
         */
        helios::gameplay::spawn::types::SpawnProfileId profileId_;

        /**
         * @brief Unique identifier for the rule.
         */
        helios::gameplay::spawn::types::SpawnRuleId ruleId_;

        /**
         * @brief Condition determining when to spawn.
         */
        std::unique_ptr<const helios::gameplay::spawn::policy::SpawnCondition> condition_;

        /**
         * @brief Provider determining how many to spawn.
         */
        std::unique_ptr<const helios::gameplay::spawn::policy::amount::SpawnAmountProvider<THandle>> amountProvider_;

    public:

        /**
         * @brief Constructs a SchedulerConfig.
         *
         * @param profileId The spawn profile this rule targets.
         * @param ruleId Unique identifier for this rule.
         */
        SchedulerConfig(
            helios::gameplay::spawn::types::SpawnProfileId profileId,
            helios::gameplay::spawn::types::SpawnRuleId ruleId
        ) : profileId_(profileId), ruleId_(ruleId), condition_(nullptr), amountProvider_(nullptr) {}

        /**
         * @brief Sets a timer-based spawn condition.
         *
         * @param intervalSeconds Seconds between spawn evaluations.
         *
         * @return Reference to this config for chaining.
         */
        SchedulerConfig& timerCondition(const float intervalSeconds) {
            condition_ = std::make_unique<
                helios::gameplay::spawn::policy::conditions::TimerSpawnCondition>(
                    intervalSeconds
                );
            return *this;
        }

        /**
         * @brief Sets a combined condition: timer + pool availability.
         *
         * @param intervalSeconds Seconds between spawn evaluations.
         *
         * @return Reference to this config for chaining.
         */
        SchedulerConfig& timerWithAvailabilityCondition(const float intervalSeconds) {
            condition_ = std::make_unique<
                helios::gameplay::spawn::policy::SpawnConditionAll>(
                    std::make_unique<helios::gameplay::spawn::policy::conditions::TimerSpawnCondition>(intervalSeconds),
                    std::make_unique<helios::gameplay::spawn::policy::conditions::RequestedAmountIsAvailableCondition>()
                );
            return *this;
        }

        /**
         * @brief Sets a custom spawn condition.
         *
         * @param customCondition Ownership is transferred.
         *
         * @return Reference to this config for chaining.
         */
        SchedulerConfig& condition(
            std::unique_ptr<const helios::gameplay::spawn::policy::SpawnCondition> customCondition
        ) {
            condition_ = std::move(customCondition);
            return *this;
        }

        /**
         * @brief Sets a fixed spawn amount.
         *
         * @param count Number of entities to spawn per trigger.
         *
         * @return Reference to this config for chaining.
         */
        SchedulerConfig& fixedAmount(const size_t count) {
            amountProvider_ = std::make_unique<helios::gameplay::spawn::policy::amount::FixedSpawnAmount<THandle>>(count);
            return *this;
        }

        /**
         * @brief Sets a custom amount provider.
         *
         * @param customProvider Ownership is transferred.
         *
         * @return Reference to this config for chaining.
         */
        SchedulerConfig& amount(
            std::unique_ptr<const helios::gameplay::spawn::policy::amount::SpawnAmountProvider<THandle>> customProvider
        ) {
            amountProvider_ = std::move(customProvider);
            return *this;
        }

        /**
         * @brief Returns the profile ID for this rule.
         *
         * @return The spawn profile ID.
         */
        [[nodiscard]] helios::gameplay::spawn::types::SpawnProfileId profileId() const noexcept {
            return profileId_;
        }

        /**
         * @brief Builds and returns the configured SpawnRule.
         *
         * @return The assembled SpawnRule. Ownership is transferred to the caller.
         */
        [[nodiscard]] std::unique_ptr<helios::gameplay::spawn::policy::SpawnRule<THandle>> build() {
            return std::make_unique<helios::gameplay::spawn::policy::SpawnRule<THandle>>(
                std::move(condition_),
                std::move(amountProvider_),
                ruleId_
            );
        }
    };

}


