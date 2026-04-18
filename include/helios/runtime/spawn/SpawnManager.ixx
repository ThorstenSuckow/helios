/**
 * @file SpawnManager.ixx
 * @brief Manager for processing spawn and despawn commands from pools.
 */
module;

#include <cassert>
#include <memory>
#include <span>
#include <unordered_map>
#include <vector>

export module helios.runtime.spawn.SpawnManager;

import helios.runtime.spawn.types.SpawnPlanCursor;
import helios.runtime.spawn.types.SpawnProfile;
import helios.runtime.spawn.types.SpawnContext;
import helios.runtime.spawn.events.SpawnPlanCommandExecutedEvent;


import helios.runtime.spawn.scheduling.SpawnScheduler;

import helios.gameplay.spawn.components.EmittedByComponent;

import helios.runtime.spawn.commands.SpawnCommand;
import helios.runtime.spawn.commands.ScheduledSpawnPlanCommand;
import helios.runtime.spawn.commands.DespawnCommand;

import helios.engine.modules.rendering.model.components.ModelAabbComponent;
import helios.engine.modules.spatial.transform.components.ScaleStateComponent;
import helios.engine.modules.spatial.transform.components.RotationStateComponent;

import helios.engine.modules.physics.collision.Bounds;
import helios.scene.components.SceneNodeComponent;

import helios.runtime.world.GameWorld;
import helios.runtime.world.UpdateContext;
import helios.runtime.pooling.GameObjectPoolManager;


import helios.runtime.spawn.types;
import helios.runtime.world.GameObject;

import helios.engine.modules.spatial.transform.components.TranslationStateComponent;
import helios.gameplay.spawn.components.SpawnedByProfileComponent;

import helios.engine.modules.physics.collision.components.AabbColliderComponent;

import helios.math;

import helios.engine.common.tags.ManagerRole;

using namespace helios::runtime::spawn::commands;
using namespace helios::runtime::spawn::types;
export namespace helios::runtime::spawn {

    /**
     * @brief Manager for processing typed spawn and despawn commands.
     *
     * @details `SpawnManager` handles pooled entity lifecycle through
     * `SpawnCommand<THandle>`, `DespawnCommand<THandle>`, and
     * `ScheduledSpawnPlanCommand<THandle>`. Commands are queued via `submit(...)`
     * and executed during `flush(...)`.
     *
     * Processing order in `flush(...)`:
     * 1. despawn commands
     * 2. direct spawn commands
     * 3. scheduled spawn-plan commands
     *
     * @tparam THandle Handle type for spawned/despawned entities.
     *
     * @see SpawnCommand
     * @see DespawnCommand
     * @see ScheduledSpawnPlanCommand
     * @see SpawnProfile
     */
    template<typename THandle>
    class SpawnManager {


        /**
         * @brief Collection of schedulers that manage spawn rules and conditions.
         *
         * @details Each scheduler owns and evaluates its registered spawn rules
         * independently. When conditions are met, the scheduler produces
         * ScheduledSpawnPlan instances for execution. Multiple schedulers allow
         * grouping spawn rules by category (e.g., enemies, powerups, projectiles).
         */
        std::vector<std::unique_ptr<helios::runtime::spawn::scheduling::SpawnScheduler<THandle>>> spawnSchedulers_;

        /**
         * @brief Queue of pending spawn commands.
         */
        std::vector<SpawnCommand<THandle>> spawnCommands_;

        /**
         * @brief Queue of pending despawn commands.
         */
        std::vector<DespawnCommand<THandle>> despawnCommands_;

        /**
         * @brief Queue of pending scheduled spawn plan commands.
         */
        std::vector<ScheduledSpawnPlanCommand<THandle>> scheduledSpawnPlanCommands_;

        /**
         * @brief Pointer to the pool manager for acquire/release operations.
         */
        helios::runtime::pooling::GameObjectPoolManager<THandle>* gameObjectPoolManager_ = nullptr;

        /**
         * @brief Map from profile IDs to their spawn profiles.
         */
        std::unordered_map<
            helios::runtime::spawn::types::SpawnProfileId,
            std::unique_ptr<const SpawnProfile<THandle>>> spawnProfiles_;

