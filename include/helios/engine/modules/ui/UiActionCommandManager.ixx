/**
 * @file UiActionCommandManager.ixx
 * @brief Manager for processing UI action commands.
 */
module;

#include <unordered_map>
#include <functional>
#include <cassert>
#include <helios/helios_config.h>

export module helios.engine.modules.ui.UiActionCommandManager;

import helios.engine.modules.ui.commands;
import helios.engine.core.data;

import helios.engine.modules.ui.UiActionCommandHandler;

import helios.engine.ecs.GameObject;

import helios.engine.runtime.world.Manager;
import helios.engine.runtime.world.UpdateContext;

import helios.engine.runtime.world.GameWorld;


export namespace helios::engine::modules::ui {

    /**
     * @brief Manages and processes UI action commands with policy-based dispatch.
     *
     * Collects UiActionCommand instances and dispatches them to registered
     * action callbacks based on ActionId. Supports policy registration for
     * handling different action types.
     *
     * @see UiActionCommandHandler
     * @see UiActionCommand
     */
    class UiActionCommandManager : public helios::engine::runtime::world::Manager,
                             public UiActionCommandHandler {

        using ActionCallback = std::function<void(
            helios::engine::runtime::world::UpdateContext& updateContext,
            const helios::engine::modules::ui::commands::UiActionCommand& command)>;

        std::vector<helios::engine::modules::ui::commands::UiActionCommand> commands_;

        std::unordered_map<
            helios::engine::core::data::ActionId, ActionCallback
        > policies_;

    public:

        /**
         * @brief Constructs the manager with default capacity.
         */
        UiActionCommandManager() {
            commands_.reserve(UI_ACTION_COMMAND_HANDLER_DEFAULT_CAPACITY);
        };

        /**
         * @brief Processes all pending commands by invoking their registered policies.
         *
         * @param gameWorld The game world.
         * @param update_context The current update context.
         */
        void flush(
            helios::engine::runtime::world::GameWorld& gameWorld,
            helios::engine::runtime::world::UpdateContext& update_context
        ) noexcept override {

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
         * @return True if the command was accepted.
         */
        bool submit(
            commands::UiActionCommand uiActionCommand
        ) noexcept override {

            commands_.push_back(std::move(uiActionCommand));

            return true;
        };

        /**
         * @brief Registers a callback for a specific action ID.
         *
         * @param actionId The action ID to register.
         * @param callback The callback to invoke when the action is triggered.
         * @return Reference to this manager for method chaining.
         */
        UiActionCommandManager& addPolicy(
            const helios::engine::core::data::ActionId actionId,
            ActionCallback callback
        ) {
            assert(policies_.find(actionId) == policies_.end() && "Action already registered");

            policies_[actionId] = std::move(callback);
            return *this;
        }

        /**
         * @brief Initializes the manager and registers it with the game world.
         *
         * @param gameWorld The game world to register with.
         */
        void init(helios::engine::runtime::world::GameWorld& gameWorld) override {
            gameWorld.registerUiActionCommandHandler(*this);
        }

    };

}
