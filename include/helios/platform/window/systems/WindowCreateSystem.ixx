/**
 * @file WindowCreateSystem.ixx
 * @brief System that translates window-create request components into window-create commands.
 */
module;

export module helios.platform.window.systems.WindowCreateSystem;



import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.messaging.command.EngineCommandBuffer;

import helios.engine.common.tags.SystemRole;

import helios.platform.window.components.WindowCreateRequestComponent;
import helios.platform.window.commands.WindowCreateCommand;

import helios.ecs.components.Active;

using namespace helios::engine::common::tags;
using namespace helios::engine::runtime::world;
using namespace helios::engine::runtime::messaging::command;
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
    template<typename THandle>
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

                updateContext.queueCommand<EngineCommandBuffer, WindowCreateCommand<THandle>>(
                    entity.handle(),
                    win->windowConfig
                );
            }
        }

    };



}