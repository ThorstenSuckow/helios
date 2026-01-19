/**
 * @file SpawnManager.ixx
 * @brief Manager for processing spawn and despawn requests from pools.
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
import helios.engine.runtime.spawn.events.SpawnPlanRequestExecutedEvent;
import helios.engine.runtime.spawn.requests.SpawnRequest;
import helios.engine.runtime.spawn.requests.ScheduledSpawnPlanRequest;
import helios.engine.runtime.spawn.requests.DespawnRequest;
import helios.engine.runtime.world.Manager;
import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.pooling.GameObjectPoolManager;
import helios.engine.runtime.pooling.SpawnRequestHandler;
import helios.engine.core.data.SpawnProfileId;
import helios.engine.ecs.GameObject;
import helios.engine.modules.spatial.transform.components.TranslationStateComponent;
import helios.engine.mechanics.spawn.components.SpawnedByProfileComponent;

export namespace helios::engine::runtime::spawn {

    /**
     * @brief Manager for processing spawn and despawn requests.
     *
     * @details SpawnManager is a Manager that handles the lifecycle of pooled
     * GameObjects. It receives requests via SpawnRequestHandler::submit() and
     * queues them for deferred processing during the manager flush phase.
     *
     * ## Request Processing Order
     *
     * During flush(), requests are processed in this order:
     * 1. **Despawn requests** — Entities are returned to their pools
     * 2. **Spawn requests** — Single entities are acquired and initialized
     * 3. **Scheduled spawn plan requests** — Batch spawns from scheduler
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
     * allowing commands to route requests to the correct handler.
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
     * @see SpawnRequest
     * @see DespawnRequest
     * @see ScheduledSpawnPlanRequest
     * @see SpawnProfile
     * @see SpawnRequestHandler
     */
    class SpawnManager : public helios::engine::runtime::world::Manager,
                         public helios::engine::runtime::pooling::SpawnRequestHandler {

        /**
         * @brief Queue of pending spawn requests.
         */
        std::vector<helios::engine::runtime::spawn::requests::SpawnRequest> spawnRequests_;

        /**
         * @brief Queue of pending despawn requests.
         */
        std::vector<helios::engine::runtime::spawn::requests::DespawnRequest> despawnRequests_;

        /**
         * @brief Queue of pending scheduled spawn plan requests.
         */
        std::vector<helios::engine::runtime::spawn::requests::ScheduledSpawnPlanRequest> scheduledSpawnPlanRequests_;

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
         * @brief Processes scheduled spawn plan requests.
         *
         * @details Iterates through each plan, acquires entities from the pool,
         * positions them using the profile's placer, initializes them using
         * the profile's initializer, and pushes a frame event for confirmation.
         *
         * @param requests Span of scheduled spawn plan requests to process.
         * @param gameWorld The game world.
         * @param updateContext The current update context.
         */
        void executeScheduledSpawnPlanRequests(
            std::span<helios::engine::runtime::spawn::requests::ScheduledSpawnPlanRequest> requests,
            helios::engine::runtime::world::GameWorld& gameWorld,
            helios::engine::runtime::world::UpdateContext& updateContext
        ) {

            for (auto& spawnPlanRequest: requests) {

                auto& scheduledPlan = spawnPlanRequest.scheduledSpawnPlan;
                auto& spawnPlan     = scheduledPlan.spawnPlan;

                auto spawnProfileId   = scheduledPlan.spawnProfileId;
                auto spawnRuleId      = spawnPlan.spawnRuleId;
                auto amount     = spawnPlan.amount;

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

                    auto spawnCursor = helios::engine::runtime::spawn::SpawnPlanCursor{spawnCount, i};
                    const auto& spawnContext =  spawnPlanRequest.spawnContext;
                    if (tsc) {
                        const auto position = spawnProfile->spawnPlacer->getPosition(
                            go->guid(),
                            gameWorld_->level().bounds(),
                            spawnCursor,
                            spawnContext

                        );
                        tsc->setTranslation(position);
                    }

                    spawnProfile->spawnInitializer->initialize(*go, spawnCursor, spawnPlanRequest.spawnContext);
                    sbp->setSpawnProfileId(spawnProfileId);
                    go->setActive(true);
                }

                updateContext.pushFrame<helios::engine::runtime::spawn::events::SpawnPlanRequestExecutedEvent>(
                    spawnRuleId, spawnCount
                );
            }
        }


        /**
         * @brief Processes spawn requests, acquiring and initializing objects.
         *
         * @param requests Span of spawn requests to process.
         * @param gameWorld The game world.
         * @param updateContext The current update context.
         */
        void spawnObjects(
            std::span<helios::engine::runtime::spawn::requests::SpawnRequest> requests,
            helios::engine::runtime::world::GameWorld& gameWorld,
            helios::engine::runtime::world::UpdateContext& updateContext) {

            for (auto& spawnRequest: requests) {
                auto spawnProfileId = spawnRequest.spawnProfileId;
                auto& spawnContext  = spawnRequest.spawnContext;

                const auto it = spawnProfiles_.find(spawnProfileId);
                assert(it != spawnProfiles_.end() && "SpawnProfile not part of SpawnManager");

                const auto spawnProfile = it->second.get();

                const auto gameObjectPoolId = spawnProfile->gameObjectPoolId;

                auto* go = gameObjectPoolManager_->acquire(gameObjectPoolId);
                assert(go && "Failed to acquire GameObject");

                auto* tsc = go->get<helios::engine::modules::spatial::transform::components::TranslationStateComponent>();
                auto* sbp = go->get<helios::engine::mechanics::spawn::components::SpawnedByProfileComponent>();
                assert(sbp && "unexpected missing SpawnedByProfileComponent");


                if (tsc) {
                    const auto position = spawnProfile->spawnPlacer->getPosition(
                        go->guid(),
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
         * @brief Processes despawn requests, releasing objects back to pools.
         *
         * @param requests Span of despawn requests to process.
         * @param gameWorld The game world.
         * @param updateContext The current update context.
         */
        void despawnObjects(
            std::span<helios::engine::runtime::spawn::requests::DespawnRequest> requests,
            helios::engine::runtime::world::GameWorld& gameWorld,
            helios::engine::runtime::world::UpdateContext& updateContext) {

            for (auto& despawnRequest : requests) {
                if (despawnRequest.spawnProfileId.has_value()) {

                    auto spawnProfileId = despawnRequest.spawnProfileId.value();

                    const auto it = spawnProfiles_.find(spawnProfileId);
                    assert(it != spawnProfiles_.end() && "SpawnProfile not part of SpawnManager");
                    const auto spawnProfile = it->second.get();
                    auto gameObjectPoolId = spawnProfile->gameObjectPoolId;

                    gameObjectPoolManager_->release(gameObjectPoolId, despawnRequest.entityId);
                }
            }
        }


    public:

        /**
         * @brief Default constructor.
         */
        SpawnManager() = default;

        /**
         * @brief Submits a spawn request for deferred processing.
         *
         * @param request The spawn request to queue.
         *
         * @return Always returns true.
         */
        bool submit(const helios::engine::runtime::spawn::requests::SpawnRequest request) noexcept override {
            spawnRequests_.push_back(request);
            return true;
        }

        /**
         * @brief Submits a despawn request for deferred processing.
         *
         * @param request The despawn request to queue.
         *
         * @return Always returns true.
         */
        bool submit(const helios::engine::runtime::spawn::requests::DespawnRequest request) noexcept override {
            despawnRequests_.push_back(request);
            return true;
        }


        /**
         * @brief Submits a scheduled spawn plan request for deferred processing.
         *
         * @param scheduledSpawnPlanRequest The scheduled plan request to queue.
         *
         * @return Always returns true.
         */
        bool submit(
            helios::engine::runtime::spawn::requests::ScheduledSpawnPlanRequest scheduledSpawnPlanRequest
        ) noexcept override {
            scheduledSpawnPlanRequests_.push_back(scheduledSpawnPlanRequest);
            return true;
        }

        /**
         * @brief Flushes pending requests, processing despawns then spawns.
         *
         * @param gameWorld The game world.
         * @param updateContext The current update context.
         */
        void flush(
            helios::engine::runtime::world::GameWorld& gameWorld,
            helios::engine::runtime::world::UpdateContext& updateContext
        ) noexcept override {
            if (!despawnRequests_.empty()) {
                despawnObjects(despawnRequests_, gameWorld, updateContext);
                despawnRequests_.clear();
            }

            if (!spawnRequests_.empty()) {
                spawnObjects(spawnRequests_, gameWorld, updateContext);
                spawnRequests_.clear();
            }

            if (!scheduledSpawnPlanRequests_.empty()) {
                executeScheduledSpawnPlanRequests(scheduledSpawnPlanRequests_, gameWorld, updateContext);
                scheduledSpawnPlanRequests_.clear();
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
         * registers this manager as the SpawnRequestHandler for all
         * configured spawn profiles.
         *
         * @param gameWorld The game world to initialize with.
         */
        void init(helios::engine::runtime::world::GameWorld& gameWorld) noexcept override {

            gameObjectPoolManager_ = gameWorld.getManager<helios::engine::runtime::pooling::GameObjectPoolManager>();

            for (const auto& [spawnProfileId, _]: spawnProfiles_) {
                gameWorld.registerSpawnRequestHandler(spawnProfileId, *this);
            }

        }

    };


}