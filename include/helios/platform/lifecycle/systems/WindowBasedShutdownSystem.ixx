/**
 * @file WindowBasedShutdownSystem.ixx
 * @brief System that requests runtime shutdown when no active windows remain.
 */
module;

export module helios.platform.lifecycle.systems.WindowBasedShutdownSystem;



import helios.runtime.world.UpdateContext;

import helios.runtime.world.tags.SystemRole;

import helios.runtime.world;
import helios.runtime.messaging.command.NullCommandBuffer;
import helios.runtime.messaging.command.concepts.IsCommandBufferLike;

import helios.ecs.components.Active;

import helios.platform.window.components;
import helios.platform.window.concepts;

import helios.platform.lifecycle.commands;
import helios.platform.environment.components;
import helios.platform.environment.concepts;

import helios.state.Bindings;
import helios.gameplay.gamestate.types;

using namespace helios::runtime::tags;
using namespace helios::runtime::world;
using namespace helios::runtime::messaging::command;
using namespace helios::runtime::messaging::command::concepts;
using namespace helios::platform::environment::components;
using namespace helios::platform::window::components;
using namespace helios::platform::window::concepts;
using namespace helios::platform::environment::concepts;
using namespace helios::platform::lifecycle::commands;
using namespace helios::ecs::components;
using namespace helios::gameplay::gamestate::types;
export namespace helios::platform::lifecycle::systems {

    /**
     * @brief Queues `ShutdownCommand` when no active window entities are left.
     *
     * @tparam THandle Window handle type.
     */
    template<typename THandle, typename TCommandBuffer = NullCommandBuffer>
    requires IsWindowHandle<THandle> && IsCommandBufferLike<TCommandBuffer>
    class WindowBasedShutdownSystem {

    public:

        /**
         * @brief Engine role marker used by runtime registries.
         */
        using EngineRoleTag = SystemRole;

        /**
         * @brief Checks window activity and queues shutdown when the set is empty.
         *
         * @param updateContext Frame-local update context.
         */
        void update(UpdateContext& updateContext) noexcept {

            if (updateContext.view<THandle, WindowComponent<THandle>, Active<THandle>>().whereEnabled().empty()) {
               updateContext.queueCommand<TCommandBuffer, ShutdownCommand>();
            }


        }

    };



}