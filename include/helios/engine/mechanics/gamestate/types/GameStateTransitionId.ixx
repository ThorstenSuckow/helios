/**
 * @file GameStateTransitionId.ixx
 * @brief Identifiers for game state transitions.
 */
module;

#include <cstdint>

export module helios.engine.mechanics.gamestate.types.GameStateTransitionId;

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