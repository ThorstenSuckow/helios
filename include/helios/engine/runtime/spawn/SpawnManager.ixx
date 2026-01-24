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

export module helios.engine.runtime.spawn.SpawnManager;

import helios.engine.runtime.spawn.SpawnPlanCursor;
import helios.engine.runtime.spawn.SpawnProfile;
import helios.engine.runtime.spawn.SpawnContext;
import helios.engine.runtime.spawn.events.SpawnPlanCommandExecutedEvent;

import helios.engine.runtime.spawn.commands.SpawnCommand;
import helios.engine.runtime.spawn.commands.ScheduledSpawnPlanCommand;
import helios.engine.runtime.spawn.commands.DespawnCommand;

import helios.engine.modules.rendering.model.components.ModelAabbComponent;
import helios.engine.modules.spatial.transform.components.ScaleStateComponent;
import helios.engine.modules.spatial.transform.components.RotationStateComponent;

import helios.engine.modules.physics.collision.Bounds;
import helios.engine.modules.scene.components.SceneNodeComponent;

import helios.engine.runtime.world.Manager;
import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.pooling.GameObjectPoolManager;
import helios.engine.runtime.spawn.SpawnCommandHandler;
import helios.engine.core.data.SpawnProfileId;
import helios.engine.ecs.GameObject;
import helios.engine.modules.spatial.transform.components.TranslationStateComponent;
import helios.engine.mechanics.spawn.components.SpawnedByProfileComponent;

import helios.engine.modules.physics.collision.components.AabbColliderComponent;

import helios.math;

export namespace helios::engine::runtime::spawn {

