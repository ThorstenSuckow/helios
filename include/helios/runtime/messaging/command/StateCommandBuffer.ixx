/**
 * @file EngineCommandBuffer.ixx
 * @brief Concrete command buffer pre-configured with all engine command types.
 */
module;

export module helios.runtime.messaging.command.StateCommandBuffer;

import helios.gameplay.gamestate.types;
import helios.gameplay.match.types;

import helios.engine.state.Bindings;

import helios.engine.state.commands;


import helios.platform.environment.types;
import helios.platform.environment.commands;

import helios.runtime.messaging.command.TypedCommandBuffer;


import helios.runtime.world.types;
import helios.engine.common.tags.CommandBufferRole;

static_assert(requires {
  typename helios::engine::state::types::StateTransitionId<
    helios::gameplay::match::types::MatchState
  >::Type;
}, "Bindings not visible in StateCommandBuffer TU");

static_assert(requires {
  typename helios::engine::state::types::StateTransitionId<
    helios::gameplay::gamestate::types::GameState
  >::Type;
}, "Bindings not visible in StateCommandBuffer TU");


using namespace helios::platform::environment::types;
using namespace helios::runtime::world;
using namespace helios::runtime::world::types;
export namespace helios::runtime::messaging::command {

    using StateCommandBuffer = helios::runtime::messaging::command::TypedCommandBuffer<
        helios::engine::state::commands::StateCommand<helios::gameplay::gamestate::types::GameState>,
        helios::engine::state::commands::DelayedStateCommand<helios::gameplay::gamestate::types::GameState>,
        helios::engine::state::commands::StateCommand<helios::gameplay::match::types::MatchState>,
        helios::engine::state::commands::DelayedStateCommand<helios::gameplay::match::types::MatchState>
    >;

}

