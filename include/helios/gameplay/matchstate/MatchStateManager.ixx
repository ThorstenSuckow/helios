/**
 * @file MatchStateManager.ixx
 * @brief Type alias for match state manager.
 */
module;

#include <span>
#include <memory>
#include <unordered_map>

export module helios.gameplay.matchstate.MatchStateManager;

import helios.gameplay.matchstate.types;
import helios.engine.state;





export namespace helios::gameplay::matchstate {

    /**
     * @brief Type alias for the match state manager.
     *
     * @details Instantiation of the generic StateManager template for
     * MatchState. Manages match/round lifecycle states (Warmup, PlayerSpawn, Playing).
     *
     * @see helios::engine::state::StateManager
     * @see types::MatchState
     */
    using MatchStateManager = helios::engine::state::StateManager<helios::gameplay::matchstate::types::MatchState>;

}
