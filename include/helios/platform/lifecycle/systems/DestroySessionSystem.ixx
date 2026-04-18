/**
 * @file DestroySessionSystem.ixx
 * @brief System that transitions the current session into the destroyed state.
 */
module;

export module helios.platform.lifecycle.systems.DestroySessionSystem;


import helios.runtime.world.UpdateContext;
import helios.runtime.world.Session;
import helios.engine.common.tags.SystemRole;

using namespace helios::runtime::world;

export namespace helios::platform::lifecycle::systems {

    /**
     * @brief System that marks the active session as destroyed.
     *
     * This system is typically executed during shutdown handling to stop further
     * world processing in subsequent frames.
     */
    class DestroySessionSystem {

    public:

        /**
         * @brief Engine role marker used by runtime system registries.
         */
        using EngineRoleTag = helios::engine::common::tags::SystemRole;

        /**
         * @brief Destroys the active session in the current update context.
         *
         * @param updateContext Frame-local update context.
         */
        void update(helios::runtime::world::UpdateContext& updateContext) noexcept {
            updateContext.session().destroy();
        }
    };

}