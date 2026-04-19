/**
 * @file GameStateTransitionId.ixx
 * @brief Identifiers for game state transitions.
 */
module;

#include <cstdint>

export module helios.gameplay.gamestate.types.GameStateTransitionId;
import helios.gameplay.gamestate.types.GameState;



export namespace helios::gameplay::gamestate::types {

    /**
     * @brief Enumeration of transition identifiers.
     *
     * Each identifier represents a specific type of state transition request.
     */
    enum class  GameStateTransitionId : uint16_t  {

        Undefined = 0,

        BootRequest,

        WarmupRequest,

        WarmupDoneSignal,

        ReadyMatchRequest,

        StartMatchRequest,

        TogglePause,

        TitleRequest,

        ShutdownRequest

    };

}

