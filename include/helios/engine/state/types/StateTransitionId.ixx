/**
 * @file StateTransitionId.ixx
 * @brief Type trait for mapping state types to transition ID types.
 */
module;

#include <cstdint>
#include <type_traits>

export module helios.engine.state.types.StateTransitionId;



export namespace helios::engine::state::types {

    /**
     * @brief Helper for dependent false in static_assert.
     */
    template<class...>
    inline constexpr bool dependent_false_v = false;

    /**
     * @brief Placeholder type for missing specializations.
     */
    struct InvalidStateTransitionId {};

    /**
     * @brief Type trait mapping StateType to its TransitionId type.
     *
     * @details Must be specialized for each state type. The specialization
     * must define a `Type` alias for the transition ID enum.
     *
     * @tparam StateType The state enum type.
     *
     * @code
     * // Specialization example:
     * template<>
     * struct StateTransitionId<GameState> {
     *     using Type = GameStateTransitionId;
     * };
     * @endcode
     */
    template<typename StateType>
    struct StateTransitionId {
        static_assert(dependent_false_v<StateType>,
            "Missing specialization: StateTransitionId<StateType>::Type must be defined in bindings.");

        using Type = InvalidStateTransitionId;
    };

    /**
     * @brief Convenience alias for transition ID type lookup.
     *
     * @tparam StateType The state enum type.
     */
    template<class StateType>
    using StateTransitionIdType = typename StateTransitionId<StateType>::Type;
}