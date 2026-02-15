/**
 * @file MatchStateTransitionId.ixx
 * @brief Identifiers for match state transitions.
 */
module;

#include <cstdint>

export module helios.engine.mechanics.match.types.MatchStateTransitionId;

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