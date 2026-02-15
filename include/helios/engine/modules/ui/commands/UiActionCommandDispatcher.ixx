/**
 * @file UiActionCommandDispatcher.ixx
 * @brief Dispatcher for routing UI action commands.
 */
module;


export module helios.engine.modules.ui.commands.UiActionCommandDispatcher;

import helios.engine.modules.ui.commands.UiActionCommand;
import helios.engine.runtime.messaging.command.TypedWorldCommandDispatcher;
import helios.engine.runtime.world.GameWorld;


export namespace helios::engine::modules::ui::commands {

    /**
     * @brief Routes UiActionCommand instances to the registered handler.
     *
     * This dispatcher forwards UI action commands to the UiActionCommandHandler
     * registered with the GameWorld.
     *
     * @see UiActionCommandHandler
     * @see UiActionCommand
     */
    class UiActionCommandDispatcher final : public helios::engine::runtime::messaging::command::TypedWorldCommandDispatcher<
        helios::engine::modules::ui::commands::UiActionCommand> {

    protected:

        /**
         * @brief Dispatches a command to the UI action handler.
         *
         * @param gameWorld The game world containing the handler.
         * @param command The command to dispatch.
         */
        void dispatchTyped(
            helios::engine::runtime::world::GameWorld& gameWorld,
            const helios::engine::modules::ui::commands::UiActionCommand& command
        ) noexcept override {

            if (auto* actionCommandHandler = gameWorld.uiActionCommandHandler()) {
                actionCommandHandler->submit(command);
            }
        };


    public:

        UiActionCommandDispatcher() = default;

    };


}