/**
 * @file SchedulerBuilder.ixx
 * @brief Builder for configuring SpawnSchedulers within the spawn system.
 */
module;

#include <memory>
#include <array>
#include <utility>

export module helios.engine.builder.spawnSystem.builders.SchedulerBuilder;

import helios.runtime.spawn.types.SpawnProfileId;
import helios.runtime.spawn.types.SpawnRuleId;
import helios.runtime.spawn.SpawnManager;
import helios.runtime.spawn.scheduling.SpawnScheduler;
import helios.runtime.spawn.scheduling.DefaultSpawnScheduler;
import helios.runtime.spawn.scheduling.CyclicSpawnScheduler;
import helios.engine.builder.spawnSystem.builders.configs;

export namespace helios::engine::builder::spawnSystem::builders {

    /**
     * @brief Builder for configuring and registering SpawnSchedulers.
     *
     * Provides factory methods for creating default and cyclic
     * schedulers, and adding rules to them via fluent SchedulerConfig.
     */
    template<typename THandle>
    class SchedulerBuilder {

        /**
         * @brief The spawn manager to register schedulers with.
         */
        helios::runtime::spawn::SpawnManager<THandle>& spawnManager_;

        /**
         * @brief Adds rules from a parameter pack to a scheduler.
         *
         * @tparam Scheduler The scheduler type.
         * @tparam Configs The SchedulerConfig parameter pack.
         * @param scheduler The scheduler to add rules to.
         * @param configs The rule configurations.
         */
        template<typename Scheduler, typename... Configs>
        void addRules(Scheduler& scheduler, Configs&&... configs) {
            (scheduler.addRule(
                configs.profileId(), configs.build()
            ), ...);
        }

    public:

        /**
         * @brief Constructs a SchedulerBuilder.
         *
         * @param spawnManager The spawn manager to register schedulers with.
         */
        explicit SchedulerBuilder(
            helios::runtime::spawn::SpawnManager<THandle>& spawnManager
        ) : spawnManager_(spawnManager) {}

        /**
         * @brief Creates and registers a DefaultSpawnScheduler with the given rules.
         *
         * @tparam Configs SchedulerConfig parameter pack.
         * @param configs Rule configurations to add.
         */
        template<typename... Configs>
        void defaultScheduler(Configs&&... configs) {
            auto scheduler = std::make_unique<
                helios::runtime::spawn::scheduling::DefaultSpawnScheduler<THandle>>();

            addRules(*scheduler, std::forward<Configs>(configs)...);

            spawnManager_.addScheduler(std::move(scheduler));
        }

        /**
         * @brief Creates and registers a CyclicSpawnScheduler with the given rules.
         *
         * The number of rules determines the template parameter N.
         *
         * @tparam Configs SchedulerConfig parameter pack.
         * @param configs Rule configurations to add in cycle order.
         */
        template<typename... Configs>
        void cyclicScheduler(Configs&&... configs) {
            constexpr std::size_t N = sizeof...(Configs);

            auto scheduler = std::make_unique<
                helios::runtime::spawn::scheduling::CyclicSpawnScheduler<THandle, N>>();

            addRules(*scheduler, std::forward<Configs>(configs)...);

            spawnManager_.addScheduler(std::move(scheduler));
        }

        /**
         * @brief Registers a custom scheduler directly.
         *
         * @param scheduler Ownership is transferred.
         */
        void customScheduler(
            std::unique_ptr<helios::runtime::spawn::scheduling::SpawnScheduler<THandle>> scheduler
        ) {
            spawnManager_.addScheduler(std::move(scheduler));
        }
    };

}
