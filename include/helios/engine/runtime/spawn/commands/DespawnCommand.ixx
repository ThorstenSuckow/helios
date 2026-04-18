/**
 * @file DespawnCommand.ixx
 * @brief Command for requesting GameObject despawning and pool return.
 */
module;

#include <cassert>
#include <optional>

export module helios.engine.runtime.spawn.commands.DespawnCommand;

import helios.ecs.types.EntityHandle;



import helios.engine.runtime.spawn.types.SpawnProfileId;


export namespace helios::engine::runtime::spawn::commands {

    /**
     * @brief Command to request despawning of a GameObject.
     *
     * @details DespawnCommand is a WorldCommand that requests the deactivation
     * and return of a GameObject to its pool. The command identifies the target
     * entity by its handle and specifies the spawn profile for pool return.
     *
     * The command is dispatched to a DespawnCommandDispatcher which forwards it
     * to the appropriate SpawnManager for deferred processing.
     *
     * @see SpawnCommand
     * @see DespawnCommandDispatcher
     * @see SpawnManager
     */
    template<typename THandle>
    class DespawnCommand {

        /**
         * @brief The handle of the entity to despawn.
         */
        THandle entityHandle_;

        /**
         * @brief The spawn profile ID for pool return.
         */
        helios::engine::runtime::spawn::types::SpawnProfileId spawnProfileId_;

    public:

        /**
         * @brief Constructs a DespawnCommand with a target profile.
         *
         * @param entityHandle The handle of the entity to despawn.
         * @param spawnProfileId The spawn profile ID for pool return.
         */
        explicit DespawnCommand(
            const THandle entityHandle,
            const helios::engine::runtime::spawn::types::SpawnProfileId spawnProfileId) :
            entityHandle_(entityHandle), spawnProfileId_(spawnProfileId) {}


        /**
         * @brief Returns the handle of the entity to despawn.
         *
         * @return The entity handle.
         */
        [[nodiscard]] THandle entityHandle() const noexcept {
            return entityHandle_;
        }

        /**
         * @brief Returns the spawn profile ID for pool return.
         *
         * @return The spawn profile ID associated with this command.
         */
        [[nodiscard]] helios::engine::runtime::spawn::types::SpawnProfileId spawnProfileId() const noexcept {
            return spawnProfileId_;
        }

    };


}