/**
 * @file PollEventsSystem.ixx
 * @brief System that queues per-frame platform event polling commands.
 */
module;

export module helios.platform.environment.systems.PollEventsSystem;



import helios.runtime.world.UpdateContext;
import helios.runtime.messaging.command.NullCommandBuffer;
import helios.runtime.messaging.command.concepts.IsCommandBufferLike;
import helios.runtime.world.tags.SystemRole;

import helios.platform.environment.commands.PollEventsCommand;

using namespace helios::runtime::tags;
using namespace helios::runtime::world;
using namespace helios::runtime::messaging::command;
using namespace helios::runtime::messaging::command::concepts;
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