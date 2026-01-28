/**
 * @file DespawnCommand.ixx
 * @brief Command for requesting GameObject despawning and pool return.
 */
module;

#include <cassert>
#include <optional>

export module helios.engine.runtime.spawn.commands.DespawnCommand;

import helios.engine.runtime.messaging.command.WorldCommand;
import helios.util.Guid;



import helios.engine.core.data.SpawnProfileId;

import helios.engine.runtime.messaging.command.WorldCommandDispatcher;

export namespace helios::engine::runtime::spawn::commands {

    /**
     * @brief Command to request despawning of a GameObject.
     *
     * @details DespawnCommand is a WorldCommand that requests the deactivation
     * and return of a GameObject to its pool. The command identifies the target
     * entity by its GUID and specifies the spawn profile for pool return.
     *
     * The command is dispatched to a DespawnCommandDispatcher which forwards it
     * to the appropriate SpawnManager for deferred processing.
     *
     * @see SpawnCommand
     * @see DespawnCommandDispatcher
     * @see SpawnManager
     */
    class DespawnCommand : public helios::engine::runtime::messaging::command::WorldCommand {

        /**
         * @brief The GUID of the entity to despawn.
         */
        const helios::util::Guid guid_;

        /**
         * @brief The spawn profile ID for pool return.
         */
        helios::engine::core::data::SpawnProfileId spawnProfileId_;

    public:

        /**
         * @brief Constructs a DespawnCommand with a target profile.
         *
         * @param guid The GUID of the entity to despawn.
         * @param spawnProfileId The spawn profile ID for pool return.
         */
        explicit DespawnCommand(
            const helios::util::Guid& guid,
            const helios::engine::core::data::SpawnProfileId spawnProfileId) :
            guid_(guid), spawnProfileId_(spawnProfileId) {}

        /**
         * @brief Direct execution is not supported; requires a dispatcher.
         *
         * @param gameWorld The game world (unused).
         *
         * @note Asserts in debug builds if called directly.
         */
        void execute(helios::engine::runtime::world::GameWorld& gameWorld) const noexcept override {
            assert(false && "Unexpected execute() for despawn, needs dispatcher");
        }

        /**
         * @brief Accepts a dispatcher for command routing.
         *
         * @param gameWorld The game world.
         * @param dispatcher The dispatcher to route this command to.
         */
        void accept(
            helios::engine::runtime::world::GameWorld& gameWorld,
            helios::engine::runtime::messaging::command::WorldCommandDispatcher& dispatcher
        ) const noexcept override {
            dispatcher.dispatch(gameWorld, *this);
        }

        /**
         * @brief Returns the GUID of the entity to despawn.
         *
         * @return The entity's GUID.
         */
        [[nodiscard]] helios::util::Guid guid() const noexcept {
            return guid_;
        }

        /**
         * @brief Returns the spawn profile ID for pool return.
         *
         * @return The spawn profile ID associated with this command.
         */
        [[nodiscard]] helios::engine::core::data::SpawnProfileId spawnProfileId() const noexcept {
            return spawnProfileId_;
        }

    };


}