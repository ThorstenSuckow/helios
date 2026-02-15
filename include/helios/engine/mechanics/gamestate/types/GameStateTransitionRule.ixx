/**
 * @file GameStateTransitionRule.ixx
 * @brief Definition of a game state transition rule.
 */
module;

#include <functional>

export module helios.engine.mechanics.gamestate.types.GameStateTransitionRule;

import helios.engine.mechanics.gamestate.types.GameState;
import helios.engine.mechanics.gamestate.types.GameStateTransitionRequest;
import helios.engine.mechanics.gamestate.types.GameStateTransitionId;
import helios.engine.mechanics.gamestate.types.GameStateTransitionType;


import helios.engine.runtime.world.UpdateContext;

export namespace helios::engine::mechanics::gamestate::types {

    /**
     * @brief Function pointer type for transition guard callbacks.
     *
     * @param updateContext The current update context.
     * @param request The transition request being evaluated.
     * @return True if the transition should proceed.
     */
    using GuardCallback = bool(*)(
        helios::engine::runtime::world::UpdateContext&,
        GameStateTransitionRequest
    );

    /**
     * @brief Defines a valid state transition with optional guard.
     *
     * A rule specifies the source state, transition identifier, target state,
     * and an optional guard callback that must return true for the transition
     * to be applied.
     */
    class GameStateTransitionRule {

        GameState from_;

        GameStateTransitionId transitionId_;

        GameState to_;

        GameStateTransitionType transitionType_;

        GuardCallback guard_ = nullptr;

    public:

        /**
         * @brief Constructs a transition rule.
         *
         * @param from The source state.
         * @param transitionId The transition identifier.
         * @param to The target state.
         * @param transitionType The type of transition.
         * @param guard Optional guard callback.
         */
        constexpr  GameStateTransitionRule(
            const GameState from,
            const GameStateTransitionId transitionId,
            const GameState to,
            const GameStateTransitionType transitionType,
            GuardCallback guard = nullptr
        ) : from_(from), transitionId_(transitionId), to_(to), transitionType_(transitionType) {
            if (guard != nullptr) {
                guard_ = std::move(guard);
            }
        }

        /**
         * @brief Returns the source state.
         */
        [[nodiscard]] GameState from() const noexcept {
            return from_;
        }

        /**
         * @brief Returns the target state.
         */
        [[nodiscard]] GameState to() const noexcept { return to_; }

        /**
         * @brief Returns the transition type.
         */
        [[nodiscard]] GameStateTransitionType transitionType() const noexcept {
            return transitionType_;
        }

        /**
         * @brief Returns the transition identifier.
         */
        [[nodiscard]] GameStateTransitionId transitionId() const noexcept {
            return transitionId_;
        }

        /**
         * @brief Returns the guard callback, or nullptr if none.
         */
        [[nodiscard]] GuardCallback guard() const noexcept {
            return guard_;
        }

    };

}