/**
 * @file PoolRequestHandler.ixx
 * @brief Interface for handling pool spawn and despawn requests.
 */
module;

export module helios.engine.game.PoolRequestHandler;

import helios.engine.game.GameObject;

import helios.engine.game.gameplay.spawn.requests.DespawnRequest;
import helios.engine.game.gameplay.spawn.requests.SpawnRequest;

export namespace helios::engine::game {

    class GameWorld;

    /**
     * @brief Interface for handlers that process spawn and despawn requests for a pool.
     *
     * @details PoolRequestHandler defines the contract for managers that handle
     * entity lifecycle operations for a specific GameObjectPool. Each handler is
     * registered with a pool ID via `GameWorld::registerPoolRequestHandler()`.
     *
     * When a pooled entity needs to spawn or despawn, the appropriate handler
     * is looked up by pool ID and receives the request for processing.
     *
     * ## Request Types
     *
     * - **SpawnRequest:** Requests acquisition of an entity from the pool
     * - **DespawnRequest:** Requests return of an entity to the pool
     *
     * ## Implementation Notes
     *
     * Handlers typically also implement the Manager interface to receive flush()
     * calls for deferred request processing.
     *
     * Example:
     * ```cpp
     * class MyPoolManager : public Manager, public PoolRequestHandler {
     *     bool submit(SpawnRequest req) noexcept override {
     *         spawnQueue_.push(req);
     *         return true;
     *     }
     *     bool submit(DespawnRequest req) noexcept override {
     *         return releaseToPool(req.entityId);
     *     }
     * };
     * ```
     *
     * @see SpawnRequest
     * @see DespawnRequest
     * @see PoolManagerRegistry
     * @see Manager
     */
    class PoolRequestHandler {


    public:

        virtual ~PoolRequestHandler() = default;

        /**
         * @brief Submits a despawn request to return an entity to its pool.
         *
         * @param despawnRequest The request containing entity GUID and pool ID.
         *
         * @return True if the request was accepted, false otherwise.
         */
        virtual bool submit(
            helios::engine::game::gameplay::spawn::requests::DespawnRequest despawnRequest
       ) noexcept = 0;

        /**
         * @brief Submits a spawn request to acquire an entity from the pool.
         *
         * @param spawnRequest The request containing pool ID and spawn budget.
         *
         * @return True if the request was accepted, false otherwise.
         */
        virtual bool submit(
            helios::engine::game::gameplay::spawn::requests::SpawnRequest spawnRequest
       ) noexcept = 0;
    };


}