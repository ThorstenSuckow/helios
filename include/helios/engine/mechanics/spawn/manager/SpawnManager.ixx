/**
 * @file SpawnManager.ixx
 * @brief Manager for processing spawn and despawn requests from pools.
 */
module;

#include <cassert>
#include <memory>
#include <span>
#include <vector>

export module helios.engine.mechanics.spawn.manager.SpawnManager;


import helios.engine.ecs.Manager;

import helios.engine.core.data.GameObjectPool;
import helios.engine.core.data.GameObjectPoolId;

import helios.engine.mechanics.spawn.requests.SpawnRequest;
import helios.engine.mechanics.spawn.requests.DespawnRequest;

import helios.engine.factory.GameObjectFactory;

import helios.engine.facade.GameObjectPoolFacade;

import helios.engine.ecs.GameObject;
import helios.engine.ecs.GameWorld;
import helios.engine.ecs.UpdateContext;


import helios.engine.game.physics.motion.components.Move2DComponent;
import helios.engine.game.physics.motion.components.DirectionComponent;
import helios.engine.game.spatial.transform.components.TranslationStateComponent;

import helios.engine.game.PoolRequestHandler;

import helios.util.Random;


export namespace helios::engine::mechanics::spawn::manager {

    /**
     * @brief Manager for processing spawn and despawn requests.
     *
     * @details SpawnManager is a Manager that handles the lifecycle of pooled
     * GameObjects. It receives SpawnRequest and DespawnRequest instances,
     * queues them, and processes them during the manager flush phase.
     *
     * During flush:
     * 1. Despawn requests are processed first, returning entities to their pools.
     * 2. Spawn requests are then processed, acquiring entities and initializing them.
     *
     * The manager uses a GameObjectFactory to populate the pool initially and
     * a GameObjectPoolFacade for acquire/release operations.
     *
     * SpawnManager also implements PoolRequestHandler, allowing it to be registered
     * with a GameWorld for a specific pool ID.
     *
     * @see SpawnRequest
     * @see DespawnRequest
     * @see GameObjectFactory
     * @see PoolRequestHandler
     */
    class SpawnManager : public helios::engine::ecs::Manager, public helios::engine::game::PoolRequestHandler {

        /**
         * @brief The pool ID this manager is responsible for.
         */
        helios::engine::core::data::GameObjectPoolId gameObjectPoolId_;

        /**
         * @brief Pointer to the managed pool.
         */
        helios::engine::core::data::GameObjectPool* gameObjectPool_;

        /**
         * @brief Queue of pending spawn requests.
         */
        std::vector<helios::engine::mechanics::spawn::requests::SpawnRequest> spawnRequests_;

        /**
         * @brief Queue of pending despawn requests.
         */
        std::vector<helios::engine::mechanics::spawn::requests::DespawnRequest> despawnRequests_;

        /**
         * @brief Factory for creating and populating pool objects.
         */
        std::shared_ptr<helios::engine::factory::GameObjectFactory> gameObjectFactory_;

        /**
         * @brief Facade for pool acquire/release operations.
         */
        helios::engine::facade::GameObjectPoolFacade gameObjectPoolFacade_{};

        /**
         * @brief Prepares a spawn request before execution.
         *
         * @param spawnRequest The request to prepare.
         */
        void prepareSpawn(
            const helios::engine::mechanics::spawn::requests::SpawnRequest& spawnRequest
        ) const noexcept {

        }

