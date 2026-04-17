/**
 * @file PollEventsSystem.ixx
 * @brief System that queues per-frame platform event polling commands.
 */
module;

export module helios.platform.runtime.systems.PollEventsSystem;



import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.messaging.command.EngineCommandBuffer;
import helios.engine.common.tags.SystemRole;

import helios.platform.runtime.commands.PollEventsCommand;

using namespace helios::engine::common::tags;
using namespace helios::engine::runtime::world;
using namespace helios::engine::runtime::messaging::command;
using namespace helios::platform::runtime::commands;
export namespace helios::platform::runtime::systems {

    /**
     * @brief Queues `PollEventsCommand` once per update call.
     */
    class PollEventsSystem {

        public:

        /**
         * @brief Engine role marker used by runtime system registries.
         */
        using EngineRoleTag = SystemRole;

        /**
         * @brief Enqueues polling of native platform/window events.
         *
         * @param updateContext Frame-local update context.
         */
        void update(UpdateContext& updateContext) noexcept {
            updateContext.queueCommand<EngineCommandBuffer, PollEventsCommand>();
        }

    };



}