        /**
         * @brief Ensures that the bounds are properly computed..
         *
         * @details Checks if the bounding box was initialized (i.e. has valid
         * min/max values: min <= max).
         * If the bounds are inverted (min > max), it recomputes the world AABB
         * using the GameObject's components (ModelAabb, ScaleState, RotationState,
         * SceneNode, TranslationState).
         *
         * @param go The GameObject to compute bounds for.
         * @param bounds The bounding box to check and potentially update.
         *
         * @see helios::engine::modules::physics::collision::Bounds::computeWorldAabb
         */
        void ensureBounds(helios::runtime::world::GameObject go, helios::math::aabbf& bounds) {
            if (bounds.min()[0] > bounds.max()[0]) {
                const auto* mab   = go.get<helios::engine::modules::rendering::model::components::ModelAabbComponent>();
                const auto* sca    = go.get<helios::engine::modules::spatial::transform::components::ScaleStateComponent>();
                auto* rsc = go.get<helios::engine::modules::spatial::transform::components::RotationStateComponent>();
                const auto* scn   = go.get<helios::scene::components::SceneNodeComponent>();
                const auto* tsc   = go.get<helios::engine::modules::spatial::transform::components::TranslationStateComponent>();

                assert(mab && scn && tsc && sca && rsc && "Missing Components for AABB computation");
                bounds = helios::engine::modules::physics::collision::Bounds::computeWorldAabb(
                   *mab, *scn, *tsc, *sca, *rsc
                );
            }
        }

        /**
         * @brief Processes scheduled spawn plan commands.
         *
         * @details Iterates through each plan, acquires entities from the pool,
         * positions them using the profile's placer, initializes them using
         * the profile's initializer, and pushes a frame event for confirmation.
         *
         * @param commands Span of scheduled spawn plan commands to process.
         * @param updateContext The current update context.
         */
        void executeScheduledSpawnPlanCommands(
            std::span<ScheduledSpawnPlanCommand<THandle>> commands,
            helios::runtime::world::UpdateContext& updateContext
        ) {

            for (auto& scheduledSpawnPlanCommand: commands) {

                const auto& spawnPlan          = scheduledSpawnPlanCommand.spawnPlan();

                const auto spawnProfileId   = scheduledSpawnPlanCommand.spawnProfileId();
                const auto spawnRuleId      = spawnPlan.spawnRuleId;
                const auto amount     = spawnPlan.amount;

                const auto it = spawnProfiles_.find(spawnProfileId);
                assert(it != spawnProfiles_.end() && "SpawnProfile not part of SpawnManager");

                const auto spawnProfile = it->second.get();

                const auto gameObjectPoolId = spawnProfile->gameObjectPoolId;



                const auto poolSnapshot = gameObjectPoolManager_->poolSnapshot(gameObjectPoolId);

                if (amount == 0) {
                    assert(false && "Amount must not be 0");
                }
                if (!spawnProfiles_.contains(spawnProfileId)) {
                    assert(false && "SpawnManager does not manage this SpawnProfileId");
                }



                const auto spawnCount = std::min(amount, poolSnapshot.inactiveCount);
                for (size_t i = 0; i < spawnCount; i++) {

                    auto go = gameObjectPoolManager_->acquire(gameObjectPoolId);
                    assert(go && "Failed to acquire GameObject");

                    auto* tsc = go->get<helios::engine::modules::spatial::transform::components::TranslationStateComponent>();

                    auto* sbp = go->get<helios::gameplay::spawn::components::SpawnedByProfileComponent>();
                    assert(sbp && "unexpected missing SpawnedByProfileComponent");

                    auto* aabb = go->get<helios::engine::modules::physics::collision::components::AabbColliderComponent>();
                    assert(aabb && "unexpected missing AabbColliderComponent");

                    auto spawnCursor = SpawnPlanCursor{spawnCount, i};
                    const auto& spawnContext =  scheduledSpawnPlanCommand.spawnContext();

                    const auto& emitter = spawnContext.emitterContext;
                    auto* ebc = go->get<helios::gameplay::spawn::components::EmittedByComponent>();
                    if (emitter.has_value() && ebc) {
                        ebc->setSource(emitter.value().source);
                    }

                    if (tsc) {
                        
                        auto bounds = aabb->bounds();
                        ensureBounds(go.value(), bounds);

                        const auto position = spawnProfile->spawnPlacer->getPosition(
                            go->handle(),
                            bounds,
                            updateContext.level()->bounds(),
                            spawnCursor,
                            spawnContext

                        );
                        tsc->setTranslation(position);
                    }

                    assert(spawnProfile->spawnInitializer && "Unexpected missing spawn initializer");

                    spawnProfile->spawnInitializer->initialize(*go, spawnCursor, spawnContext);
                    sbp->setSpawnProfileId(spawnProfileId);
                    go->setActive(true);
                }

                updateContext.pushFrame<helios::runtime::spawn::events::SpawnPlanCommandExecutedEvent>(
                    spawnRuleId, spawnCount
                );
            }
        }


