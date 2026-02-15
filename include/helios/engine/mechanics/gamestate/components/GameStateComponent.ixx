/**
 * @file GameStateComponent.ixx
 * @brief Component for storing game state on an entity.
 */
module;

export module helios.engine.mechanics.gamestate.components.GameStateComponent;

import helios.engine.mechanics.gamestate.types;

export namespace helios::engine::mechanics::gamestate::components {

    /**
     * @brief Stores the current game state associated with an entity.
     *
     * Typically attached to a session or global entity to track the
     * current state of the game (e.g., Title, Running, Paused).
     */
    struct GameStateComponent  {

        /**
         * @brief The current game state.
         */
        helios::engine::mechanics::gamestate::types::GameState gameState;


    };


}