        /**
         * @brief Processes spawn requests, acquiring and initializing objects.
         *
         * @param requests Span of spawn requests to process.
         * @param gameWorld The game world.
         * @param updateContext The current update context.
         */
        void spawnObjects(
            std::span<helios::engine::mechanics::spawn::requests::SpawnRequest> requests,
            helios::engine::ecs::GameWorld& gameWorld,
            helios::engine::ecs::UpdateContext& updateContext) {

            for (auto& spawnRequest : requests) {
                auto* gameObject = gameObjectPoolFacade_.acquire(gameWorld, *gameObjectPool_);
                if (!gameObject) {
                    return;
                }

                static auto rGen = helios::util::Random(12345);

                auto* mc = gameObject->get<helios::engine::game::physics::motion::components::Move2DComponent>();
                auto* dc = gameObject->get<helios::engine::game::physics::motion::components::DirectionComponent>();
                auto* tsc = gameObject->get<helios::engine::game::spatial::transform::components::TranslationStateComponent>();

                auto& levelBounds = gameWorld.level().bounds();

                float xPos = rGen.randomFloat(levelBounds.min()[0], levelBounds.max()[0]);
                float yPos = rGen.randomFloat(levelBounds.min()[1], levelBounds.max()[1]);

                auto position = helios::math::vec3f{xPos, yPos, 0.0f};
                auto dir = helios::math::vec2f{rGen.randomFloat(-1.0f, 1.0f), rGen.randomFloat(-1.0f, 1.0f)};

                dc->setDirection(dir.normalize().toVec3());
                mc->move(dc->direction(), 1.0f);
                tsc->setTranslation(position);

                gameObject->setActive(true);
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
            std::span<helios::engine::mechanics::spawn::requests::DespawnRequest> requests,
            helios::engine::ecs::GameWorld& gameWorld,
            helios::engine::ecs::UpdateContext& updateContext) {

            for (auto& despawnRequest : requests) {
                if (despawnRequest.gameObjectPoolId.has_value()) {
                    auto gameObjectPool = gameWorld.pool(despawnRequest.gameObjectPoolId.value());
                    gameObjectPoolFacade_.release(gameWorld, *gameObjectPool, despawnRequest.entityId);
                }
            }
        }

    public:

        /**
         * @brief Constructs a SpawnManager for a specific pool.
         *
         * @param gameObjectPoolId The ID of the pool to manage.
         * @param gameObjectFactory Factory for populating the pool with objects.
         */
        explicit SpawnManager(
            helios::engine::core::data::GameObjectPoolId gameObjectPoolId,
            std::unique_ptr<helios::engine::factory::GameObjectFactory> gameObjectFactory
        ) :
            gameObjectPoolId_(gameObjectPoolId),
            gameObjectFactory_(std::move(gameObjectFactory))
        {}

        /**
         * @brief Submits a spawn request for deferred processing.
         *
         * @param request The spawn request to queue.
         *
         * @return Always returns true.
         */
        bool submit(const helios::engine::mechanics::spawn::requests::SpawnRequest request) noexcept override {
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
        bool submit(const helios::engine::mechanics::spawn::requests::DespawnRequest request) noexcept override {
            despawnRequests_.push_back(request);
            return true;
        }

        /**
         * @brief Flushes pending requests, processing despawns then spawns.
         *
         * @param gameWorld The game world.
         * @param updateContext The current update context.
         */
        void flush(
            helios::engine::ecs::GameWorld& gameWorld,
            helios::engine::ecs::UpdateContext& updateContext
        ) noexcept override {
            if (despawnRequests_.size() > 0) {
                despawnObjects(despawnRequests_, gameWorld, updateContext);
                despawnRequests_.clear();
            }

            if (spawnRequests_.size() > 0) {
                spawnObjects(spawnRequests_, gameWorld, updateContext);
                spawnRequests_.clear();
            }
        };

        /**
         * @brief Initializes the manager, populating the pool.
         *
         * @details Retrieves the pool from the GameWorld, uses the factory to
         * fill it with objects, and registers this manager as the pool's
         * request handler.
         *
         * @param gameWorld The game world containing the pool.
         */
        void init(helios::engine::ecs::GameWorld& gameWorld) noexcept {
            gameObjectPool_ = gameWorld.pool(gameObjectPoolId_);
            assert(gameObjectPool_ != nullptr && "Unexpected nullptr for GameObjectPool");
            gameObjectFactory_->fillPool(gameWorld, *gameObjectPool_);

            gameWorld.registerPoolRequestHandler(gameObjectPoolId_, *this);
        }

    };


}