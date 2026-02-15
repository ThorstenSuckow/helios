/**
 * @file UiActionCommand.ixx
 * @brief Command for triggering UI actions.
 */
module;

#include <cassert>
#include <optional>

export module helios.engine.modules.ui.commands.UiActionCommand;

import helios.engine.runtime.messaging.command.WorldCommand;
import helios.engine.ecs.EntityHandle;

import helios.engine.core.data;

import helios.engine.runtime.messaging.command.WorldCommandDispatcher;

export namespace helios::engine::modules::ui::commands {

    /**
     * @brief A command that encapsulates a UI action request.
     *
     * UiActionCommands are submitted when UI elements are activated
     * (e.g., button pressed, menu item selected). They carry the source
     * entity and the action identifier to execute.
     *
     * @see UiActionCommandManager
     * @see UiActionComponent
     */
    class UiActionCommand : public helios::engine::runtime::messaging::command::WorldCommand {


        helios::engine::ecs::EntityHandle source_;

        helios::engine::core::data::ActionId actionId_;


    public:

        /**
         * @brief Constructs a command with source entity and action ID.
         *
         * @param source The entity that triggered the action.
         * @param actionId The identifier of the action to execute.
         */
        explicit UiActionCommand(
            const helios::engine::ecs::EntityHandle source,
            const helios::engine::core::data::ActionId actionId) :
            source_(source), actionId_(actionId) {}

        /**
         * @brief Direct execution is a no-op; dispatching is required.
         *
         * @param gameWorld The game world (unused).
         */
        void execute(helios::engine::runtime::world::GameWorld& gameWorld) const noexcept override {
            assert(false && "Unexpected execute() for UiActionCommand, needs dispatcher");
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
         * @brief Returns the source entity that triggered the action.
         */
        [[nodiscard]] helios::engine::ecs::EntityHandle source() const noexcept {
            return source_;
        }

        /**
         * @brief Returns the action identifier.
         */
        [[nodiscard]] helios::engine::core::data::ActionId actionId() const noexcept {
            return actionId_;
        }

    };


}