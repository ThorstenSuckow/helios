/**
 * @file SpawnPoolConfig.ixx
 * @brief Fluent configuration for a complete spawn pool with profiles and scheduling.
 *
 * @details This file contains SpawnSystemConfigurator, SpawnRuleConfig,
 * SpawnProfileConfig, and SpawnPoolConfig in a single translation unit.
 * These classes form a cyclic done()-chaining dependency
 * (Rule → Profile → Pool → Configurator → Pool) so we will not split them in separate files.
 */
module;

#include <memory>
#include <vector>

export module helios.engine.builder.spawnSystem.builders.configs.SpawnPoolConfig;

import helios.engine.ecs;

import helios.engine.core.data;

import helios.engine.runtime.pooling;

import helios.engine.runtime.spawn;

import helios.math;

export namespace helios::engine::builder::spawnSystem::builders::configs {

    // Forward declarations for nested builder return types.
    class SpawnPoolConfig;
    class SpawnProfileConfig;
    class SpawnRuleConfig;

    /**
     * @brief Lightweight handle for chaining pool configurations.
     *
     * Returned by commit() and commitCyclic() to allow starting
     * the next pool() call. Holds only the two manager references.
     */
    class SpawnSystemConfigurator {

        /**
         * @brief The pool manager for pool registration.
         */
        helios::engine::runtime::pooling::GameObjectPoolManager& poolManager_;

        /**
         * @brief The spawn manager for profile and scheduler registration.
         */
        helios::engine::runtime::spawn::SpawnManager& spawnManager_;

    public:

        /**
         * @brief Constructs a configurator with the given managers.
         *
         * @param poolManager The pool manager for pool registration.
         * @param spawnManager The spawn manager for profile and scheduler registration.
         */
        SpawnSystemConfigurator(
            helios::engine::runtime::pooling::GameObjectPoolManager& poolManager,
            helios::engine::runtime::spawn::SpawnManager& spawnManager
        ) noexcept : poolManager_(poolManager), spawnManager_(spawnManager) {}

        /**
         * @brief Begins ID-centric configuration of a spawn pool.
         *
         * @param poolId Unique pool identifier.
         * @param prefab Template GameObject for cloning.
         * @param poolSize Number of instances to pre-allocate.
         *
         * @return SpawnPoolConfig for fluent configuration.
         */
        [[nodiscard]] SpawnPoolConfig pool(
            helios::engine::core::data::GameObjectPoolId poolId,
            helios::engine::ecs::GameObject prefab,
            size_t poolSize
        );
    };

    /**
     * @brief Fluent configuration for a spawn rule bound to a profile.
     *
     * Configures the condition and amount for a scheduled spawn rule.
     * Returns to the parent SpawnProfileConfig via done().
     */
    class SpawnRuleConfig {

        /**
         * @brief Parent profile this rule belongs to.
         */
        SpawnProfileConfig& parent_;

        /**
         * @brief Unique identifier for this rule.
         */
        helios::engine::core::data::SpawnRuleId ruleId_;

        /**
         * @brief Condition determining when to spawn.
         */
        std::unique_ptr<const helios::engine::runtime::spawn::policy::SpawnCondition> condition_;

        /**
         * @brief Provider determining how many to spawn.
         */
        std::unique_ptr<const helios::engine::runtime::spawn::policy::amount::SpawnAmountProvider> amountProvider_;

    public:

        /**
         * @brief Constructs a SpawnRuleConfig.
         *
         * @param parent The parent profile config.
         * @param ruleId Unique identifier for this rule.
         */
        SpawnRuleConfig(
            SpawnProfileConfig& parent,
            helios::engine::core::data::SpawnRuleId ruleId
        ) : parent_(parent), ruleId_(ruleId),
            condition_(nullptr), amountProvider_(nullptr) {}

