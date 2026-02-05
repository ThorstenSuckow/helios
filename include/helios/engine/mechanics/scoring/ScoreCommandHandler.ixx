/**
 * @file ScoreCommandHandler.ixx
 * @brief Abstract interface for handling score commands.
 */
module;

export module helios.engine.mechanics.scoring.ScoreCommandHandler;

import helios.engine.mechanics.scoring.commands;


export namespace helios::engine::mechanics::scoring {


    /**
     * @brief Abstract interface for processing score update commands.
     *
     * Implementations receive UpdateScoreCommand instances and process them
     * according to the specific scoring logic (e.g., adding to score pools).
     */
    class ScoreCommandHandler {

    public:

        virtual ~ScoreCommandHandler() = default;

        /**
         * @brief Submits a score update command for processing.
         *
         * @param updateScoreCommand The command containing score context.
         *
         * @return True if the command was accepted, false otherwise.
         */
        virtual bool submit(
            const helios::engine::mechanics::scoring::commands::UpdateScoreCommand& updateScoreCommand
        ) noexcept = 0;


    };

}