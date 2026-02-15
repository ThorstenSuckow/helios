/**
 * @file MatchStateTransitionRule.ixx
 * @brief Definition of a match state transition rule.
 */
module;

#include <functional>

export module helios.engine.mechanics.match.types.MatchStateTransitionRule;

import helios.engine.mechanics.match.types.MatchState;
import helios.engine.mechanics.match.types.MatchStateTransitionRequest;
import helios.engine.mechanics.match.types.MatchStateTransitionId;
import helios.engine.mechanics.match.types.MatchStateTransitionType;


import helios.engine.runtime.world.UpdateContext;

export namespace helios::engine::mechanics::match::types {

    /**
     * @brief Function pointer type for transition guard callbacks.
     *
     * @param updateContext The current update context.
     * @param request The transition request being evaluated.
     * @return True if the transition should proceed.
     */
    using GuardCallback = bool(*)(
        helios::engine::runtime::world::UpdateContext&,
        MatchStateTransitionRequest
    );

    /**
     * @brief Defines a valid state transition with optional guard.
     *
     * A rule specifies the source state, transition identifier, target state,
     * and an optional guard callback that must return true for the transition
     * to be applied.
     */
    class MatchStateTransitionRule {

        MatchState from_;

        MatchStateTransitionId transitionId_;

        MatchState to_;

        MatchStateTransitionType transitionType_;

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
        constexpr  MatchStateTransitionRule(
            const MatchState from,
            const MatchStateTransitionId transitionId,
            const MatchState to,
            const MatchStateTransitionType transitionType,
            GuardCallback guard = nullptr
        ) : from_(from), transitionId_(transitionId), to_(to), transitionType_(transitionType) {
            if (guard != nullptr) {
                guard_ = std::move(guard);
            }
        }

        /**
         * @brief Returns the source state.
         */
        [[nodiscard]] MatchState from() const noexcept {
            return from_;
        }

        /**
         * @brief Returns the target state.
         */
        [[nodiscard]] MatchState to() const noexcept { return to_; }

        /**
         * @brief Returns the transition type.
         */
        [[nodiscard]] MatchStateTransitionType transitionType() const noexcept {
            return transitionType_;
        }

        /**
         * @brief Returns the transition identifier.
         */
        [[nodiscard]] MatchStateTransitionId transitionId() const noexcept {
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