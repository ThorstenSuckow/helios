/**
 * @file GameStateTransitionContext.ixx
 * @brief Context information for a game state transition.
 */
module;


export module helios.engine.mechanics.gamestate.types.GameStateTransitionContext;

import helios.engine.mechanics.gamestate.types.GameState;
import helios.engine.mechanics.gamestate.types.GameStateTransitionId;
import helios.engine.mechanics.gamestate.types.GameStateTransitionType;

export namespace helios::engine::mechanics::gamestate::types {

    /**
     * @brief Encapsulates the context of a state transition.
     *
     * Provides information about the source state, target state,
     * and the transition identifier.
     */
    class GameStateTransitionContext {

        GameState from_;
        GameStateTransitionId transitionId_;
        GameState to_;

    public:

        /**
         * @brief Constructs a transition context.
         *
         * @param from The state being exited.
         * @param to The state being entered.
         * @param transitionId The identifier of the transition.
         */
        explicit GameStateTransitionContext(
            const GameState from ,
            const GameState to,
            const GameStateTransitionId transitionId
        ) : from_(from), to_(to), transitionId_(transitionId) {}

        /**
         * @brief Returns the target state.
         */
        [[nodiscard]] GameState to() const noexcept {
            return to_;
        }

        /**
         * @brief Returns the source state.
         */
        [[nodiscard]] GameState from() const noexcept {
            return from_;
        }

        /**
         * @brief Returns the transition identifier.
         */
        [[nodiscard]] GameStateTransitionId transitionId() const noexcept {
            return transitionId_;
        }

    };

}