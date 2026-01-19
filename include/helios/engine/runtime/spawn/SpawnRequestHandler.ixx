/**
 * @file SpawnRequestHandler.ixx
 * @brief Interface for handling pool spawn and despawn requests.
 */
module;

export module helios.engine.runtime.pooling.SpawnRequestHandler;

import helios.engine.runtime.spawn.requests.DespawnRequest;
import helios.engine.runtime.spawn.requests.SpawnRequest;
import helios.engine.runtime.spawn.requests.ScheduledSpawnPlanRequest;

export namespace helios::engine::runtime::pooling {

    /**
     * @brief Interface for handlers that process spawn and despawn requests.
     *
     * @details SpawnRequestHandler defines the contract for managers that handle
     * entity lifecycle operations for GameObjectPools. Each handler is registered
     * with a SpawnProfileId via `GameWorld::registerSpawnRequestHandler()`.
     *
     * When a pooled entity needs to spawn or despawn, the appropriate handler
     * is looked up by profile ID and receives the request for deferred processing.
     *
     * ## Request Types
     *
     * - **SpawnRequest:** Requests acquisition of an entity from the pool
     * - **DespawnRequest:** Requests return of an entity to the pool
     * - **ScheduledSpawnPlanRequest:** Requests batch spawning from scheduler
     *
     * ## Implementation Notes
     *
     * Handlers typically also implement the Manager interface to receive flush()
     * calls for deferred request processing during the manager flush phase.
     *
     * Example:
     * ```cpp
     * class MyPoolManager : public Manager, public SpawnRequestHandler {
     *     bool submit(SpawnRequest req) noexcept override {
     *         spawnQueue_.push(req);
     *         return true;
     *     }
     *     bool submit(DespawnRequest req) noexcept override {
     *         despawnQueue_.push(req);
     *         return true;
     *     }
     *     bool submit(ScheduledSpawnPlanRequest req) noexcept override {
     *         planQueue_.push(req);
     *         return true;
     *     }
     *     void flush(GameWorld& world, UpdateContext& ctx) noexcept override {
     *         // Process queued requests
     *     }
     * };
     * ```
     *
     * @see SpawnRequest
     * @see DespawnRequest
     * @see ScheduledSpawnPlanRequest
     * @see SpawnManager
     * @see PoolManagerRegistry
     */
    class SpawnRequestHandler {

    public:

        /**
         * @brief Virtual destructor for proper polymorphic cleanup.
         */
        virtual ~SpawnRequestHandler() = default;

        /**
         * @brief Submits a despawn request to return an entity to its pool.
         *
         * @param despawnRequest The request containing entity GUID and profile ID.
         *
         * @return true if the request was accepted, false otherwise.
         */
        virtual bool submit(
            helios::engine::runtime::spawn::requests::DespawnRequest despawnRequest
        ) noexcept = 0;

        /**
         * @brief Submits a spawn request to acquire an entity from the pool.
         *
         * @param spawnRequest The request containing profile ID and spawn context.
         *
         * @return true if the request was accepted, false otherwise.
         */
        virtual bool submit(
            helios::engine::runtime::spawn::requests::SpawnRequest spawnRequest
        ) noexcept = 0;

        /**
         * @brief Submits a scheduled spawn plan request for batch spawning.
         *
         * @param scheduledSpawnPlanRequest The request containing the spawn plan
         *        and spawn context.
         *
         * @return true if the request was accepted, false otherwise.
         */
        virtual bool submit(
            helios::engine::runtime::spawn::requests::ScheduledSpawnPlanRequest scheduledSpawnPlanRequest
        ) noexcept = 0;

    };

}