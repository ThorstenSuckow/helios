/**
 * @file Bindings.ixx
 * @brief Compile-time bindings mapping state types to their transition ID types.
 */
module;

export module helios.engine.state.Bindings;

export import helios.engine.state.types.StateTransitionId;

export import helios.engine.mechanics.gamestate.types.GameState;
export import helios.engine.mechanics.match.types.MatchState;

export import helios.engine.mechanics.gamestate.types.GameStateTransitionId;
export import helios.engine.mechanics.match.types.MatchStateTransitionId;

export namespace helios::engine::state::types {

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
    struct StateTransitionId<helios::engine::mechanics::gamestate::types::GameState> {
        using Type = helios::engine::mechanics::gamestate::types::GameStateTransitionId;
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
    struct StateTransitionId<helios::engine::mechanics::match::types::MatchState> {
        using Type = helios::engine::mechanics::match::types::MatchStateTransitionId;
    };

}