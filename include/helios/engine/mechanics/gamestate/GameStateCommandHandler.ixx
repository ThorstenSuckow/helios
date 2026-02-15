/**
 * @file GameStateCommandHandler.ixx
 * @brief Interface for game state command handlers.
 */
module;

export module helios.engine.mechanics.gamestate.GameStateCommandHandler;

import helios.engine.mechanics.gamestate.commands;


export namespace helios::engine::mechanics::gamestate {

    /**
     * @brief Abstract interface for handling game state commands.
     *
     * Implementations receive GameStateCommand instances and process them
     * to trigger state transitions.
     *
     * @see GameStateManager
     * @see GameStateCommand
     */
    class GameStateCommandHandler {

    public:

        virtual ~GameStateCommandHandler() = default;

        /**
         * @brief Submits a game state command for processing.
         *
         * @param gameStateCommand The command to submit.
         * @return True if the command was accepted.
         */
        virtual bool submit(
        const helios::engine::mechanics::gamestate::commands::GameStateCommand& gameStateCommand
        ) noexcept = 0;


    };

}