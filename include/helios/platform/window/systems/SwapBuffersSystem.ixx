/**
 * @file SwapBuffersSystem.ixx
 * @brief System that queues swap-buffer commands for shown and active windows.
 */
module;

#include <GLFW/glfw3.h>
#include <cassert>

export module helios.platform.window.systems.SwapBuffersSystem;



import helios.engine.runtime.world.UpdateContext;

import helios.engine.common.tags.SystemRole;

import helios.engine.runtime.world;
import helios.engine.runtime.messaging.command.EngineCommandBuffer;

import helios.ecs.components.Active;

import helios.platform.window.commands;
import helios.platform.window.components;
import helios.platform.window.concepts.IsWindowHandle;


import helios.engine.state.Bindings;
import helios.engine.mechanics.gamestate.types;

using namespace helios::engine::common::tags;
using namespace helios::engine::runtime::world;
using namespace helios::engine::runtime::messaging::command;
using namespace helios::platform::window::components;
using namespace helios::platform::window::concepts;
using namespace helios::platform::window::commands;
using namespace helios::ecs::components;
using namespace helios::engine::mechanics::gamestate::types;
export namespace helios::platform::window::systems {

    /**
     * @brief Emits `SwapBuffersCommand` for each active and shown window.
     *
     * @tparam THandle Window handle type.
     */
    template<typename THandle>
    requires IsWindowHandle<THandle>
    class SwapBuffersSystem {

    public:

        /**
         * @brief Engine role marker used by runtime registries.
         */
        using EngineRoleTag = SystemRole;

        /**
         * @brief Enqueues swap-buffer commands for the current frame.
         *
         * @param updateContext Frame-local update context.
         */
        void update(UpdateContext& updateContext) noexcept {

            for (auto [entity, wc, wsc, active]: updateContext.view<
                THandle,
                WindowComponent<THandle>, WindowShownComponent<THandle>, Active<THandle>
                >().whereEnabled()) {

                updateContext.queueCommand<EngineCommandBuffer, SwapBuffersCommand<THandle>>(entity.handle());
            }

        }

    };



}