/**
 * @file WarmupDoneSystem.ixx
 * @brief System that emits a game-state warmup-done transition when no active shader sources remain.
 */
module;

#include <concepts>

export module helios.platform.lifecycle.systems.WarmupDoneSystem;


import helios.runtime.world.UpdateContext;

import helios.engine.common.tags.SystemRole;

import helios.runtime.messaging.command;

import helios.engine.common.concepts;

import helios.runtime.world;
import helios.ecs.components.Active;
import helios.rendering.shader.concepts;

import helios.rendering.shader.components;

import helios.engine.state.commands;
import helios.engine.state.types;
import helios.gameplay.gamestate.types;

using namespace helios::rendering::shader::concepts;
using namespace helios::engine::common::concepts;
using namespace helios::ecs::components;
using namespace helios::rendering::shader::components;
using namespace helios::engine::common::tags;
using namespace helios::runtime::world;
using namespace helios::runtime::messaging::command;
using namespace helios::engine::state::types;
using namespace helios::engine::state::commands;
using namespace helios::gameplay::gamestate::types;
export namespace helios::platform::lifecycle::systems {

    /**
     * @brief Signals warmup completion through a typed state command buffer.
     *
     * @tparam THandle Shader handle type.
     * @tparam TCommandBuffer Command buffer type used to queue state commands.
     */
    template<typename THandle, typename TCommandBuffer = NullCommandBuffer>
    requires IsShaderHandle<THandle> &&
             IsCommandBufferLike<TCommandBuffer> &&
             (!std::is_same_v<TCommandBuffer, NullCommandBuffer>)
    class WarmupDoneSystem {

    public:

        /**
         * @brief Engine role marker used by runtime registries.
         */
        using EngineRoleTag = SystemRole;

        /**
         * @brief Queues `StateCommand<GameState>` with `WarmupDoneSignal` when warmup resources are consumed.
         *
         * @param updateContext Frame-local update context.
         */
        void update(UpdateContext& updateContext) noexcept {

            if (updateContext.view<
                THandle,
                ShaderSourceComponent<THandle>,
                Active<THandle>
                >().whereEnabled().empty()) {

                updateContext.queueCommand<
                    TCommandBuffer,
                    StateCommand<GameState>>(
                    StateTransitionRequest<GameState>(
                        updateContext.session().state<GameState>(),
                        GameStateTransitionId::WarmupDoneSignal
                    )
                );

            }


        }

    };



}