    /**
     * @brief Manager for processing spawn and despawn commands.
     *
     * @details SpawnManager is a Manager that handles the lifecycle of pooled
     * GameObjects. It receives commands via SpawnCommandHandler::submit() and
     * queues them for deferred processing during the manager flush phase.
     *
     * ## Command Processing Order
     *
     * During flush(), commands are processed in this order:
     * 1. **Despawn commands** — Entities are returned to their pools
     * 2. **Spawn commands** — Single entities are acquired and initialized
     * 3. **Scheduled spawn plan commands** — Batch spawns from scheduler
     *
     * ## Spawn Profiles
     *
     * Each spawn profile defines:
     * - Which pool to acquire entities from
     * - How to position spawned entities (SpawnPlacer)
     * - How to initialize spawned entities (SpawnInitializer)
     *
     * ## Integration
     *
     * SpawnManager registers itself with the GameWorld for each profile ID,
     * allowing commands to route to the correct handler.
     *
     * Example:
     * ```cpp
     * auto spawnManager = std::make_unique<SpawnManager>();
     *
     * spawnManager->addSpawnProfile(bulletProfileId, std::make_unique<SpawnProfile>(
     *     SpawnProfile{
     *         .gameObjectPoolId = bulletPoolId,
     *         .spawnPlacer = std::make_unique<EmitterSpawnPlacer>(),
     *         .spawnInitializer = std::make_unique<EmitterInitializer>()
     *     }
     * ));
     *
     * gameWorld.addManager(std::move(spawnManager));
     * ```
     *
     * @see SpawnCommand
     * @see DespawnCommand
     * @see ScheduledSpawnPlanCommand
     * @see SpawnProfile
     * @see SpawnCommandHandler
     */
    class SpawnManager : public helios::engine::runtime::world::Manager,
                         public helios::engine::runtime::spawn::SpawnCommandHandler {

        /**
         * @brief Queue of pending spawn commands.
         */
        std::vector<helios::engine::runtime::spawn::commands::SpawnCommand> spawnCommands_;

        /**
         * @brief Queue of pending despawn commands.
         */
        std::vector<helios::engine::runtime::spawn::commands::DespawnCommand> despawnCommands_;

        /**
         * @brief Queue of pending scheduled spawn plan commands.
         */
        std::vector<helios::engine::runtime::spawn::commands::ScheduledSpawnPlanCommand> scheduledSpawnPlanCommands_;

        /**
         * @brief Pointer to the pool manager for acquire/release operations.
         */
        helios::engine::runtime::pooling::GameObjectPoolManager* gameObjectPoolManager_ = nullptr;

        /**
         * @brief Map from profile IDs to their spawn profiles.
         */
        std::unordered_map<
            helios::engine::core::data::SpawnProfileId,
            std::unique_ptr<const helios::engine::runtime::spawn::SpawnProfile>> spawnProfiles_;

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
        void ensureBounds(const helios::engine::ecs::GameObject* go, helios::math::aabbf& bounds) {
            if (bounds.min()[0] > bounds.max()[0]) {
                const auto mab   = go->get<helios::engine::modules::rendering::model::components::ModelAabbComponent>();
                const auto sca    = go->get<helios::engine::modules::spatial::transform::components::ScaleStateComponent>();
                const auto rsc = go->get<helios::engine::modules::spatial::transform::components::RotationStateComponent>();
                const auto scn   = go->get<helios::engine::modules::scene::components::SceneNodeComponent>();
                const auto tsc   = go->get<helios::engine::modules::spatial::transform::components::TranslationStateComponent>();

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
         * @param gameWorld The game world.
         * @param updateContext The current update context.
         */
        void executeScheduledSpawnPlanCommands(
            std::span<helios::engine::runtime::spawn::commands::ScheduledSpawnPlanCommand> commands,
            helios::engine::runtime::world::GameWorld& gameWorld,
            helios::engine::runtime::world::UpdateContext& updateContext
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



                    auto* go = gameObjectPoolManager_->acquire(gameObjectPoolId);
                    assert(go && "Failed to acquire GameObject");

                    auto* tsc = go->get<helios::engine::modules::spatial::transform::components::TranslationStateComponent>();

                    auto* sbp = go->get<helios::engine::mechanics::spawn::components::SpawnedByProfileComponent>();
                    assert(sbp && "unexpected missing SpawnedByProfileComponent");

                    auto* aabb = go->get<helios::engine::modules::physics::collision::components::AabbColliderComponent>();
                    assert(aabb && "unexpected missing AabbColliderComponent");

                    auto spawnCursor = helios::engine::runtime::spawn::SpawnPlanCursor{spawnCount, i};
                    const auto& spawnContext =  scheduledSpawnPlanCommand.spawnContext();
                    if (tsc) {
                        
                        auto bounds = aabb->bounds();
                        ensureBounds(go, bounds);

                        const auto position = spawnProfile->spawnPlacer->getPosition(
                            go->guid(),
                            bounds,
                            gameWorld_->level().bounds(),
                            spawnCursor,
                            spawnContext

                        );
                        tsc->setTranslation(position);
                    }

                    spawnProfile->spawnInitializer->initialize(*go, spawnCursor, spawnContext);
                    sbp->setSpawnProfileId(spawnProfileId);
                    go->setActive(true);
                }

                updateContext.pushFrame<helios::engine::runtime::spawn::events::SpawnPlanCommandExecutedEvent>(
                    spawnRuleId, spawnCount
                );
            }
        }


        /**
         * @brief Processes spawn commands, acquiring and initializing objects.
         *
         * @param commands Span of spawn commands to process.
         * @param gameWorld The game world.
         * @param updateContext The current update context.
         */
        void spawnObjects(
            std::span<helios::engine::runtime::spawn::commands::SpawnCommand> commands,
            helios::engine::runtime::world::GameWorld& gameWorld,
            helios::engine::runtime::world::UpdateContext& updateContext) {

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

                auto* go = gameObjectPoolManager_->acquire(gameObjectPoolId);
                assert(go && "Failed to acquire GameObject");

                auto* tsc = go->get<helios::engine::modules::spatial::transform::components::TranslationStateComponent>();
                auto* sbp = go->get<helios::engine::mechanics::spawn::components::SpawnedByProfileComponent>();
                assert(sbp && "unexpected missing SpawnedByProfileComponent");

                auto* aabb = go->get<helios::engine::modules::physics::collision::components::AabbColliderComponent>();
                assert(aabb && "unexpected missing AabbColliderComponent");


                if (tsc) {

                    auto bounds = aabb->bounds();
                    ensureBounds(go, bounds);

                    const auto position = spawnProfile->spawnPlacer->getPosition(
                        go->guid(),
                        bounds,
                        gameWorld_->level().bounds(),
                        {1, 1},
                        spawnContext

                    );
                    tsc->setTranslation(position);
                }

                spawnProfile->spawnInitializer->initialize(*go, {1, 1}, spawnContext);
                sbp->setSpawnProfileId(spawnProfileId);

                go->setActive(true);
            }
        }


