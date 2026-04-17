/**
 * @file ShutdownSystem.ixx
 * @brief System that requests runtime shutdown when no active windows remain.
 */
module;

export module helios.platform.runtime.systems.ShutdownSystem;



import helios.engine.runtime.world.UpdateContext;

import helios.engine.common.tags.SystemRole;

import helios.engine.runtime.world;
import helios.engine.runtime.messaging.command.EngineCommandBuffer;

import helios.ecs.components.Active;

import helios.platform.window.components;
import helios.platform.window.concepts;

import helios.platform.runtime.commands;
import helios.platform.runtime.components;
import helios.platform.runtime.concepts;

import helios.engine.state.Bindings;
import helios.engine.mechanics.gamestate.types;

using namespace helios::engine::common::tags;
using namespace helios::engine::runtime::world;
using namespace helios::engine::runtime::messaging::command;
using namespace helios::platform::runtime::components;
using namespace helios::platform::window::components;
using namespace helios::platform::window::concepts;
using namespace helios::platform::runtime::concepts;
using namespace helios::platform::runtime::commands;
using namespace helios::ecs::components;
using namespace helios::engine::mechanics::gamestate::types;
export namespace helios::platform::runtime::systems {

    /**
     * @brief Queues `ShutdownCommand` when no active window entities are left.
     *
     * @tparam THandle Window handle type.
     */
    template<typename THandle>
    requires IsWindowHandle<THandle>
    class ShutdownSystem {

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
               updateContext.queueCommand<EngineCommandBuffer, ShutdownCommand>();
            }


        }

    };



}