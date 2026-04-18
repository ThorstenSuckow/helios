/**
 * @file SpawnCommand.ixx
 * @brief Command for requesting GameObject spawning from a pool.
 */
module;

#include <cassert>
#include <cinttypes>
#include <expected>

export module helios.gameplay.spawn.commands.SpawnCommand;

import helios.gameplay.spawn.types;

import helios.math.types;


using namespace helios::gameplay::spawn::types;
export namespace helios::gameplay::spawn::commands {

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
    template<typename THandle>
    class SpawnCommand {

        /**
         * @brief The number of GameObjects to spawn.
         */
        size_t spawnBudget_;

        /**
         * @brief Profile identifying the spawn configuration.
         */
        SpawnProfileId spawnProfileId_;

        /**
         * @brief Context providing spawn-time information (e.g., emitter).
         */
        SpawnContext<THandle> spawnContext_;

    public:

        /**
         * @brief Constructs a SpawnCommand.
         *
         * @param spawnProfileId The profile identifying the spawn configuration.
         * @param spawnContext Context providing spawn-time information.
         * @param spawnBudget Number of objects to spawn (default: 1).
         */
        explicit SpawnCommand(
            const SpawnProfileId spawnProfileId,
            const SpawnContext<THandle>& spawnContext,
            const size_t spawnBudget = 1
        ) :
        spawnProfileId_(spawnProfileId),
        spawnContext_(spawnContext),
        spawnBudget_(spawnBudget)
        {}


        /**
         * @brief Returns the target pool ID.
         *
         * @return The GameObjectPoolId to spawn from.
         */
        [[nodiscard]] SpawnProfileId spawnProfileId() const noexcept {
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
        [[nodiscard]] SpawnContext<THandle> spawnContext() const noexcept {
            return spawnContext_;
        }
    };


}