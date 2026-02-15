/**
 * @file GameStateTransitionRequest.ixx
 * @brief Request for a game state transition.
 */
module;


export module helios.engine.mechanics.gamestate.types.GameStateTransitionRequest;

import helios.engine.mechanics.gamestate.types.GameState;
import helios.engine.mechanics.gamestate.types.GameStateTransitionId;
import helios.engine.mechanics.gamestate.types.GameStateTransitionType;

export namespace helios::engine::mechanics::gamestate::types {

    /**
     * @brief Encapsulates a request to transition from a given state.
     *
     * Contains the source state and the desired transition identifier.
     * The target state is determined by matching rules in the GameStateManager.
     */
    class GameStateTransitionRequest {

        GameState from_;

        GameStateTransitionId transitionId_;


    public:

        /**
         * @brief Constructs a transition request.
         *
         * @param from The current state from which to transition.
         * @param transitionId The identifier of the requested transition.
         */
        explicit GameStateTransitionRequest(
            const GameState from,
            const GameStateTransitionId transitionId
        ) : from_(from), transitionId_(transitionId) {}

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