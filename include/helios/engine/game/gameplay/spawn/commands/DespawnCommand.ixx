/**
 * @file DespawnCommand.ixx
 * @brief Command for requesting GameObject despawning and pool return.
 */
module;

#include <cassert>
#include <optional>

export module helios.engine.game.gameplay.spawn.commands.DespawnCommand;

import helios.engine.core.messaging.command.WorldCommand;
import helios.util.Guid;

import helios.engine.ecs.GameWorld;

import helios.engine.core.data.GameObjectPoolId;

import helios.engine.core.messaging.command.WorldCommandDispatcher;

export namespace helios::engine::game::gameplay::spawn::commands {

    /**
     * @brief Command to request despawning of a GameObject.
     *
     * @details DespawnCommand is a WorldCommand that requests the deactivation
     * and return of a GameObject to its pool. The command identifies the target
     * entity by its GUID and optionally specifies which pool to return it to.
     *
     * If no pool ID is provided, the dispatcher must determine the correct pool
     * or handle the despawn without pool return.
     *
     * @see SpawnCommand
     * @see DespawnCommandDispatcher
     * @see SpawnManager
     */
    class DespawnCommand : public helios::engine::core::messaging::command::WorldCommand {

        /**
         * @brief The GUID of the entity to despawn.
         */
        const helios::util::Guid guid_;

        /**
         * @brief Optional pool ID to return the entity to.
         */
        std::optional<helios::engine::core::data::GameObjectPoolId> gameObjectPoolId_;

    public:

        /**
         * @brief Constructs a DespawnCommand without a specific pool.
         *
         * @param guid The GUID of the entity to despawn.
         */
        explicit DespawnCommand(const helios::util::Guid& guid) : guid_(guid) {}

        /**
         * @brief Constructs a DespawnCommand with a target pool.
         *
         * @param guid The GUID of the entity to despawn.
         * @param gameObjectPoolId The pool to return the entity to.
         */
        explicit DespawnCommand(
            const helios::util::Guid& guid,
            const helios::engine::core::data::GameObjectPoolId gameObjectPoolId) :
            guid_(guid), gameObjectPoolId_(gameObjectPoolId) {}

        /**
         * @brief Direct execution is not supported; requires a dispatcher.
         *
         * @param gameWorld The game world (unused).
         *
         * @note Asserts in debug builds if called directly.
         */
        void execute(helios::engine::ecs::GameWorld& gameWorld) const noexcept override {
            assert(false && "unexpected execute for despawn, needs dispatcher");
        }

        /**
         * @brief Accepts a dispatcher for command routing.
         *
         * @param gameWorld The game world.
         * @param dispatcher The dispatcher to route this command to.
         */
        void accept(
            helios::engine::ecs::GameWorld& gameWorld,
            helios::engine::core::messaging::command::WorldCommandDispatcher& dispatcher
        ) const noexcept override {
            dispatcher.dispatch(gameWorld, *this);
        }

        /**
         * @brief Returns the GUID of the entity to despawn.
         *
         * @return Const reference to the entity's GUID.
         */
        [[nodiscard]] const helios::util::Guid& guid() const noexcept {
            return guid_;
        }

        /**
         * @brief Returns the optional pool ID.
         *
         * @return Optional containing the pool ID if specified.
         */
        [[nodiscard]] std::optional<helios::engine::core::data::GameObjectPoolId> gameObjectPoolId() const noexcept {
            return gameObjectPoolId_;
        }

    };


}