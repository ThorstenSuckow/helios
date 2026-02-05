/**
 * @file ScoreCommandDispatcher.ixx
 * @brief Dispatcher for routing score commands to the appropriate handler.
 */
module;

#include <cassert>

export module helios.engine.mechanics.scoring.ScoreCommandDispatcher;

import helios.engine.mechanics.scoring.commands;
import helios.engine.runtime.messaging.command.TypedWorldCommandDispatcher;
import helios.engine.runtime.world.GameWorld;


export namespace helios::engine::mechanics::scoring {


    /**
     * @brief Dispatcher that routes UpdateScoreCommand to the registered ScoreCommandHandler.
     *
     * Extends TypedWorldCommandDispatcher to handle score-specific commands.
     * When a command is dispatched, it forwards the score context to the
     * GameWorld's registered ScoreCommandHandler.
     */
    class ScoreCommandDispatcher final : public helios::engine::runtime::messaging::command::TypedWorldCommandDispatcher<
        helios::engine::mechanics::scoring::commands::UpdateScoreCommand> {

    protected:

        /**
         * @brief Dispatches the command to the game world's score handler.
         *
         * @param gameWorld Reference to the game world.
         * @param command The UpdateScoreCommand to dispatch.
         */
        void dispatchTyped(
            helios::engine::runtime::world::GameWorld& gameWorld,
            const helios::engine::mechanics::scoring::commands::UpdateScoreCommand& command
        ) noexcept override {

            auto scoreContext = command.scoreContext();

            if (auto* scoreCommandHandler = gameWorld.scoreCommandHandler()) {
                scoreCommandHandler->submit(command);
            }
        };


    public:

        ScoreCommandDispatcher() = default;

    };


}