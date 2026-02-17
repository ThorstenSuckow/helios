/**
 * @file StateToViewportPolicyUpdateSystem.ixx
 * @brief System that updates active viewports based on game/match state.
 */
module;


#include <cassert>
#include <vector>

export module helios.engine.modules.rendering.viewport.systems.StateToViewportPolicyUpdateSystem;

import helios.engine.modules.rendering.viewport.types.StateToViewportPolicy;

import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.world.Session;

import helios.engine.ecs.System;
import helios.engine.runtime.world.UpdateContext;

import helios.engine.mechanics.gamestate.types;
import helios.engine.mechanics.match.types;

export namespace helios::engine::modules::rendering::viewport::systems {

    using namespace helios::engine::modules::rendering::viewport::types;
    using namespace helios::engine::mechanics::match::types;
    using namespace helios::engine::mechanics::gamestate::types;

    /**
     * @brief Updates the session's active viewport list based on state policy.
     *
     * @details This system queries the current GameState and MatchState from
     * the session, then uses the configured StateToViewportPolicy to determine
     * which viewports should be active. The resulting viewport IDs are stored
     * in the session for use by the rendering system.
     *
     * @see StateToViewportPolicy
     * @see Session
     */
    class StateToViewportPolicyUpdateSystem : public helios::engine::ecs::System {

        /**
         * @brief Policy defining viewport-to-state mappings.
         */
        StateToViewportPolicy stateToViewportPolicy_;

    public:

        /**
         * @brief Constructs the system with a state-to-viewport policy.
         *
         * @param stateToViewportPolicy Policy defining which viewports are
         *        active for each game/match state combination.
         */
        explicit StateToViewportPolicyUpdateSystem(StateToViewportPolicy stateToViewportPolicy)
            : stateToViewportPolicy_(std::move(stateToViewportPolicy)){}

        /**
         * @brief Updates the session's active viewport IDs based on current state.
         *
         * @param updateContext The current frame's update context.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept override {

            auto& session = updateContext.session();

            auto gameState  = session.gameState();
            auto matchState = session.matchState();

            session.clearViewportIds();

            auto viewportIds = stateToViewportPolicy_.viewportIds(gameState, matchState);
            session.addViewportIds(viewportIds);




        }

    };

}

