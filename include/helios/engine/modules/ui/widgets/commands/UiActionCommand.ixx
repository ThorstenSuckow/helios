/**
 * @file UiActionCommand.ixx
 * @brief Command for triggering UI actions.
 */
module;

#include <cassert>
#include <optional>

export module helios.engine.modules.ui.widgets.commands.UiActionCommand;

import helios.engine.modules.ui.widgets.types;

import helios.engine.ecs.EntityHandle;

import helios.engine.core.data;


export namespace helios::engine::modules::ui::widgets::commands {

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
    class UiActionCommand  {

        /**
         * @brief Handle of the entity that triggered the action.
         */
        helios::engine::ecs::EntityHandle source_;

        /**
         * @brief Identifier of the action to execute.
         */
        helios::engine::modules::ui::widgets::types::ActionId actionId_;


    public:

        /**
         * @brief Constructs a command with source entity and action ID.
         *
         * @param source The entity that triggered the action.
         * @param actionId The identifier of the action to execute.
         */
        explicit UiActionCommand(
            const helios::engine::ecs::EntityHandle source,
            const helios::engine::modules::ui::widgets::types::ActionId actionId) :
            source_(source), actionId_(actionId) {}


        /**
         * @brief Returns the source entity that triggered the action.
         *
         * @return The entity handle.
         */
        [[nodiscard]] helios::engine::ecs::EntityHandle source() const noexcept {
            return source_;
        }

        /**
         * @brief Returns the action identifier.
         *
         * @return The ActionId.
         */
        [[nodiscard]] helios::engine::modules::ui::widgets::types::ActionId actionId() const noexcept {
            return actionId_;
        }

    };


}