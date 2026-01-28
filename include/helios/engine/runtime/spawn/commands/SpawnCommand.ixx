/**
 * @file SpawnCommand.ixx
 * @brief Command for requesting GameObject spawning from a pool.
 */
module;

#include <cassert>
#include <cinttypes>
#include <expected>

export module helios.engine.runtime.spawn.commands.SpawnCommand;

import helios.engine.runtime.spawn.SpawnContext;

import helios.engine.runtime.messaging.command.WorldCommand;
import helios.engine.runtime.messaging.command.WorldCommandDispatcher;

import helios.math.types;

import helios.engine.core.data.SpawnProfileId;


export namespace helios::engine::runtime::spawn::commands {

    /**
     * @brief Command to request spawning of GameObjects from a pool.
     *
     * @details SpawnCommand is a WorldCommand that requests the activation of
     * GameObjects from a specific pool. The command carries a spawn budget
     * indicating how many objects should be spawned.
     *
     * The command itself does not execute spawning directly. Instead, it is
     * dispatched to a SpawnCommandDispatcher which forwards the command to the
     * appropriate SpawnManager for deferred processing.
     *
     * @see SpawnCommandDispatcher
     * @see SpawnManager
     * @see GameObjectSpawnSystem
     */
    class SpawnCommand : public helios::engine::runtime::messaging::command::WorldCommand {

        /**
         * @brief The number of GameObjects to spawn.
         */
        const size_t spawnBudget_;


        const helios::engine::core::data::SpawnProfileId spawnProfileId_;

        const helios::engine::runtime::spawn::SpawnContext spawnContext_;

    public:


        explicit SpawnCommand(
            const helios::engine::core::data::SpawnProfileId spawnProfileId,
            const helios::engine::runtime::spawn::SpawnContext& spawnContext,
            const size_t spawnBudget = 1
        ) :
        spawnProfileId_(spawnProfileId),
        spawnContext_(spawnContext),
        spawnBudget_(spawnBudget)
        {}

        /**
         * @brief Direct execution is a no-op; dispatching is required.
         *
         * @param gameWorld The game world (unused).
         */
        void execute(helios::engine::runtime::world::GameWorld& gameWorld) const noexcept override {
            assert(false && "Unexpected execute() on SpawnCommand");
        }

        /**
         * @brief Accepts a dispatcher for command routing.
         *
         * @param gameWorld The game world.
         * @param dispatcher The dispatcher to route this command to.
         */
        virtual void accept(
            helios::engine::runtime::world::GameWorld& gameWorld,
            helios::engine::runtime::messaging::command::WorldCommandDispatcher& dispatcher) const noexcept override {

            dispatcher.dispatch(gameWorld, *this);

        };

        /**
         * @brief Returns the target pool ID.
         *
         * @return The GameObjectPoolId to spawn from.
         */
        [[nodiscard]] helios::engine::core::data::SpawnProfileId spawnProfileId() const noexcept {
            return spawnProfileId_;
        }

        /**
         * @brief Returns the spawn budget.
         *
         * @return The number of objects to spawn.
         */
        [[nodiscard]] size_t spawnBudget() const noexcept {
            return spawnBudget_;
        }

        /**
         * @brief Returns the spawn context.
         *
         * @return The spawn context of this command.
         */
        [[nodiscard]] helios::engine::runtime::spawn::SpawnContext spawnContext() const noexcept {
            return spawnContext_;
        }
    };


}