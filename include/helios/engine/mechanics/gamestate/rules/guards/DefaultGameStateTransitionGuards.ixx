/**
 * @file DefaultGameStateTransitionGuards.ixx
 * @brief Default guard functions for game state transitions.
 */
module;

#include <cassert>

export module helios.engine.mechanics.gamestate.rules.guards.DefaultGameStateTransitionGuards;

import helios.runtime.world.UpdateContext;
import helios.runtime.world.Session;
import helios.runtime.world.GameWorld;

import helios.engine.state.Bindings;

import helios.engine.mechanics.gamestate.types;
import helios.engine.state.types;

import helios.rendering.shader.components;

using namespace helios::engine::mechanics::gamestate::types;
using namespace helios::engine::state::types;
using namespace helios::rendering::shader::components;
export namespace helios::engine::mechanics::gamestate::rules::guards {


    class DefaultGameStateTransitionGuards {

    public:

        /**
         * @brief Guard that checks if a current context component is available.
         *
         * @param updateContext The current frame's update context.
         * @param transitionRequest The requested state transition.
         *
         * @return True if an entity with a CurrentContext exists.
         */
        static bool isPlatformInitialized(
            helios::runtime::world::UpdateContext& updateContext,
            const StateTransitionRequest<GameState> transitionRequest
        ) {
            return updateContext.runtimeEnvironment().isInitialized();
        }

        /**
         * @brief Guard that checks if the runtime infrastructure is available.
         *
         * @param updateContext The current frame's update context.
         * @param transitionRequest The requested state transition.
         *
         * @return True if the infrastructure can be considered in a ready-state.
         */
        static bool isRuntimeInfrastructureReady(
            helios::runtime::world::UpdateContext& updateContext,
            const StateTransitionRequest<GameState> transitionRequest
        ) {
            return updateContext.runtimeEnvironment().isRuntimeInfrastructureReady();
        }


    };


}
