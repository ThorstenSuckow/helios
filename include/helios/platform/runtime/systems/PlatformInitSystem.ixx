/**
 * @file PlatformInitSystem.ixx
 * @brief System that requests runtime platform initialization while session is not initialized.
 */
module;

export module helios.platform.runtime.systems.PlatformInitSystem;



import helios.engine.runtime.world.UpdateContext;

import helios.engine.common.tags.SystemRole;

import helios.engine.runtime.world;
import helios.engine.runtime.messaging.command.EngineCommandBuffer;

import helios.platform.runtime.commands;

import helios.ecs.components.Active;

using namespace helios::engine::common::tags;
using namespace helios::engine::runtime::world;
using namespace helios::engine::runtime::messaging::command;
using namespace helios::platform::runtime::commands;
export namespace helios::platform::runtime::systems {

    /**
     * @brief Submits `PlatformInitCommand` until session/runtime initialization completed.
     */
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
                 updateContext.queueCommand<EngineCommandBuffer, PlatformInitCommand>();
            }
        }

    };



}