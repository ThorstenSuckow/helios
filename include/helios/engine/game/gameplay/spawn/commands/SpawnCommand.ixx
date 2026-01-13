/**
 * @file SpawnCommand.ixx
 * @brief Command for requesting GameObject spawning from a pool.
 */
module;

#include <cinttypes>

export module helios.engine.game.gameplay.spawn.commands.SpawnCommand;

import helios.engine.core.messaging.command.WorldCommand;
import helios.engine.core.messaging.command.WorldCommandDispatcher;
import helios.engine.game.GameWorld;
import helios.math.types;

import helios.engine.core.data.GameObjectPoolId;


export namespace helios::engine::game::gameplay::spawn::commands {

    /**
     * @brief Command to request spawning of GameObjects from a pool.
     *
     * @details SpawnCommand is a WorldCommand that requests the activation of
     * GameObjects from a specific pool. The command carries a spawn budget
     * indicating how many objects should be spawned.
     *
     * The command itself does not execute spawning directly. Instead, it is
     * dispatched to a SpawnCommandDispatcher which routes the request to the
     * appropriate SpawnManager.
     *
     * @see SpawnCommandDispatcher
     * @see SpawnManager
     * @see GameObjectSpawnSystem
     */
    class SpawnCommand : public helios::engine::core::messaging::command::WorldCommand {

        /**
         * @brief The number of GameObjects to spawn.
         */
        const size_t spawnBudget_;

        /**
         * @brief The ID of the pool from which to acquire GameObjects.
         */
        const helios::engine::core::data::GameObjectPoolId gameObjectPoolId_;

    public:

        /**
         * @brief Constructs a SpawnCommand.
         *
         * @param gameObjectPoolId The ID of the pool to spawn from.
         * @param spawnBudget The number of objects to spawn.
         */
        explicit SpawnCommand(
            const helios::engine::core::data::GameObjectPoolId gameObjectPoolId,
            const size_t spawnBudget
        ) :
        gameObjectPoolId_(gameObjectPoolId),
        spawnBudget_(spawnBudget)
        {}

        /**
         * @brief Direct execution is a no-op; dispatching is required.
         *
         * @param gameWorld The game world (unused).
         */
        void execute(helios::engine::game::GameWorld& gameWorld) const noexcept override {
            // noop - requires dispatcher
        }

        /**
         * @brief Accepts a dispatcher for command routing.
         *
         * @param gameWorld The game world.
         * @param dispatcher The dispatcher to route this command to.
         */
        virtual void accept(
            helios::engine::game::GameWorld& gameWorld,
            helios::engine::core::messaging::command::WorldCommandDispatcher& dispatcher) const noexcept override {

            dispatcher.dispatch(gameWorld, *this);

        };

        /**
         * @brief Returns the target pool ID.
         *
         * @return The GameObjectPoolId to spawn from.
         */
        [[nodiscard]] helios::engine::core::data::GameObjectPoolId gameObjectPoolId() const noexcept {
            return gameObjectPoolId_;
        }

        /**
         * @brief Returns the spawn budget.
         *
         * @return The number of objects to spawn.
         */
        [[nodiscard]] size_t spawnBudget() const noexcept {
            return spawnBudget_;
        }
    };


}