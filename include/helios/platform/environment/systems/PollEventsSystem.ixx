/**
 * @file PollEventsSystem.ixx
 * @brief System that queues per-frame platform event polling commands.
 */
module;

export module helios.platform.environment.systems.PollEventsSystem;



import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.messaging.command.NullCommandBuffer;
import helios.engine.common.concepts.IsCommandBufferLike;
import helios.engine.common.tags.SystemRole;

import helios.platform.environment.commands.PollEventsCommand;

using namespace helios::engine::common::tags;
using namespace helios::engine::runtime::world;
using namespace helios::engine::runtime::messaging::command;
using namespace helios::engine::common::concepts;
using namespace helios::platform::environment::commands;
export namespace helios::platform::environment::systems {

    /**
     * @brief Queues `PollEventsCommand` once per update call.
     */
    template<typename TCommandBuffer = NullCommandBuffer>
    requires IsCommandBufferLike<TCommandBuffer>
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
            updateContext.queueCommand<TCommandBuffer, PollEventsCommand>();
        }

    };



}