/**
 * @file UpdateScoreCommand.ixx
 * @brief Command for updating scores in a score pool.
 */
module;

#include <cassert>
#include <memory>

export module helios.engine.mechanics.scoring.commands.UpdateScoreCommand;

import helios.engine.runtime.messaging.command.WorldCommandDispatcher;

import helios.engine.runtime.messaging.command.WorldCommand;
import helios.engine.mechanics.scoring.types;



export namespace helios::engine::mechanics::scoring::commands {

    /**
     * @brief Command that carries score update information.
     *
     * Contains a ScoreValueContext with the score type, pool ID, and value.
     * Processed by ScoreCommandDispatcher and ultimately handled by
     * ScorePoolManager.
     */
    class UpdateScoreCommand : public helios::engine::runtime::messaging::command::WorldCommand {

        /**
         * @brief The score context containing type, pool, and value.
         */
        helios::engine::mechanics::scoring::types::ScoreValueContext scoreContext_;

    public:

        /**
         * @brief Constructs an UpdateScoreCommand with the given score context.
         *
         * @param scoreContext The context containing score update data.
         */
        template<typename... Args>
        explicit UpdateScoreCommand(
            helios::engine::mechanics::scoring::types::ScoreValueContext scoreContext
        ) : scoreContext_(std::move(scoreContext)) { }

        /**
         * @copydoc WorldCommand::execute()
         */
        virtual void execute(helios::engine::runtime::world::GameWorld& gameWorld) const noexcept {
            assert(false && "Unexpected execute() on UpdateScoreCommand");
        }

        /**
         * @brief Returns the score context (lvalue reference).
         *
         * @return Const reference to the score context.
         */
        [[nodiscard]] const helios::engine::mechanics::scoring::types::ScoreValueContext& scoreContext() const & noexcept {
            return scoreContext_;
        }

        /**
         * @brief Returns the score context (rvalue reference).
         *
         * @return The score context by move.
         */
        [[nodiscard]] helios::engine::mechanics::scoring::types::ScoreValueContext scoreContext() && noexcept {
            return std::move(scoreContext_);
        }

        /**
         * @copydoc WorldCommand::accept()
         */
        virtual void accept(
            helios::engine::runtime::world::GameWorld& gameWorld,
            helios::engine::runtime::messaging::command::WorldCommandDispatcher& dispatcher) const noexcept {

            dispatcher.dispatch(gameWorld, *this);
        }

    };


}