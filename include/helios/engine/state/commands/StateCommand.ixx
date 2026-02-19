/**
 * @file StateCommand.ixx
 * @brief Command for requesting state transitions.
 */
module;

#include <cassert>
#include <memory>

export module helios.engine.state.commands.StateCommand;


import helios.engine.state.types;


import helios.engine.runtime.messaging.command.WorldCommand;
import helios.engine.runtime.messaging.command.WorldCommandDispatcher;



export namespace helios::engine::state::commands {

    using namespace helios::engine::state::types;

    /**
     * @brief Command that requests a state transition.
     *
     * @details Encapsulates a StateTransitionRequest and routes it through
     * the command dispatcher to the appropriate StateManager. The command
     * does not execute directly; instead it uses the visitor pattern via
     * accept() to be dispatched.
     *
     * @tparam StateType The state enum type.
     *
     * @see StateTransitionRequest
     * @see StateCommandDispatcher
     * @see StateManager
     */
    template<typename StateType>
    class StateCommand : public helios::engine::runtime::messaging::command::WorldCommand {

        /**
         * @brief The transition request to process.
         */
        StateTransitionRequest<StateType> transitionRequest_;


    public:

        /**
         * @brief Constructs a state command.
         *
         * @param transitionRequest The transition request.
         */
        explicit StateCommand(StateTransitionRequest<StateType> transitionRequest)
            : transitionRequest_(transitionRequest) {}

        /**
         * @brief Returns the transition request.
         *
         * @return The encapsulated transition request.
         */
        [[nodiscard]] types::StateTransitionRequest<StateType> transitionRequest() const noexcept {
            return transitionRequest_;
        }

        /**
         * @brief Direct execution is not supported.
         *
         * @details State commands must be dispatched via accept().
         *
         * @param gameWorld The game world (unused).
         */
        void execute(helios::engine::runtime::world::GameWorld& gameWorld) const noexcept override {
            assert(false && "Unexpected execute() on StateCommand");
        }

        /**
         * @brief Dispatches this command through the visitor.
         *
         * @param gameWorld The game world.
         * @param dispatcher The command dispatcher.
         */
        virtual void accept(
            helios::engine::runtime::world::GameWorld& gameWorld,
            helios::engine::runtime::messaging::command::WorldCommandDispatcher& dispatcher) const noexcept override {

            dispatcher.dispatch(gameWorld, *this);

        };


    };


}