        /**
         * @brief Processes despawn commands, releasing objects back to pools.
         *
         * @param commands Span of despawn commands to process.
         * @param gameWorld The game world.
         * @param updateContext The current update context.
         */
        void despawnObjects(
            std::span<helios::engine::runtime::spawn::commands::DespawnCommand> commands,
            helios::engine::runtime::world::GameWorld& gameWorld,
            helios::engine::runtime::world::UpdateContext& updateContext) {

            for (auto& despawnCommand : commands) {
                const auto spawnProfileId = despawnCommand.spawnProfileId();

                const auto it = spawnProfiles_.find(spawnProfileId);
                assert(it != spawnProfiles_.end() && "SpawnProfile not part of SpawnManager");
                const auto spawnProfile = it->second.get();
                auto gameObjectPoolId = spawnProfile->gameObjectPoolId;

                gameObjectPoolManager_->release(gameObjectPoolId, despawnCommand.guid());

            }
        }


    public:

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
        bool submit(const helios::engine::runtime::spawn::commands::SpawnCommand& command) noexcept override {
            spawnCommands_.push_back(command);
            return true;
        }

        /**
         * @brief Submits a despawn command for deferred processing.
         *
         * @param command The despawn command to queue.
         *
         * @return Always returns true.
         */
        bool submit(const helios::engine::runtime::spawn::commands::DespawnCommand& command) noexcept override {
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
            const helios::engine::runtime::spawn::commands::ScheduledSpawnPlanCommand& scheduledSpawnPlanCommand
        ) noexcept override {
            scheduledSpawnPlanCommands_.push_back(scheduledSpawnPlanCommand);
            return true;
        }

        /**
         * @brief Flushes pending commands, processing despawns then spawns.
         *
         * @param gameWorld The game world.
         * @param updateContext The current update context.
         */
        void flush(
            helios::engine::runtime::world::GameWorld& gameWorld,
            helios::engine::runtime::world::UpdateContext& updateContext
        ) noexcept override {
            if (!despawnCommands_.empty()) {
                despawnObjects(despawnCommands_, gameWorld, updateContext);
                despawnCommands_.clear();
            }

            if (!spawnCommands_.empty()) {
                spawnObjects(spawnCommands_, gameWorld, updateContext);
                spawnCommands_.clear();
            }

            if (!scheduledSpawnPlanCommands_.empty()) {
                executeScheduledSpawnPlanCommands(scheduledSpawnPlanCommands_, gameWorld, updateContext);
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
            const helios::engine::core::data::SpawnProfileId& spawnProfileId,
            std::unique_ptr<const helios::engine::runtime::spawn::SpawnProfile> spawnProfile) {

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
        [[nodiscard]] const helios::engine::runtime::spawn::SpawnProfile* spawnProfile(
            const helios::engine::core::data::SpawnProfileId& spawnProfileId) const {

            if (!spawnProfiles_.contains(spawnProfileId)) {
                return nullptr;
            }

            return spawnProfiles_.find(spawnProfileId)->second.get();
        }

        /**
         * @brief Initializes the manager with the game world.
         *
         * @details Acquires a reference to the GameObjectPoolManager and
         * registers this manager as the SpawnCommandHandler for all
         * configured spawn profiles.
         *
         * @param gameWorld The game world to initialize with.
         */
        void init(helios::engine::runtime::world::GameWorld& gameWorld) noexcept override {

            gameObjectPoolManager_ = gameWorld.getManager<helios::engine::runtime::pooling::GameObjectPoolManager>();

            for (const auto& [spawnProfileId, _]: spawnProfiles_) {
                gameWorld.registerSpawnCommandHandler(spawnProfileId, *this);
            }

        }

    };


}
