/**
 * @file MatchStateTransitionId.ixx
 * @brief Identifiers for match state transitions.
 */
module;

#include <cstdint>

export module helios.engine.mechanics.match.types.MatchStateTransitionId;

import helios.engine.mechanics.match.types.MatchState;

import helios.engine.state.types.StateTransitionId;

export namespace helios::engine::mechanics::match::types {

    /**
     * @brief Enumeration of transition identifiers.
     *
     * Each identifier represents a specific type of match state transition request.
     */
    enum class  MatchStateTransitionId : uint16_t  {

        Undefined,

        WarmupRequested,

        WarmupDone,

        IntroRequested,

        IntroDone,

        CountdownRequested,

        CountdownDone,

        PlayerSpawnRequested,

        StartRequested,

        TimeExpired,

        PlayerDied,

        RespawnRequested,

        QuitRequested,

        RestartRequested


    };

}


export namespace helios::engine::state::types {

    /**
     * @brief Type trait specialization for MatchState.
     *
     * @details Maps MatchState to its corresponding transition ID type,
     * enabling the generic StateManager to work with MatchState.
     */
    template<>
    struct StateTransitionId<helios::engine::mechanics::match::types::MatchState> {
        using Type = helios::engine::mechanics::match::types::MatchStateTransitionId;
    };
}