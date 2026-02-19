/**
 * @file GameStateManager.ixx
 * @brief Type alias for game state manager.
 */
module;

#include <memory>
#include <span>
#include <vector>

export module helios.engine.mechanics.gamestate.GameStateManager;

import helios.engine.state;
import helios.engine.mechanics.gamestate.types;

export namespace helios::engine::mechanics::gamestate {

    /**
     * @brief Type alias for the game state manager.
     *
     * @details Instantiation of the generic StateManager template for
     * GameState. Manages application-level states (Title, Running, Paused).
     *
     * @see helios::engine::state::StateManager
     * @see types::GameState
     */
    using GameStateManager = helios::engine::state::StateManager<types::GameState>;
}
