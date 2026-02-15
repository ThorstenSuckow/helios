/**
 * @file MatchStateCommandDispatcher.ixx
 * @brief Dispatcher for routing match state commands.
 */
module;


export module helios.engine.mechanics.match.dispatcher.MatchStateCommandDispatcher;

import helios.engine.mechanics.match.commands;
import helios.engine.mechanics.match.types;

import helios.engine.runtime.messaging.command.TypedWorldCommandDispatcher;
import helios.engine.runtime.world.GameWorld;


export namespace helios::engine::mechanics::match::dispatcher {

    /**
     * @brief Routes MatchStateCommand instances to the registered handler.
     *
     * This dispatcher forwards match state commands to the MatchStateCommandHandler
     * registered with the GameWorld.
     *
     * @see MatchStateCommandHandler
     * @see MatchStateCommand
     */
    class MatchStateCommandDispatcher final : public helios::engine::runtime::messaging::command::TypedWorldCommandDispatcher<
        helios::engine::mechanics::match::commands::MatchStateCommand> {

    protected:

        /**
         * @brief Dispatches a command to the match state handler.
         *
         * @param gameWorld The game world containing the handler.
         * @param command The command to dispatch.
         */
        void dispatchTyped(
            helios::engine::runtime::world::GameWorld& gameWorld,
            const helios::engine::mechanics::match::commands::MatchStateCommand& command
        ) noexcept override {

            if (auto* matchStateCommandHandler = gameWorld.matchStateCommandHandler()) {
                matchStateCommandHandler->submit(command);
            }
        };


    public:

        MatchStateCommandDispatcher() = default;

    };


}