/**
 * @file ProjectilePoolManager.ixx
 * @brief Manager for projectile spawning and pool lifecycle.
 */
module;

#include <cassert>
#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

export module helios.engine.mechanics.combat.manager.ProjectilePoolManager;


import helios.engine.runtime.world.Manager;
import helios.engine.ecs.GameObject;
import helios.engine.runtime.world.UpdateContext;

import helios.engine.runtime.pooling.GameObjectPoolFacade;
import helios.engine.factory.GameObjectFactory;

import helios.engine.mechanics.spawn.logic.SpawnCondition;
import helios.engine.mechanics.spawn.requests.SpawnRequest;
import helios.engine.mechanics.spawn.requests.DespawnRequest;

import helios.engine.game.physics.motion.components.Move2DComponent;
import helios.engine.game.physics.motion.components.SteeringComponent;
import helios.engine.game.physics.motion.components.RotationStateComponent;
import helios.engine.game.physics.motion.components.DirectionComponent;

import helios.engine.game.spatial.transform.components.TranslationStateComponent;

import helios.engine.runtime.pooling.GameObjectPool;
import helios.engine.core.data.GameObjectPoolId;
import helios.engine.runtime.world.Manager;
import helios.engine.runtime.world.GameWorld;

import helios.engine.runtime.pooling.PoolRequestHandler;

import helios.engine.game.pool.components.PoolIdComponent;

import helios.engine.mechanics.combat.ProjectileSpawnRequest;

export namespace helios::engine::mechanics::combat::manager {

    /**
     * @brief Manager for projectile spawning with rate-limited pooling.
     *
     * @details ProjectilePoolManager handles the lifecycle of projectile GameObjects.
     * It receives ProjectileSpawnRequests from the ShootCommandDispatcher, queues them,
     * and spawns projectiles during the manager flush phase based on a SpawnCondition.
     *
     * ## Rate Limiting
     *
     * The manager uses an accumulator-based system for rate limiting:
     * - Each request's intensity is multiplied by the spawn budget
     * - Fractional spawns accumulate across frames
     * - Only whole numbers of projectiles are spawned per frame
     *
     * This allows smooth fire rate control based on trigger pressure.
     *
     * ## Despawn Handling
     *
     * The manager also implements PoolRequestHandler to process DespawnRequests,
     * returning projectiles to the pool when they expire or hit something.
     *
     * @see ProjectileSpawnRequest
     * @see ShootCommandDispatcher
     * @see SpawnCondition
     * @see PoolRequestHandler
     */
    class ProjectilePoolManager : public helios::engine::runtime::world::Manager, public helios::engine::runtime::pooling::PoolRequestHandler {

        /**
         * @brief The pool ID this manager is responsible for.
         */
        helios::engine::core::data::GameObjectPoolId gameObjectPoolId_;

        /**
         * @brief Pointer to the managed projectile pool.
         */
        helios::engine::runtime::pooling::GameObjectPool* gameObjectPool_;

        /**
         * @brief Queue of pending projectile spawn requests.
         */
        std::vector<ProjectileSpawnRequest> requests_;

        /**
         * @brief Condition that determines spawn budget per frame.
         */
        std::unique_ptr<helios::engine::mechanics::spawn::logic::SpawnCondition> spawnCondition_;

        /**
         * @brief Accumulator for fractional spawn counts.
         *
         * @details Stores the fractional part of spawn calculations between frames.
         * When the accumulator exceeds 1.0, a projectile is spawned.
         *
         * @todo If the Manager manages multiple sources, this should be mapped accordingly.
         */
        float sourceAccumulator_ = 0.0f;

        /**
         * @brief Factory for populating the pool with projectile GameObjects.
         */
        std::shared_ptr<helios::engine::factory::GameObjectFactory> gameObjectFactory_;

        /**
         * @brief Facade for pool acquire/release operations.
         */
        helios::engine::runtime::pooling::GameObjectPoolFacade gameObjectPoolFacade_{};

        /**
         * @brief Initializes a projectile GameObject from a spawn request.
         *
         * @details Sets up position, direction, velocity inheritance, and activates
         * the projectile.
         *
         * @param go The projectile GameObject to initialize.
         * @param spawnRequest The request containing spawn parameters.
         */
        void prepareBullet(
            helios::engine::ecs::GameObject* go,
            const ProjectileSpawnRequest& spawnRequest
        ) const noexcept {

            auto* m2c = go->get<helios::engine::game::physics::motion::components::Move2DComponent>();
            auto* tsc = go->get<helios::engine::game::spatial::transform::components::TranslationStateComponent>();

            if (m2c && tsc) {
                m2c->setMoveIntent(spawnRequest.direction, spawnRequest.throttle);
                m2c->setInheritedVelocity(spawnRequest.inheritedVelocity);
                tsc->setTranslation(spawnRequest.location);
            }

            auto* dc = go->get<helios::engine::game::physics::motion::components::DirectionComponent>();
            auto* hc = go->get<helios::engine::game::physics::motion::components::SteeringComponent>();

           // rsc->setHeadingRotationAngle(spawnRequest.direction);

            if (dc) {
                dc->setDirection(spawnRequest.direction);
            }
            if (hc) {
                hc->setSteeringIntent(spawnRequest.direction,1.0f);
            }

            go->setActive(true);

        }

