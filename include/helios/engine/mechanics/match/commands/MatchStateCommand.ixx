/**
 * @file MatchStateCommand.ixx
 * @brief Command for requesting match state transitions.
 */
module;

#include <cassert>

export module helios.engine.mechanics.match.commands.MatchStateCommand;


import helios.engine.mechanics.match.types;


import helios.engine.runtime.messaging.command.WorldCommand;
import helios.engine.runtime.messaging.command.WorldCommandDispatcher;



export namespace helios::engine::mechanics::match::commands {

    using namespace helios::engine::mechanics::match::types;

    /**
     * @brief A command that encapsulates a match state transition request.
     *
     * MatchStateCommands are submitted to the CommandBuffer and routed via
     * dispatchers to the MatchStateManager for processing.
     *
     * @see MatchStateTransitionRequest
     * @see MatchStateManager
     */
    class MatchStateCommand : public helios::engine::runtime::messaging::command::WorldCommand {


        MatchStateTransitionRequest transitionRequest_;


    public:

        /**
         * @brief Constructs a command with the given transition request.
         *
         * @param transitionRequest The transition request to encapsulate.
         */
        explicit MatchStateCommand(const MatchStateTransitionRequest transitionRequest)
            : transitionRequest_(transitionRequest) {}

        /**
         * @brief Returns the encapsulated transition request.
         *
         * @return The transition request.
         */
        [[nodiscard]] types::MatchStateTransitionRequest transitionRequest() const noexcept {
            return transitionRequest_;
        }

        /**
         * @brief Direct execution is a no-op; dispatching is required.
         *
         * @param gameWorld The game world (unused).
         */
        void execute(helios::engine::runtime::world::GameWorld& gameWorld) const noexcept override {
            assert(false && "Unexpected execute() on MatchStateCommand");
        }

        /**
         * @brief Accepts a dispatcher for command routing.
         *
         * @param gameWorld The game world.
         * @param dispatcher The dispatcher to route this command to.
         */
        virtual void accept(
            helios::engine::runtime::world::GameWorld& gameWorld,
            helios::engine::runtime::messaging::command::WorldCommandDispatcher& dispatcher) const noexcept override {

            dispatcher.dispatch(gameWorld, *this);

        };


    };


}