        /**
         * @brief Processes spawn commands, acquiring and initializing objects.
         *
         * @param commands Span of spawn commands to process.
         * @param updateContext The current update context.
         */
        void spawnObjects(
            std::span<SpawnCommand<THandle>> commands,
            helios::runtime::world::UpdateContext& updateContext) {

            for (auto& spawnCommand: commands) {
                const auto spawnProfileId = spawnCommand.spawnProfileId();
                const auto spawnContext   = spawnCommand.spawnContext();

                const auto it = spawnProfiles_.find(spawnProfileId);
                assert(it != spawnProfiles_.end() && "SpawnProfile not part of SpawnManager");

                const auto spawnProfile = it->second.get();
                const auto gameObjectPoolId = spawnProfile->gameObjectPoolId;

                if (gameObjectPoolManager_->poolSnapshot(gameObjectPoolId).inactiveCount == 0) {
                    /**
                     * @todo log
                     */
                    continue;
                }

                auto go = gameObjectPoolManager_->acquire(gameObjectPoolId);
                assert(go && "Failed to acquire GameObject");

                auto* tsc = go->get<helios::engine::modules::spatial::transform::components::TranslationStateComponent>();
                auto* sbp = go->get<helios::gameplay::spawn::components::SpawnedByProfileComponent>();
                assert(sbp && "unexpected missing SpawnedByProfileComponent");

                auto* aabb = go->get<helios::engine::modules::physics::collision::components::AabbColliderComponent>();
                assert(aabb && "unexpected missing AabbColliderComponent");

                const auto& emitter = spawnContext.emitterContext;
                auto* ebc = go->get<helios::gameplay::spawn::components::EmittedByComponent>();
                if (emitter.has_value() && ebc) {
                    ebc->setSource(emitter.value().source);
                }

                if (tsc) {

                    auto bounds = aabb->bounds();
                    ensureBounds(go.value(), bounds);

                    const auto position = spawnProfile->spawnPlacer->getPosition(
                        go->handle(),
                        bounds,
                        updateContext.level()->bounds(),
                        {1, 1},
                        spawnContext

                    );
                    tsc->setTranslation(position);
                }


                assert(spawnProfile->spawnInitializer && "Unexpected missing spawn initializer");

                spawnProfile->spawnInitializer->initialize(*go, {1, 1}, spawnContext);
                sbp->setSpawnProfileId(spawnProfileId);

                go->setActive(true);
            }
        }


        /**
         * @brief Processes despawn commands, releasing objects back to pools.
         *
         * @param commands Span of despawn commands to process.
         * @param updateContext The current update context.
         */
        void despawnObjects(
            std::span<DespawnCommand<THandle>> commands,
            helios::runtime::world::UpdateContext& updateContext) {

            for (auto& despawnCommand : commands) {
                const auto spawnProfileId = despawnCommand.spawnProfileId();

                const auto it = spawnProfiles_.find(spawnProfileId);
                assert(it != spawnProfiles_.end() && "SpawnProfile not part of SpawnManager");
                const auto spawnProfile = it->second.get();
                auto gameObjectPoolId = spawnProfile->gameObjectPoolId;

                gameObjectPoolManager_->release(gameObjectPoolId, despawnCommand.entityHandle());

            }
        }


    public:
        using EngineRoleTag = helios::engine::common::tags::ManagerRole;

        /**
         * @brief Default constructor.
         */
        SpawnManager() = default;

        /**
         * @brief Submits a spawn command for deferred processing.
         *
         * @param command The spawn command to queue.
         *
         * @return Always returns true.
         */
        bool submit(const SpawnCommand<THandle> command) noexcept {
            spawnCommands_.push_back(command);
            return true;
        }

        /**
         * @brief Adds a spawn scheduler to this manager.
         *
         * @details Schedulers evaluate spawn rules and produce spawn plans based
         * on game conditions. Multiple schedulers can be added for different
         * spawn categories.
         *
         * @param scheduler The scheduler to add. Ownership is transferred.
         */
        void addScheduler(std::unique_ptr<helios::runtime::spawn::scheduling::SpawnScheduler<THandle>> scheduler) {
            spawnSchedulers_.push_back(std::move(scheduler));
        }