        /**
         * @brief Spawns projectiles from queued requests based on spawn budget.
         *
         * @details Iterates through all pending spawn requests and spawns projectiles
         * using an accumulator-based rate limiting system:
         *
         * 1. For each request, calculates spawn count as `spawnBudget * intensity`
         * 2. Adds this to the accumulator (preserving fractional values across frames)
         * 3. Spawns floor(accumulator) projectiles and subtracts from accumulator
         * 4. Acquires projectiles from pool and initializes via prepareBullet()
         *
         * This allows sub-frame spawn timing and smooth fire rate control.
         *
         * @param gameWorld The game world for entity access.
         * @param updateContext The current frame's update context.
         */
        void spawnProjectiles(
            helios::engine::runtime::world::GameWorld& gameWorld,
            helios::engine::runtime::world::UpdateContext& updateContext
        ) {

            float spawnBudget = spawnCondition_->spawnBudget(gameWorld, *gameObjectPool_, updateContext);

            for (const auto& spawnRequest : requests_) {

                // add to this gameobjects sourceAccumulator the budget aligned to the intensity.
                // e.g., if the intensity fot his frame is 0.2, and the spawnBudget is 5,
                // only 5 * 0.2 = 1 enemies should spawn.
                // add this fraction to the sourceAccumulator.
                sourceAccumulator_ += spawnBudget * spawnRequest.intensity;

                // floor the current sourceAccumulator and store in discrete value
                // so we can iterate
                size_t sb = static_cast<size_t>(sourceAccumulator_);

                // reduce the sourceAccumulator by this amount, since this is the
                // exact amount we _want_ to spawn in this frame.
                sourceAccumulator_ -= static_cast<float>(sb);

                for (size_t i = 0; i < sb; i++) {
                    auto* go = gameObjectPoolFacade_.acquire(gameWorld, *gameObjectPool_);
                    if (!go) {
                        break;
                    }
                    prepareBullet(go, spawnRequest);
                }
            }
        }


    public:

        /**
         * @brief Constructs a ProjectilePoolManager.
         *
         * @param gameObjectPoolId The ID of the pool to manage.
         * @param gameObjectFactory Factory for populating the pool. Ownership is transferred.
         * @param spawnCondition Condition that controls spawn rate. Ownership is transferred.
         */
        explicit ProjectilePoolManager(
            helios::engine::core::data::GameObjectPoolId gameObjectPoolId,
            std::unique_ptr<helios::engine::factory::GameObjectFactory> gameObjectFactory,
            std::unique_ptr<helios::engine::mechanics::spawn::logic::SpawnCondition> spawnCondition
        ) :
            gameObjectPoolId_(gameObjectPoolId),
            spawnCondition_(std::move(spawnCondition)),
            gameObjectFactory_(std::move(gameObjectFactory))
        {}

        /**
         * @brief Not supported for ProjectilePoolManager.
         *
         * @param spawnRequest Ignored.
         *
         * @return Always returns false.
         *
         * @note Use addRequest() instead for projectile spawning.
         */
        bool submit(helios::engine::mechanics::spawn::requests::SpawnRequest spawnRequest) noexcept override {
            assert(false && "not supported");
            return false;
        }

        /**
         * @brief Submits a despawn request to return a projectile to the pool.
         *
         * @param despawnRequest The request containing the entity GUID and pool ID.
         *
         * @return True if the projectile was successfully released, false otherwise.
         */
        bool submit(helios::engine::mechanics::spawn::requests::DespawnRequest despawnRequest) noexcept override {

            const auto guid = despawnRequest.entityId;
            const auto goPid = despawnRequest.gameObjectPoolId;

            if (!goPid) {
                return false;
            }

            if (goPid.value() != gameObjectPoolId_) {
                return false;
            }

            return gameObjectPoolFacade_.release(*Manager::gameWorld_, *gameObjectPool_, guid) != nullptr;
        }

        /**
         * @brief Queues a projectile spawn request for processing.
         *
         * @details Requests are accumulated and processed during the flush() phase.
         *
         * @param request The spawn request containing position, direction, and intensity.
         */
        void addRequest(const ProjectileSpawnRequest& request) {
            requests_.push_back(request);
        }

        /**
         * @brief Processes all queued spawn requests.
         *
         * @details Called during the game loop's manager flush phase. Spawns projectiles
         * based on the accumulated requests and spawn condition budget, then clears
         * the request queue.
         *
         * @param gameWorld The game world for entity access.
         * @param updateContext The current frame's update context.
         */
        void flush(
            helios::engine::runtime::world::GameWorld& gameWorld,
            helios::engine::runtime::world::UpdateContext& updateContext
        ) noexcept override {

            spawnProjectiles(gameWorld, updateContext);
            requests_.clear();
        };


        /**
         * @brief Initializes the manager with the game world.
         *
         * @details Retrieves the pool, registers as the pool's request handler,
         * and populates the pool with projectile templates via the factory.
         *
         * @param gameWorld The game world to initialize with.
         *
         * @pre The pool with gameObjectPoolId_ must exist in gameWorld.
         */
        void init(helios::engine::runtime::world::GameWorld& gameWorld) noexcept {
            gameObjectPool_ = gameWorld.pool(gameObjectPoolId_);
            assert(gameObjectPool_ != nullptr && "Unexpected nullptr for GameObjectPool");

            gameWorld.registerPoolRequestHandler(gameObjectPoolId_, *this);

            gameObjectFactory_->fillPool(gameWorld, *gameObjectPool_);

        }

    };


}