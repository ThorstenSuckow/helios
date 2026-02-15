/**
 * @file GameStateCommand.ixx
 * @brief Command for requesting game state transitions.
 */
module;

#include <cassert>

export module helios.engine.mechanics.gamestate.commands.GameStateCommand;


import helios.engine.mechanics.gamestate.types;


import helios.engine.runtime.messaging.command.WorldCommand;
import helios.engine.runtime.messaging.command.WorldCommandDispatcher;



export namespace helios::engine::mechanics::gamestate::commands {

    using namespace helios::engine::mechanics::gamestate::types;

    /**
     * @brief A command that encapsulates a game state transition request.
     *
     * GameStateCommands are submitted to the CommandBuffer and routed via
     * dispatchers to the GameStateManager for processing.
     *
     * @see GameStateTransitionRequest
     * @see GameStateManager
     */
    class GameStateCommand : public helios::engine::runtime::messaging::command::WorldCommand {


        GameStateTransitionRequest transitionRequest_;


    public:

        /**
         * @brief Constructs a command with the given transition request.
         *
         * @param transitionRequest The transition request to encapsulate.
         */
        explicit GameStateCommand(const GameStateTransitionRequest transitionRequest)
            : transitionRequest_(transitionRequest) {}

        /**
         * @brief Returns the encapsulated transition request.
         *
         * @return The transition request.
         */
        [[nodiscard]] types::GameStateTransitionRequest transitionRequest() const noexcept {
            return transitionRequest_;
        }

        /**
         * @brief Direct execution is a no-op; dispatching is required.
         *
         * @param gameWorld The game world (unused).
         */
        void execute(helios::engine::runtime::world::GameWorld& gameWorld) const noexcept override {
            assert(false && "Unexpected execute() on GameStateCommand");
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