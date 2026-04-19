/**
 * @file StateTransitionType.ixx
 * @brief Empty type trait for state transition customization.
 */
module;

#include <cstdint>

export module helios.state.types.StateTransitionType;

export namespace helios::state::types {

    /**
     * @brief Type trait for state transition customization points.
     *
     * @details Can be specialized to provide additional type information
     * for state transitions. Currently serves as a placeholder for
     * future extension.
     *
     * @tparam StateType The state enum type.
     */
    template<typename StateType>
    struct StateTransitionType {};
}