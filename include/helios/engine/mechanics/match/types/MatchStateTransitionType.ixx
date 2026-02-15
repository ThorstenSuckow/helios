/**
 * @file MatchStateTransitionType.ixx
 * @brief Types of match state transitions.
 */
module;

#include <cstdint>

export module helios.engine.mechanics.match.types.MatchStateTransitionType;

export namespace helios::engine::mechanics::match::types {

    /**
     * @brief Enumeration of transition types.
     *
     * Allows categorizing transitions for special handling.
     */
    enum class  MatchStateTransitionType : uint16_t  {

        /**
         * @brief A standard transition.
         */
        Standard = 0

    };

}