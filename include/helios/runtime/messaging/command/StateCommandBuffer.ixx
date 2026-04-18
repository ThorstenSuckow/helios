/**
 * @file EngineCommandBuffer.ixx
 * @brief Concrete command buffer pre-configured with all engine command types.
 */
module;

export module helios.runtime.messaging.command.StateCommandBuffer;

import helios.gameplay.gamestate.types;
import helios.gameplay.matchstate.types;

import helios.state.Bindings;

import helios.state.commands;


import helios.platform.environment.types;
import helios.platform.environment.commands;

import helios.runtime.messaging.command.TypedCommandBuffer;


import helios.runtime.world.types;
import helios.runtime.messaging.command.tags.CommandBufferRole;

static_assert(requires {
  typename helios::state::types::StateTransitionId<
    helios::gameplay::matchstate::types::MatchState
  >::Type;
}, "Bindings not visible in StateCommandBuffer TU");

static_assert(requires {
  typename helios::state::types::StateTransitionId<
    helios::gameplay::gamestate::types::GameState
  >::Type;
}, "Bindings not visible in StateCommandBuffer TU");


using namespace helios::platform::environment::types;
using namespace helios::runtime::world;
using namespace helios::runtime::world::types;
export namespace helios::runtime::messaging::command {

    using StateCommandBuffer = helios::runtime::messaging::command::TypedCommandBuffer<
        helios::state::commands::StateCommand<helios::gameplay::gamestate::types::GameState>,
        helios::state::commands::DelayedStateCommand<helios::gameplay::gamestate::types::GameState>,
        helios::state::commands::StateCommand<helios::gameplay::matchstate::types::MatchState>,
        helios::state::commands::DelayedStateCommand<helios::gameplay::matchstate::types::MatchState>
    >;

}

