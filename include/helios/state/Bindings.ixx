/**
 * @file Bindings.ixx
 * @brief Compile-time bindings mapping state types to their transition ID types.
 */
module;

export module helios.state.Bindings;

export import helios.state.types.StateTransitionId;

export import helios.gameplay.gamestate.types.GameState;
export import helios.gameplay.matchstate.types.MatchState;

export import helios.gameplay.gamestate.types.GameStateTransitionId;
export import helios.gameplay.matchstate.types.MatchStateTransitionId;

export namespace helios::state::types {

    /**
     * @brief Binds GameState to GameStateTransitionId.
     *
     * @details Allows `StateCommand<GameState>` and related template machinery
     * to resolve the correct transition ID type at compile time.
     *
     * @see StateTransitionId
     * @see GameState
     * @see GameStateTransitionId
     */
    template<>
    struct StateTransitionId<helios::gameplay::gamestate::types::GameState> {
        using Type = helios::gameplay::gamestate::types::GameStateTransitionId;
    };

    /**
     * @brief Binds MatchState to MatchStateTransitionId.
     *
     * @details Allows `StateCommand<MatchState>` and related template machinery
     * to resolve the correct transition ID type at compile time.
     *
     * @see StateTransitionId
     * @see MatchState
     * @see MatchStateTransitionId
     */
    template<>
    struct StateTransitionId<helios::gameplay::matchstate::types::MatchState> {
        using Type = helios::gameplay::matchstate::types::MatchStateTransitionId;
    };

}