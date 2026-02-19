/**
 * @file StateCommandDispatcher.ixx
 * @brief Dispatcher for routing state commands to handlers.
 */
module;


export module helios.engine.state.dispatcher.StateCommandDispatcher;

import helios.engine.state.commands;
import helios.engine.state.types;

import helios.engine.runtime.messaging.command.TypedWorldCommandDispatcher;
import helios.engine.runtime.world.GameWorld;


export namespace helios::engine::state::dispatcher {

    /**
     * @brief Routes state commands to the appropriate handler.
     *
     * @details Implements the TypedWorldCommandDispatcher interface for
     * StateCommand. Retrieves the registered StateCommandHandler from
     * GameWorld and submits the command.
     *
     * @tparam StateType The state enum type.
     *
     * @see StateCommand
     * @see TypedStateCommandHandler
     * @see StateManager
     */
    template<typename StateType>
    class StateCommandDispatcher final : public helios::engine::runtime::messaging::command::TypedWorldCommandDispatcher<
        helios::engine::state::commands::StateCommand<StateType>> {

    protected:

        /**
         * @brief Dispatches a state command to its handler.
         *
         * @param gameWorld The game world.
         * @param command The state command to dispatch.
         */
        void dispatchTyped(
            helios::engine::runtime::world::GameWorld& gameWorld,
            const helios::engine::state::commands::StateCommand<StateType>& command
        ) noexcept override {

            if (auto* gameStateCommandHandler = gameWorld.stateCommandHandler<StateType>()) {
                gameStateCommandHandler->submit(command);
            }
        };


    public:

        StateCommandDispatcher() = default;

    };


}