/**
 * @file GameStateCommandDispatcher.ixx
 * @brief Dispatcher for routing game state commands.
 */
module;


export module helios.engine.mechanics.gamestate.dispatcher.GameStateCommandDispatcher;

import helios.engine.mechanics.gamestate.commands;
import helios.engine.mechanics.gamestate.types;

import helios.engine.runtime.messaging.command.TypedWorldCommandDispatcher;
import helios.engine.runtime.world.GameWorld;


export namespace helios::engine::mechanics::gamestate::dispatcher {

    /**
     * @brief Routes GameStateCommand instances to the registered handler.
     *
     * This dispatcher forwards game state commands to the GameStateCommandHandler
     * registered with the GameWorld.
     *
     * @see GameStateCommandHandler
     * @see GameStateCommand
     */
    class GameStateCommandDispatcher final : public helios::engine::runtime::messaging::command::TypedWorldCommandDispatcher<
        helios::engine::mechanics::gamestate::commands::GameStateCommand> {

    protected:


        /**
         * @brief Dispatches a command to the game state handler.
         *
         * @param gameWorld The game world containing the handler.
         * @param command The command to dispatch.
         */
        void dispatchTyped(
            helios::engine::runtime::world::GameWorld& gameWorld,
            const helios::engine::mechanics::gamestate::commands::GameStateCommand& command
        ) noexcept override {

            if (auto* gameStateCommandHandler = gameWorld.gameStateCommandHandler()) {
                gameStateCommandHandler->submit(command);
            }
        };


    public:

        GameStateCommandDispatcher() = default;

    };


}