        /**
         * @brief Submits a despawn command for deferred processing.
         *
         * @param command The despawn command to queue.
         *
         * @return Always returns true.
         */
        bool submit(const DespawnCommand<THandle> command) noexcept {
            despawnCommands_.push_back(command);
            return true;
        }


        /**
         * @brief Submits a scheduled spawn plan command for deferred processing.
         *
         * @param scheduledSpawnPlanCommand The scheduled plan command to queue.
         *
         * @return Always returns true.
         */
        bool submit(
            const ScheduledSpawnPlanCommand<THandle> scheduledSpawnPlanCommand
        ) noexcept {
            scheduledSpawnPlanCommands_.push_back(scheduledSpawnPlanCommand);
            return true;
        }

        /**
         * @brief Flushes pending commands, processing despawns then spawns.
         *
         * @param updateContext The current update context.
         */
        void flush(
            helios::runtime::world::UpdateContext& updateContext
        ) noexcept {
            if (!despawnCommands_.empty()) {
                despawnObjects(despawnCommands_, updateContext);
                despawnCommands_.clear();
            }

            if (!spawnCommands_.empty()) {
                spawnObjects(spawnCommands_, updateContext);
                spawnCommands_.clear();
            }

            if (!scheduledSpawnPlanCommands_.empty()) {
                executeScheduledSpawnPlanCommands(scheduledSpawnPlanCommands_, updateContext);
                scheduledSpawnPlanCommands_.clear();
            }
        };


        /**
         * @brief Adds a spawn profile for a profile ID.
         *
         * @details Registers a spawn profile that defines how entities should
         * be spawned for this profile. The profile contains the pool ID,
         * placer, and initializer.
         *
         * @param spawnProfileId The unique ID for this profile.
         * @param spawnProfile The profile configuration. Ownership transferred.
         *
         * @return Reference to this manager for chaining.
         *
         * @pre No profile is already registered for this ID.
         */
        SpawnManager& addSpawnProfile(
            const SpawnProfileId& spawnProfileId,
            std::unique_ptr<const SpawnProfile<THandle>> spawnProfile) {

            assert(!spawnProfiles_.contains(spawnProfileId) && "SpawnProfileId already added");

            spawnProfiles_[spawnProfileId] = std::move(spawnProfile);

            return *this;
        }

        /**
         * @brief Returns a spawn profile by ID.
         *
         * @param spawnProfileId The profile ID to look up.
         *
         * @return Pointer to the profile, or nullptr if not found.
         */
        [[nodiscard]] const SpawnProfile<THandle>* spawnProfile(
            const SpawnProfileId& spawnProfileId) const {

            if (!spawnProfiles_.contains(spawnProfileId)) {
                return nullptr;
            }

            return spawnProfiles_.find(spawnProfileId)->second.get();
        }

        /**
         * @brief Initializes the manager with required runtime resources.
         *
         * @details Resolves `GameObjectPoolManager<THandle>` and registers
         * command handlers for typed spawn command variants.
         *
         * @param gameWorld Game world used for resource lookup and handler registration.
         */
        void init(helios::runtime::world::GameWorld& gameWorld) noexcept {

            assert(gameWorld.hasManager<helios::runtime::pooling::GameObjectPoolManager<THandle>>() && "Unexpected missing GameObjectPoolManager");
            gameObjectPoolManager_ = gameWorld.tryManager<helios::runtime::pooling::GameObjectPoolManager<THandle>>();

            gameWorld.registerCommandHandler<
                SpawnCommand<THandle>,
                DespawnCommand<THandle>,
                ScheduledSpawnPlanCommand<THandle>
            >(*this);
        }

        /**
         * @brief Resets all schedulers, profiles, and pending commands.
         *
         * @details Called during level transitions or game restarts to clear
         * all spawn state. Resets each scheduler, calls `onReset()` on all
         * placers and initializers, and clears all pending command queues.
         */
        void reset() {

            for (auto& scheduler: spawnSchedulers_) {
                scheduler->reset();
            }

            for (auto& [_, profile]: spawnProfiles_) {
                profile->spawnPlacer->onReset();
                profile->spawnInitializer->onReset();
            }


            despawnCommands_.clear();
            spawnCommands_.clear();
            scheduledSpawnPlanCommands_.clear();
        }

        /**
         * @brief Returns a span of all registered spawn schedulers.
         *
         * @return Span of spawn scheduler unique pointers.
         */
        std::span<std::unique_ptr<helios::runtime::spawn::scheduling::SpawnScheduler<THandle>>> spawnSchedulers() {
            return spawnSchedulers_;
        }
    };


}
