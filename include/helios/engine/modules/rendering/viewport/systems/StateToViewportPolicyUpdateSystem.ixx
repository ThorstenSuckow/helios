/**
 * @file StateToViewportPolicyUpdateSystem.ixx
 * @brief System that updates active viewports based on game/match state.
 */
module;


#include <cassert>
#include <vector>

export module helios.engine.modules.rendering.viewport.systems.StateToViewportPolicyUpdateSystem;

import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.world.Session;

import helios.engine.state.StateToIdMapPair;

import helios.engine.ecs.System;
import helios.engine.runtime.world.UpdateContext;

import helios.engine.core.data;

export namespace helios::engine::modules::rendering::viewport::systems {

    using namespace helios::engine::state;

    /**
     * @brief Updates the session's active viewport list based on state policy.
     *
     * @details Queries the current states from the session using the configured
     * template parameters, then uses StateToIdMapPair to determine which viewports
     * should be active. The resulting viewport IDs are stored in the session
     * for use by the rendering system.
     *
     * ## Usage
     *
     * ```cpp
     * StateToIdMapPair<GameState, MatchState, ViewportId> policy;
     * policy.add(GameState::Running, ViewportId("game"));
     * policy.add(MatchState::GameOver, ViewportId("game_over"));
     * policy.freeze();
     *
     * gameLoop.phase(PhaseType::Pre)
     *     .addPass<GameState>(GameState::Any)
     *         .addSystem<StateToViewportPolicyUpdateSystem<GameState, MatchState>>(
     *             std::move(policy)
     *         );
     * ```
     *
     * @tparam StateLft The left/primary state type (e.g., GameState).
     * @tparam StateRgt The right/secondary state type (e.g., MatchState).
     *
     * @see StateToIdMapPair
     * @see Session
     */
    template<typename StateLft, typename StateRgt>
    class StateToViewportPolicyUpdateSystem : public helios::engine::ecs::System {

        /**
         * @brief Policy defining viewport-to-state mappings.
         */
        StateToIdMapPair<StateLft, StateRgt, ViewportId> stateToIdMapPair_;

    public:

        /**
         * @brief Constructs the system with a state-to-ID map pair.
         *
         * @param stateToIdMapPair Policy mapping states to viewport IDs.
         */
        explicit StateToViewportPolicyUpdateSystem(StateToIdMapPair<StateLft, StateRgt, ViewportId> stateToIdMapPair)
            : stateToIdMapPair_(std::move(stateToIdMapPair)){}

        /**
         * @brief Updates the session's active viewport IDs.
         *
         * @details Clears the current viewport IDs and sets them based on
         * the current game and match state combination.
         *
         * @param updateContext The current frame's update context.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept override {

            auto& session = updateContext.session();

            auto gameState  = session.state<StateLft>();
            auto matchState = session.state<StateRgt>();

            session.clearViewportIds();

            auto viewportIds = stateToIdMapPair_.ids(gameState, matchState);
            session.setViewportIds(viewportIds);




        }

    };

}

