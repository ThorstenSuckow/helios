/**
 * @file GameStateTransitionId.ixx
 * @brief Identifiers for game state transitions.
 */
module;

#include <cstdint>

export module helios.engine.mechanics.gamestate.types.GameStateTransitionId;
import helios.engine.mechanics.gamestate.types.GameState;

import helios.engine.state.types.StateTransitionId;

export namespace helios::engine::mechanics::gamestate::types {

    /**
     * @brief Enumeration of transition identifiers.
     *
     * Each identifier represents a specific type of state transition request.
     */
    enum class  GameStateTransitionId : uint16_t  {

        Undefined = 0,

        TitleRequested,
        TitleShowing,

        StartRequested,
        Starting,

        ResumeRequested,
        Resuming,

        TogglePause,

        LoadRequested,
        Loading,

        QuitGameRequested,
        QuittingGame,

        RestartRequested,
        Restarting

    };

}

export namespace helios::engine::state::types {

    /**
     * @brief Type trait specialization for GameState.
     *
     * @details Maps GameState to its corresponding transition ID type,
     * enabling the generic StateManager to work with GameState.
     */
    template<>
    struct StateTransitionId<helios::engine::mechanics::gamestate::types::GameState> {
        using Type = helios::engine::mechanics::gamestate::types::GameStateTransitionId;
    };

}