        /**
         * @brief Sets a timer-based spawn condition.
         *
         * @param intervalSeconds Seconds between spawn evaluations.
         *
         * @return Reference to this config for chaining.
         */
        SpawnRuleConfig& timerCondition(const float intervalSeconds) {
            condition_ = std::make_unique<
                helios::engine::runtime::spawn::policy::conditions::TimerSpawnCondition>(
                    intervalSeconds);
            return *this;
        }

        /**
         * @brief Sets a combined condition: timer + pool availability.
         *
         * @param intervalSeconds Seconds between spawn evaluations.
         *
         * @return Reference to this config for chaining.
         */
        SpawnRuleConfig& timerWithAvailabilityCondition(const float intervalSeconds) {
            condition_ = std::make_unique<
                helios::engine::runtime::spawn::policy::SpawnConditionAll>(
                    std::make_unique<helios::engine::runtime::spawn::policy::conditions::TimerSpawnCondition>(intervalSeconds),
                    std::make_unique<helios::engine::runtime::spawn::policy::conditions::RequestedAmountIsAvailableCondition>()
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
        SpawnRuleConfig& condition(
            std::unique_ptr<const helios::engine::runtime::spawn::policy::SpawnCondition> customCondition
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
        SpawnRuleConfig& fixedAmount(const size_t count) {
            amountProvider_ = std::make_unique<
                helios::engine::runtime::spawn::policy::amount::FixedSpawnAmount>(count);
            return *this;
        }

        /**
         * @brief Sets a custom amount provider.
         *
         * @param customProvider Ownership is transferred.
         *
         * @return Reference to this config for chaining.
         */
        SpawnRuleConfig& amount(
            std::unique_ptr<const helios::engine::runtime::spawn::policy::amount::SpawnAmountProvider> customProvider
        ) {
            amountProvider_ = std::move(customProvider);
            return *this;
        }

        /**
         * @brief Builds the SpawnRule from this configuration.
         *
         * @return The assembled SpawnRule.
         */
        [[nodiscard]] std::unique_ptr<helios::engine::runtime::spawn::policy::SpawnRule> build() {
            return std::make_unique<helios::engine::runtime::spawn::policy::SpawnRule>(
                std::move(condition_),
                std::move(amountProvider_),
                ruleId_
            );
        }

        /**
         * @brief Finalizes the rule and returns to the parent profile config.
         *
         * @return Reference to the parent SpawnProfileConfig.
         */
        SpawnProfileConfig& done();
    };

    /**
     * @brief Fluent configuration for a spawn profile bound to a pool.
     *
     * Configures placement, initialization, and optional scheduling rules
     * for a specific spawn profile. Returns to the parent SpawnPoolConfig
     * via done().
     */
    class SpawnProfileConfig {

        /**
         * @brief Parent pool this profile belongs to.
         */
        SpawnPoolConfig& parent_;

        /**
         * @brief The spawn manager to register with.
         */
        helios::engine::runtime::spawn::SpawnManager& spawnManager_;

        /**
         * @brief Profile identifier.
         */
        helios::engine::core::data::SpawnProfileId profileId_;

        /**
         * @brief Pool to acquire entities from.
         */
        helios::engine::core::data::GameObjectPoolId poolId_;

        /**
         * @brief Placement strategy for spawned entities.
         */
        std::unique_ptr<helios::engine::runtime::spawn::behavior::SpawnPlacer> placer_;

        /**
         * @brief Initialization strategy for spawned entities.
         */
        std::unique_ptr<helios::engine::runtime::spawn::behavior::SpawnInitializer> initializer_;

        /**
         * @brief Scheduled rules for this profile.
         */
        std::vector<std::unique_ptr<SpawnRuleConfig>> rules_;

    public:

        /**
         * @brief Constructs a SpawnProfileConfig.
         *
         * @param parent The parent pool config.
         * @param spawnManager The spawn manager to register profiles with.
         * @param profileId Unique identifier for the profile.
         * @param poolId Pool from which entities are acquired.
         */
        SpawnProfileConfig(
            SpawnPoolConfig& parent,
            helios::engine::runtime::spawn::SpawnManager& spawnManager,
            helios::engine::core::data::SpawnProfileId profileId,
            helios::engine::core::data::GameObjectPoolId poolId
        ) : parent_(parent), spawnManager_(spawnManager),
            profileId_(profileId), poolId_(poolId) {}

        /**
         * @brief Configures emitter-relative placement and initialization.
         *
         * @return Reference to this config for chaining.
         */
        SpawnProfileConfig& emitterPlacement() {
            placer_ = std::make_unique<
                helios::engine::runtime::spawn::behavior::placements::EmitterSpawnPlacer>();
            initializer_ = std::make_unique<
                helios::engine::runtime::spawn::behavior::initializers::EmitterInitializer>();
            return *this;
        }

        /**
         * @brief Configures random placement within level bounds.
         *
         * @return Reference to this config for chaining.
         */
        SpawnProfileConfig& randomPlacement() {
            placer_ = std::make_unique<
                helios::engine::runtime::spawn::behavior::placements::RandomSpawnPlacer>();
            return *this;
        }

        /**
         * @brief Configures axis-aligned placement.
         *
         * @param axis Direction axis for spawn distribution.
         * @param origin Starting point of the axis.
         *
         * @return Reference to this config for chaining.
         */
        SpawnProfileConfig& axisPlacement(
            const helios::math::vec3f& axis,
            const helios::math::vec3f& origin
        ) {
            placer_ = std::make_unique<
                helios::engine::runtime::spawn::behavior::placements::AxisSpawnPlacer>(axis, origin);
            return *this;
        }

        /**
         * @brief Sets a custom spawn placer.
         *
         * @param customPlacer Ownership is transferred.
         *
         * @return Reference to this config for chaining.
         */
        SpawnProfileConfig& placer(
            std::unique_ptr<helios::engine::runtime::spawn::behavior::SpawnPlacer> customPlacer
        ) {
            placer_ = std::move(customPlacer);
            return *this;
        }

        /**
         * @brief Configures a move initializer with random direction.
         *
         * @return Reference to this config for chaining.
         */
        SpawnProfileConfig& randomDirectionInitializer() {
            initializer_ = std::make_unique<
                helios::engine::runtime::spawn::behavior::initializers::MoveInitializer>(
                    helios::engine::runtime::spawn::behavior::initializers::DirectionType::Random);
            return *this;
        }

        /**
         * @brief Configures a move initializer with a fixed direction.
         *
         * @param direction The movement direction.
         *
         * @return Reference to this config for chaining.
         */
        SpawnProfileConfig& moveInitializer(const helios::math::vec3f& direction) {
            initializer_ = std::make_unique<
                helios::engine::runtime::spawn::behavior::initializers::MoveInitializer>(direction);
            return *this;
        }

        /**
         * @brief Sets a custom spawn initializer.
         *
         * @param customInitializer Ownership is transferred.
         *
         * @return Reference to this config for chaining.
         */
        SpawnProfileConfig& initializer(
            std::unique_ptr<helios::engine::runtime::spawn::behavior::SpawnInitializer> customInitializer
        ) {
            initializer_ = std::move(customInitializer);
            return *this;
        }

        /**
         * @brief Begins configuration of a scheduled spawn rule for this profile.
         *
         * @param ruleId Unique identifier for the rule.
         *
         * @return Reference to the new rule config for chaining.
         */
        SpawnRuleConfig& scheduledBy(helios::engine::core::data::SpawnRuleId ruleId) {
            rules_.push_back(std::make_unique<SpawnRuleConfig>(*this, ruleId));
            return *rules_.back();
        }

        /**
         * @brief Returns the profile ID.
         *
         * @return The spawn profile ID.
         */
        [[nodiscard]] helios::engine::core::data::SpawnProfileId profileId() const noexcept {
            return profileId_;
        }

        /**
         * @brief Commits the profile and builds any associated rules.
         *
         * @return Vector of built spawn rules (may be empty).
         */
        std::vector<std::pair<helios::engine::core::data::SpawnProfileId,
                              std::unique_ptr<helios::engine::runtime::spawn::policy::SpawnRule>>>
        commit() {
            spawnManager_.addSpawnProfile(
                profileId_,
                std::make_unique<helios::engine::runtime::spawn::SpawnProfile>(
                    helios::engine::runtime::spawn::SpawnProfile{
                        .gameObjectPoolId = poolId_,
                        .spawnPlacer = std::move(placer_),
                        .spawnInitializer = std::move(initializer_)
                    }
                )
            );

            std::vector<std::pair<helios::engine::core::data::SpawnProfileId,
                                  std::unique_ptr<helios::engine::runtime::spawn::policy::SpawnRule>>> result;
            for (auto& rule : rules_) {
                result.emplace_back(profileId_, rule->build());
            }
            return result;
        }

        /**
         * @brief Finalizes the profile and returns to the parent pool config.
         *
         * @return Reference to the parent SpawnPoolConfig.
         */
        SpawnPoolConfig& done();
    };

    /**
     * @brief ID-centric configuration for a spawn pool with associated profiles.
     *
     * @details Bundles a pool ID with its prefab, pool size, and one or more
     * spawn profiles. Each profile can optionally have scheduled rules.
     * Calling done() commits the pool, all profiles, and registers any
     * scheduled rules with a DefaultSpawnScheduler.
     *
     * @see SpawnProfileConfig
     * @see SpawnRuleConfig
     * @see SpawnSystemFactory
     */
    class SpawnPoolConfig {

        /**
         * @brief The pool manager to register with.
         */
        helios::engine::runtime::pooling::GameObjectPoolManager& poolManager_;

        /**
         * @brief The spawn manager to register profiles with.
         */
        helios::engine::runtime::spawn::SpawnManager& spawnManager_;

        /**
         * @brief Pool identifier.
         */
        helios::engine::core::data::GameObjectPoolId poolId_;

        /**
         * @brief Prefab template for cloning.
         */
        helios::engine::ecs::GameObject prefab_;

        /**
         * @brief Number of instances to pre-allocate.
         */
        size_t size_;

        /**
         * @brief Profile configurations attached to this pool.
         */
        std::vector<std::unique_ptr<SpawnProfileConfig>> profiles_;

    public:

        /**
         * @brief Constructs a SpawnPoolConfig.
         *
         * @param poolManager The pool manager to register pools with.
         * @param spawnManager The spawn manager to register profiles with.
         * @param poolId Unique identifier for the pool.
         * @param prefab Template GameObject for cloning.
         * @param poolSize Number of instances to pre-allocate.
         */
        SpawnPoolConfig(
            helios::engine::runtime::pooling::GameObjectPoolManager& poolManager,
            helios::engine::runtime::spawn::SpawnManager& spawnManager,
            helios::engine::core::data::GameObjectPoolId poolId,
            helios::engine::ecs::GameObject prefab,
            size_t poolSize
        ) : poolManager_(poolManager), spawnManager_(spawnManager),
            poolId_(poolId), prefab_(prefab), size_(poolSize) {}

        /**
         * @brief Begins configuration of a spawn profile for this pool.
         *
         * @param profileId Unique identifier for the profile.
         *
         * @return Reference to the new profile config for chaining.
         */
        SpawnProfileConfig& profile(helios::engine::core::data::SpawnProfileId profileId) {
            profiles_.push_back(std::make_unique<SpawnProfileConfig>(
                *this, spawnManager_, profileId, poolId_
            ));
            return *profiles_.back();
        }

        /**
         * @brief Commits pool, all profiles, and registers scheduled rules.
         *
         * Profiles with scheduled rules are automatically registered
         * with a DefaultSpawnScheduler per profile.
         *
         * @return Configurator for chaining the next pool() call.
         */
        SpawnSystemConfigurator commit() {
            commitPool();
            commitProfiles(false);
            return SpawnSystemConfigurator{poolManager_, spawnManager_};
        }

        /**
         * @brief Commits pool and profiles without creating any schedulers.
         *
         * @details Use this when profiles should be registered but scheduling
         * is handled separately (e.g., via SchedulerBuilder). Rules attached
         * via scheduledBy() are silently discarded.
         *
         * This enables reuse of the same profile configuration under different
         * scheduling strategies:
         *
         * ```cpp
         * // Register pool + profiles (no schedulers)
         * spawns.pool(PoolId, prefab, 100)
         *     .profile(LeftId).axisPlacement(...).moveInitializer(...).done()
         *     .profile(RightId).axisPlacement(...).moveInitializer(...).done()
         *     .commitProfilesOnly();
         *
         * // Attach scheduling separately
         * SchedulerBuilder sb(spawnManager);
         * sb.cyclicScheduler(
         *     SchedulerConfig(LeftId, Rule1).timerCondition(5.0f).fixedAmount(10),
         *     SchedulerConfig(RightId, Rule2).timerCondition(5.0f).fixedAmount(10)
         * );
         * ```
         *
         * @return Configurator for chaining the next pool() call.
         */
        SpawnSystemConfigurator commitProfilesOnly() {
            commitPool();
            commitProfiles(true);
            return SpawnSystemConfigurator{poolManager_, spawnManager_};
        }

        /**
         * @brief Commits with all scheduled rules in a single CyclicSpawnScheduler.
         *
         * @details Use this when multiple profiles share a cyclic spawn
         * pattern (e.g., wave-based edge spawning). All rules across all
         * profiles are bundled into one CyclicSpawnScheduler.
         *
         * @tparam N Number of rules in the cyclic scheduler.
         *
         * @return Configurator for chaining the next pool() call.
         */
        template<std::size_t N>
        SpawnSystemConfigurator commitCyclic() {
            commitPool();

            auto scheduler = std::make_unique<
                helios::engine::runtime::spawn::scheduling::CyclicSpawnScheduler<N>>();

            for (auto& profileConfig : profiles_) {
                auto rules = profileConfig->commit();
                for (auto& [profileId, rule] : rules) {
                    scheduler->addRule(profileId, std::move(rule));
                }
            }

            spawnManager_.addScheduler(std::move(scheduler));
            return SpawnSystemConfigurator{poolManager_, spawnManager_};
        }

    private:

        /**
         * @brief Registers the pool with the pool manager.
         */
        void commitPool() {
            poolManager_.addPoolConfig(
                std::make_unique<helios::engine::runtime::pooling::GameObjectPoolConfig>(
                    poolId_, prefab_, size_
                )
            );
        }

        /**
         * @brief Commits all profiles and optionally creates per-profile schedulers.
         *
         * @param skipSchedulers If true, profiles are committed without scheduler creation.
         */
        void commitProfiles(bool skipSchedulers) {
            for (auto& profileConfig : profiles_) {
                auto rules = profileConfig->commit();
                if (!skipSchedulers && !rules.empty()) {
                    auto scheduler = std::make_unique<
                        helios::engine::runtime::spawn::scheduling::DefaultSpawnScheduler>();
                    for (auto& [profileId, rule] : rules) {
                        scheduler->addRule(profileId, std::move(rule));
                    }
                    spawnManager_.addScheduler(std::move(scheduler));
                }
            }
        }
    };

    // Out-of-line definitions for done() and pool() methods.

    inline SpawnProfileConfig& SpawnRuleConfig::done() {
        return parent_;
    }

    inline SpawnPoolConfig& SpawnProfileConfig::done() {
        return parent_;
    }

    inline SpawnPoolConfig SpawnSystemConfigurator::pool(
        helios::engine::core::data::GameObjectPoolId poolId,
        helios::engine::ecs::GameObject prefab,
        size_t poolSize
    ) {
        return SpawnPoolConfig{poolManager_, spawnManager_, poolId, prefab, poolSize};
    }

}







