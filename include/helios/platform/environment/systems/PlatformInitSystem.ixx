/**
 * @file PlatformInitSystem.ixx
 * @brief System that requests runtime platform initialization while session is not initialized.
 */
module;

export module helios.platform.environment.systems.PlatformInitSystem;



import helios.engine.runtime.world.UpdateContext;

import helios.engine.common.tags.SystemRole;

import helios.engine.runtime.world;
import helios.engine.runtime.messaging.command.NullCommandBuffer;
import helios.engine.common.concepts.IsCommandBufferLike;

import helios.platform.environment.commands;

import helios.ecs.components.Active;

using namespace helios::engine::common::tags;
using namespace helios::engine::runtime::world;
using namespace helios::engine::runtime::messaging::command;
using namespace helios::engine::common::concepts;
using namespace helios::platform::environment::commands;
export namespace helios::platform::environment::systems {

    /**
     * @brief Submits `PlatformInitCommand` until session/runtime initialization completed.
     *
     * @tparam TCommandBuffer Command buffer used for platform init command submission.
     */
    template<typename TCommandBuffer = NullCommandBuffer>
    requires IsCommandBufferLike<TCommandBuffer>
    class PlatformInitSystem {

        public:

        /**
         * @brief Engine role marker used by runtime system registries.
         */
        using EngineRoleTag = SystemRole;

        /**
         * @brief Queues platform initialization command when required.
         *
         * @param updateContext Frame-local update context.
         */
        void update(UpdateContext& updateContext) noexcept {

            if (!updateContext.session().isInitialized()) {
                 updateContext.queueCommand<TCommandBuffer, PlatformInitCommand>();
            }
        }

    };



}