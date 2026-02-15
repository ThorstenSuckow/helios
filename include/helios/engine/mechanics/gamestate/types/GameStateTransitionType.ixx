/**
 * @file GameStateTransitionType.ixx
 * @brief Types of game state transitions.
 */
module;

#include <cstdint>

export module helios.engine.mechanics.gamestate.types.GameStateTransitionType;

export namespace helios::engine::mechanics::gamestate::types {

    /**
     * @brief Enumeration of transition types.
     *
     * Allows categorizing transitions for special handling.
     */
    enum class  GameStateTransitionType : uint16_t  {

        /**
         * @brief A standard transition.
         */
        Standard,

        /**
         * @brief A transition that starts the game.
         */
        Start,

        /**
         * @brief A transition that restarts the game.
         */
        Restart

    };

}