/**
 * @file MatchStateTransitionId.ixx
 * @brief Identifiers for match state transitions.
 */
module;

#include <cstdint>

export module helios.gameplay.match.types.MatchStateTransitionId;

import helios.gameplay.match.types.MatchState;



export namespace helios::gameplay::match::types {

    /**
     * @brief Enumeration of transition identifiers.
     *
     * Each identifier represents a specific type of match state transition request.
     */
    enum class  MatchStateTransitionId : uint16_t  {

        Undefined,

        WarmupRequest,

        StartRequest,

        QuitRequest,

        CountdownRequest,

        PlayerDied,

        PlayerSpawnRequest,

        GameOverRequest

    };

}

