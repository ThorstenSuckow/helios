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

import helios.engine.mechanics.gamestate.types;
import helios.engine.mechanics.match.types;

import helios.engine.core.data;

export namespace helios::engine::modules::rendering::viewport::systems {


    using namespace helios::engine::mechanics::match::types;
    using namespace helios::engine::mechanics::gamestate::types;
    using namespace helios::engine::state;

    /**
     * @brief Updates the session's active viewport list based on state policy.
     *
     * @details Queries the current GameState and MatchState from the session,
     * then uses the configured StateToIdMapPair to determine which viewports
     * should be active. The resulting viewport IDs are stored in the session
     * for use by the rendering system.
     *
     * @see StateToIdMapPair
     * @see Session
     */
    class StateToViewportPolicyUpdateSystem : public helios::engine::ecs::System {

        /**
         * @brief Policy defining viewport-to-state mappings.
         */
        StateToIdMapPair<GameState, MatchState, ViewportId> stateToIdMapPair_;

    public:

        /**
         * @brief Constructs the system with a state-to-ID map pair.
         *
         * @param stateToIdMapPair Policy mapping game/match states to viewport IDs.
         */
        explicit StateToViewportPolicyUpdateSystem(StateToIdMapPair<GameState, MatchState, ViewportId> stateToIdMapPair)
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

            auto gameState  = session.gameState();
            auto matchState = session.matchState();

            session.clearViewportIds();

            auto viewportIds = stateToIdMapPair_.ids(gameState, matchState);
            session.setViewportIds(viewportIds);




        }

    };

}

