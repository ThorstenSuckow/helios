/**
 * @file StateToViewportPolicyUpdateSystem.ixx
 * @brief System that updates active viewports based on game/match state.
 */
module;


#include <cassert>
#include <vector>

export module helios.engine.modules.rendering.viewport.systems.StateToViewportPolicyUpdateSystem;

import helios.runtime.world.GameWorld;
import helios.runtime.world.Session;

import helios.state.StateToIdMapPair;

import helios.runtime.world.UpdateContext;
import helios.rendering.viewport.types.ViewportHandle;
import helios.engine.common.tags.SystemRole;

using namespace helios::state;
using namespace helios::rendering::viewport::types;
export namespace helios::engine::modules::rendering::viewport::systems {

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
     * StateToIdMapPair<GameState, MatchState, ViewportHandle> policy;
     * policy.add(GameState::Running, ViewportHandle("game"));
     * policy.add(MatchState::GameOver, ViewportHandle("game_over"));
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
    class StateToViewportPolicyUpdateSystem {

        /**
         * @brief Policy defining viewport-to-state mappings.
         */
        StateToIdMapPair<StateLft, StateRgt, ViewportHandle> stateToIdMapPair_;

    public:

        using EngineRoleTag = helios::engine::common::tags::SystemRole;

        /**
         * @brief Constructs the system with a state-to-ID map pair.
         *
         * @param stateToIdMapPair Policy mapping states to viewport IDs.
         */
        explicit StateToViewportPolicyUpdateSystem(StateToIdMapPair<StateLft, StateRgt, ViewportHandle> stateToIdMapPair)
            : stateToIdMapPair_(std::move(stateToIdMapPair)){}

        /**
         * @brief Updates the session's active viewport IDs.
         *
         * @details Clears the current viewport IDs and sets them based on
         * the current game and match state combination.
         *
         * @param updateContext The current frame's update context.
         */
        void update(helios::runtime::world::UpdateContext& updateContext) noexcept {

            auto& session = updateContext.session();

            auto gameState  = session.state<StateLft>();
            auto matchState = session.state<StateRgt>();

            session.clearViewportHandles();

            auto viewportIds = stateToIdMapPair_.ids(gameState, matchState);
            session.setViewportHandles(viewportIds);




        }

    };

}

