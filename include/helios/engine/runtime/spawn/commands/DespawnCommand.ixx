/**
 * @file DespawnCommand.ixx
 * @brief Command for requesting GameObject despawning and pool return.
 */
module;

#include <cassert>
#include <optional>

export module helios.engine.runtime.spawn.commands.DespawnCommand;

import helios.engine.runtime.messaging.command.WorldCommand;
import helios.engine.ecs.EntityHandle;



import helios.engine.core.data.SpawnProfileId;

import helios.engine.runtime.messaging.command.WorldCommandDispatcher;

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
    class DespawnCommand : public helios::engine::runtime::messaging::command::WorldCommand {

        /**
         * @brief The handle of the entity to despawn.
         */
        helios::engine::ecs::EntityHandle entityHandle_;

        /**
         * @brief The spawn profile ID for pool return.
         */
        helios::engine::core::data::SpawnProfileId spawnProfileId_;

    public:

        /**
         * @brief Constructs a DespawnCommand with a target profile.
         *
         * @param entityHandle The handle of the entity to despawn.
         * @param spawnProfileId The spawn profile ID for pool return.
         */
        explicit DespawnCommand(
            const helios::engine::ecs::EntityHandle entityHandle,
            const helios::engine::core::data::SpawnProfileId spawnProfileId) :
            entityHandle_(entityHandle), spawnProfileId_(spawnProfileId) {}

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
         * @brief Returns the handle of the entity to despawn.
         *
         * @return The entity handle.
         */
        [[nodiscard]] helios::engine::ecs::EntityHandle entityHandle() const noexcept {
            return entityHandle_;
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