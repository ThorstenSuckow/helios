/**
 * @file SpawnCommandHandler.ixx
 * @brief Interface for handling pool spawn and despawn commands.
 */
module;

export module helios.engine.runtime.spawn.SpawnCommandHandler;

import helios.engine.runtime.spawn.commands.DespawnCommand;
import helios.engine.runtime.spawn.commands.SpawnCommand;
import helios.engine.runtime.spawn.commands.ScheduledSpawnPlanCommand;

export namespace helios::engine::runtime::spawn {

    /**
     * @brief Interface for handlers that process spawn and despawn commands.
     *
     * @details SpawnCommandHandler defines the contract for managers that handle
     * entity lifecycle operations for GameObjectPools. Each handler is registered
     * with a SpawnProfileId via `GameWorld::registerSpawnCommandHandler()`.
     *
     * When a pooled entity needs to spawn or despawn, the appropriate handler
     * is looked up by profile ID and receives the command for deferred processing.
     *
     * ## Command Types
     *
     * - **SpawnCommand:** Commands acquisition of an entity from the pool
     * - **DespawnCommand:** Commands return of an entity to the pool
     * - **ScheduledSpawnPlanCommand:** Commands batch spawning from scheduler
     *
     * ## Implementation Notes
     *
     * Handlers typically also implement the Manager interface to receive flush()
     * calls for deferred command processing during the manager flush phase.
     *
     * Example:
     * ```cpp
     * class MyPoolManager : public Manager, public SpawnCommandHandler {
     *     bool submit(const SpawnCommand& cmd) noexcept override {
     *         spawnQueue_.push(cmd);
     *         return true;
     *     }
     *     bool submit(const DespawnCommand& cmd) noexcept override {
     *         despawnQueue_.push(cmd);
     *         return true;
     *     }
     *     bool submit(const ScheduledSpawnPlanCommand& cmd) noexcept override {
     *         planQueue_.push(cmd);
     *         return true;
     *     }
     *     void flush(GameWorld& world, UpdateContext& ctx) noexcept override {
     *         // Process queued commands
     *     }
     * };
     * ```
     *
     * @see SpawnCommand
     * @see DespawnCommand
     * @see ScheduledSpawnPlanCommand
     * @see SpawnManager
     * @see PoolManagerRegistry
     */
    class SpawnCommandHandler {

    public:

        /**
         * @brief Virtual destructor for proper polymorphic cleanup.
         */
        virtual ~SpawnCommandHandler() = default;

        /**
         * @brief Submits a despawn command to return an entity to its pool.
         *
         * @param despawnCommand The command containing entity GUID and profile ID.
         *
         * @return true if the command was accepted, false otherwise.
         */
        virtual bool submit(
            const helios::engine::runtime::spawn::commands::DespawnCommand& despawnCommand
        ) noexcept = 0;

        /**
         * @brief Submits a spawn command to acquire an entity from the pool.
         *
         * @param spawnCommand The command containing profile ID and spawn context.
         *
         * @return true if the command was accepted, false otherwise.
         */
        virtual bool submit(
            const helios::engine::runtime::spawn::commands::SpawnCommand& spawnCommand
        ) noexcept = 0;

        /**
         * @brief Submits a scheduled spawn plan command for batch spawning.
         *
         * @param scheduledSpawnPlanCommand The command containing the spawn plan
         *        and spawn context.
         *
         * @return true if the command was accepted, false otherwise.
         */
        virtual bool submit(
            const helios::engine::runtime::spawn::commands::ScheduledSpawnPlanCommand& scheduledSpawnPlanCommand
        ) noexcept = 0;

    };

}