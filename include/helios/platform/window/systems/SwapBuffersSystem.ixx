/**
 * @file SwapBuffersSystem.ixx
 * @brief System that queues swap-buffer commands for shown and active windows.
 */
module;

#include <GLFW/glfw3.h>
#include <cassert>

export module helios.platform.window.systems.SwapBuffersSystem;



import helios.runtime.world.UpdateContext;

import helios.runtime.world.tags.SystemRole;

import helios.runtime.world;
import helios.runtime.messaging.command.NullCommandBuffer;
import helios.runtime.messaging.command.concepts.IsCommandBufferLike;

import helios.ecs.components.Active;

import helios.platform.window.commands;
import helios.platform.window.components;
import helios.platform.window.concepts.IsWindowHandle;


import helios.state.Bindings;
import helios.gameplay.gamestate.types;

using namespace helios::runtime::tags;
using namespace helios::runtime::world;
using namespace helios::runtime::messaging::command;
using namespace helios::runtime::messaging::command::concepts;
using namespace helios::platform::window::components;
using namespace helios::platform::window::concepts;
using namespace helios::platform::window::commands;
using namespace helios::ecs::components;
using namespace helios::gameplay::gamestate::types;
export namespace helios::platform::window::systems {

    /**
     * @brief Emits `SwapBuffersCommand` for each active and shown window.
     *
     * @tparam THandle Window handle type.
     */
    template<typename THandle, typename TCommandBuffer = NullCommandBuffer>
    requires IsWindowHandle<THandle> && IsCommandBufferLike<TCommandBuffer>
    class SwapBuffersSystem {

    public:

        using CommandBuffer_type = TCommandBuffer;

        /**
         * @brief Engine role marker used by runtime registries.
         */
        using EngineRoleTag = SystemRole;

        /**
         * @brief Enqueues swap-buffer commands for the current frame.
         *
         * @param updateContext Frame-local update context.
         */
        void update(UpdateContext& updateContext, TCommandBuffer& cmdBuffer) noexcept {

            for (auto [entity, wc, wsc, active]: updateContext.view<
                THandle,
                WindowComponent<THandle>, WindowShownComponent<THandle>, Active<THandle>
                >().whereEnabled()) {

                cmdBuffer.template add<SwapBuffersCommand<THandle>>(entity.handle());
            }

        }

    };



}