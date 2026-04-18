/**
 * @file WindowCreateSystem.ixx
 * @brief System that translates window-create request components into window-create commands.
 */
module;

export module helios.platform.window.systems.WindowCreateSystem;



import helios.runtime.world.UpdateContext;
import helios.runtime.messaging.command.NullCommandBuffer;
import helios.engine.common.concepts.IsCommandBufferLike;

import helios.engine.common.tags.SystemRole;

import helios.platform.window.components.WindowCreateRequestComponent;
import helios.platform.window.commands.WindowCreateCommand;

import helios.ecs.components.Active;
import helios.platform.window.concepts.IsWindowHandle;

using namespace helios::platform::window::concepts;
using namespace helios::engine::common::tags;
using namespace helios::runtime::world;
using namespace helios::runtime::messaging::command;
using namespace helios::engine::common::concepts;
using namespace helios::platform::window::components;
using namespace helios::platform::window::commands;
using namespace helios::ecs::components;
using namespace helios::platform::window::components;
export namespace helios::platform::window::systems {

    /**
     * @brief Queues `WindowCreateCommand` for active entities with pending create requests.
     *
     * @tparam THandle Window-domain entity handle type.
     */
    template<typename THandle, typename TCommandBuffer = NullCommandBuffer>
    requires IsWindowHandle<THandle> && IsCommandBufferLike<TCommandBuffer>
    class WindowCreateSystem {

        public:

        /**
         * @brief Engine role marker used by runtime registries.
         */
        using EngineRoleTag = SystemRole;

        /**
         * @brief Scans create requests and submits create commands.
         *
         * @param updateContext Frame-local update context.
         */
        void update(UpdateContext& updateContext) noexcept {

            for (auto [entity, win, active]: updateContext.view<
                THandle,
                WindowCreateRequestComponent<THandle>, Active<THandle>
                >().whereEnabled()) {

                updateContext.queueCommand<TCommandBuffer, WindowCreateCommand<THandle>>(
                    entity.handle(),
                    win->windowConfig
                );
            }
        }

    };



}