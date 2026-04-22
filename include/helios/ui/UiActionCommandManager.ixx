/**
 * @file UiActionCommandManager.ixx
 * @brief Manager for processing UI action commands.
 */
module;

#include <unordered_map>
#include <functional>
#include <cassert>
#include <helios/helios_config.h>

export module helios.ui.UiActionCommandManager;

import helios.ui.widgets.commands;
import helios.ui.widgets.types;
import helios.ui.widgets.types.ActionId;


import helios.runtime.world.GameObject;

import helios.runtime.world.UpdateContext;

import helios.runtime.messaging.command.CommandHandlerRegistry;

import helios.runtime.world.tags.ManagerRole;

using namespace helios::ui::widgets::types;
using namespace helios::ui::widgets::commands;

export namespace helios::ui {

    /**
     * @brief Manages and processes UI action commands with policy-based dispatch.
     *
     * Collects UiActionCommand instances and dispatches them to registered
     * action callbacks based on ActionId. Supports policy registration for
     * handling different action types.
     *
     * @see UiActionCommand
     * @see CommandHandlerRegistry
     */
    template<typename THandle>
    class UiActionCommandManager {

        using ActionCallback = std::function<void(
            helios::runtime::world::UpdateContext& updateContext, const UiActionCommand<THandle>& command)>;

        /**
         * @brief Queue of pending UI action commands.
         */
        std::vector<UiActionCommand<THandle>> commands_;

        /**
         * @brief Map from ActionId to their handling callbacks.
         */
        std::unordered_map<ActionId, ActionCallback> policies_;

    public:
        using EngineRoleTag = helios::runtime::tags::ManagerRole;

        /**
         * @brief Constructs the manager with default capacity.
         */
        UiActionCommandManager() {
            commands_.reserve(UI_ACTION_COMMAND_HANDLER_DEFAULT_CAPACITY);
        };

        /**
         * @brief Processes all pending commands by invoking their registered policies.
         *
         * @param update_context The current update context.
         */
        void flush(
            helios::runtime::world::UpdateContext& update_context
        ) noexcept {

            for (auto cmd : commands_) {
                if (policies_.contains(cmd.actionId())) {
                    policies_[cmd.actionId()](
                        update_context, cmd
                    );
                }
            }

            commands_.clear();
        }

        /**
         * @brief Submits a UI action command for processing.
         *
         * @param uiActionCommand The command to submit.
         *
         * @return Always returns true.
         */
        bool submit(UiActionCommand<THandle> uiActionCommand) noexcept {

            commands_.push_back(std::move(uiActionCommand));

            return true;
        };

        /**
         * @brief Registers a callback for a specific action ID.
         *
         * @param actionId The action ID to register.
         * @param callback The callback to invoke when the action is triggered.
         *
         * @return Reference to this manager for method chaining.
         */
        UiActionCommandManager& addPolicy(
            const ActionId actionId,
            ActionCallback callback
        ) {
            assert(policies_.find(actionId) == policies_.end() && "Action already registered");

            policies_[actionId] = std::move(callback);
            return *this;
        }

        /**
         * @brief Initializes the manager and registers it in the command-handler registry.
         *
         * @param commandHandlerRegistry The command-handler registry to register with.
         */
        void init(helios::runtime::messaging::command::CommandHandlerRegistry& commandHandlerRegistry) {
            commandHandlerRegistry.registerHandler<UiActionCommand<THandle>>(*this);
        }

    };

}
