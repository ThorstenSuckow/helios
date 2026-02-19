/**
 * @file MatchStateManager.ixx
 * @brief Type alias for match state manager.
 */
module;

#include <span>
#include <memory>
#include <unordered_map>

export module helios.engine.mechanics.match.MatchStateManager;

import helios.engine.mechanics.match.types;
import helios.engine.state;





export namespace helios::engine::mechanics::match {

    /**
     * @brief Type alias for the match state manager.
     *
     * @details Instantiation of the generic StateManager template for
     * MatchState. Manages match/round lifecycle states (Warmup, PlayerSpawn, Playing).
     *
     * @see helios::engine::state::StateManager
     * @see types::MatchState
     */
    using MatchStateManager = helios::engine::state::StateManager<helios::engine::mechanics::match::types::MatchState>;

}
