/**
 * @file MatchStateCommandHandler.ixx
 * @brief Interface for match state command handlers.
 */
module;

export module helios.engine.mechanics.match.MatchStateCommandHandler;

import helios.engine.mechanics.match.commands;


export namespace helios::engine::mechanics::match {

    /**
     * @brief Abstract interface for handling match state commands.
     *
     * Implementations receive MatchStateCommand instances and process them
     * to trigger state transitions.
     *
     * @see MatchStateManager
     * @see MatchStateCommand
     */
    class MatchStateCommandHandler {

    public:

        virtual ~MatchStateCommandHandler() = default;

        /**
         * @brief Submits a match state command for processing.
         *
         * @param matchStateCommand The command to submit.
         * @return True if the command was accepted.
         */
        virtual bool submit(
        const helios::engine::mechanics::match::commands::MatchStateCommand& matchStateCommand
        ) noexcept = 0;

    };

}