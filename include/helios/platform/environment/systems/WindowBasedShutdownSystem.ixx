/**
 * @file WindowBasedShutdownSystem.ixx
 * @brief System that requests runtime shutdown when no active windows remain.
 */
module;

export module helios.platform.environment.systems.WindowBasedShutdownSystem;



import helios.engine.runtime.world.UpdateContext;

import helios.engine.common.tags.SystemRole;

import helios.engine.runtime.world;
import helios.engine.runtime.messaging.command.NullCommandBuffer;
import helios.engine.common.concepts.IsCommandBufferLike;

import helios.ecs.components.Active;

import helios.platform.window.components;
import helios.platform.window.concepts;

import helios.platform.environment.commands;
import helios.platform.environment.components;
import helios.platform.environment.concepts;

import helios.engine.state.Bindings;
import helios.engine.mechanics.gamestate.types;

using namespace helios::engine::common::tags;
using namespace helios::engine::runtime::world;
using namespace helios::engine::runtime::messaging::command;
using namespace helios::engine::common::concepts;
using namespace helios::platform::environment::components;
using namespace helios::platform::window::components;
using namespace helios::platform::window::concepts;
using namespace helios::platform::environment::concepts;
using namespace helios::platform::environment::commands;
using namespace helios::ecs::components;
using namespace helios::engine::mechanics::gamestate::types;
export namespace helios::platform::environment::systems {

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