/**
 * @file StateTransitionRule.ixx
 * @brief Definition of a state transition rule.
 */
module;

#include <functional>

export module helios.engine.state.types.StateTransitionRule;

import helios.engine.state.types.StateTransitionRequest;
import helios.engine.state.types.StateTransitionId;
import helios.engine.state.types.StateTransitionType;


import helios.engine.runtime.world.UpdateContext;

export namespace helios::engine::state::types {

    /**
     * @brief Function pointer type for transition guards.
     *
     * @details Guards are called before a transition executes. If the guard
     * returns false, the transition is blocked.
     *
     * @tparam StateType The state enum type.
     */
    template<typename StateType>
    using GuardCallback = bool(*)(
        helios::engine::runtime::world::UpdateContext&,
        const StateTransitionRequest<StateType>
    );

    /**
     * @brief Defines a valid state transition with optional guard.
     *
     * @details Rules are registered with the StateManager and define which
     * transitions are valid. Each rule specifies a source state, transition
     * ID, target state, and an optional guard callback.
     *
     * @tparam StateType The state enum type.
     *
     * @see StateManager
     * @see GuardCallback
     */
    template<typename StateType>
    class StateTransitionRule {

        using StateTransitionIdType = StateTransitionIdType<StateType>;

        /**
         * @brief The state this rule applies to.
         */
        StateType from_;

        /**
         * @brief The transition ID that triggers this rule.
         */
        StateTransitionIdType transitionId_;

        /**
         * @brief The target state after transition.
         */
        StateType to_;

        /**
         * @brief Optional guard callback.
         */
        GuardCallback<StateType> guard_ = nullptr;

    public:

        /**
         * @brief Constructs a transition rule.
         *
         * @param from The source state.
         * @param transitionId The transition that triggers this rule.
         * @param to The target state.
         * @param guard Optional guard callback (default: nullptr).
         */
        constexpr  StateTransitionRule(
            const StateType from,
            const StateTransitionIdType transitionId,
            const StateType to,
            GuardCallback<StateType> guard = nullptr
        ) : from_(from), transitionId_(transitionId), to_(to) {
            if (guard != nullptr) {
                guard_ = std::move(guard);
            }
        }

        /**
         * @brief Returns the source state.
         *
         * @return The state this rule applies to.
         */
        [[nodiscard]] StateType from() const noexcept {
            return from_;
        }

        /**
         * @brief Returns the target state.
         *
         * @return The state after transition.
         */
        [[nodiscard]] StateType to() const noexcept { return to_; }

        /**
         * @brief Returns the transition identifier.
         *
         * @return The transition ID that triggers this rule.
         */
        [[nodiscard]] StateTransitionIdType transitionId() const noexcept {
            return transitionId_;
        }

        /**
         * @brief Returns the guard callback.
         *
         * @return The guard function pointer, or nullptr if no guard.
         */
        [[nodiscard]] GuardCallback<StateType> guard() const noexcept {
            return guard_;
        }

    };

}