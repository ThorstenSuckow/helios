/**
 * @file EngineCommandBuffer.ixx
 * @brief Concrete command buffer pre-configured with all engine command types.
 */
module;

export module helios.engine.runtime.messaging.command.StateCommandBuffer;

import helios.engine.mechanics.gamestate.types;
import helios.engine.mechanics.match.types;

import helios.engine.state.Bindings;

import helios.engine.state.commands;


import helios.platform.environment.types;
import helios.platform.environment.commands;

import helios.engine.runtime.messaging.command.TypedCommandBuffer;


import helios.engine.runtime.world.types;
import helios.engine.common.tags.CommandBufferRole;

static_assert(requires {
  typename helios::engine::state::types::StateTransitionId<
    helios::engine::mechanics::match::types::MatchState
  >::Type;
}, "Bindings not visible in StateCommandBuffer TU");

static_assert(requires {
  typename helios::engine::state::types::StateTransitionId<
    helios::engine::mechanics::gamestate::types::GameState
  >::Type;
}, "Bindings not visible in StateCommandBuffer TU");


using namespace helios::platform::environment::types;
using namespace helios::engine::runtime::world;
using namespace helios::engine::runtime::world::types;
export namespace helios::engine::runtime::messaging::command {

    using StateCommandBuffer = helios::engine::runtime::messaging::command::TypedCommandBuffer<
        helios::engine::state::commands::StateCommand<helios::engine::mechanics::gamestate::types::GameState>,
        helios::engine::state::commands::DelayedStateCommand<helios::engine::mechanics::gamestate::types::GameState>,
        helios::engine::state::commands::StateCommand<helios::engine::mechanics::match::types::MatchState>,
        helios::engine::state::commands::DelayedStateCommand<helios::engine::mechanics::match::types::MatchState>
    >;

}

