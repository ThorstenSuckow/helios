/**
 * @file HealthUpdateClearSystem.ixx
 * @brief System that clears the dirty flag on HealthComponents each frame.
 */
module;

#include <cassert>
#include <format>


export module helios.engine.mechanics.health.systems.HealthUpdateClearSystem;

import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.world.UpdateContext;



import helios.ecs.components.Active;

import helios.engine.mechanics.health.components;


import helios.engine.common.tags.SystemRole;

export namespace helios::engine::mechanics::health::systems {

    /**
     * @brief Clears the dirty flag on every active HealthComponent.
     *
     * Should run after all health-related observers have processed the
     * current frame so that stale change notifications are discarded.
     */
    template<typename THandle>
    class HealthUpdateClearSystem {

    public:

        using EngineRoleTag = helios::engine::common::tags::SystemRole;
        /**
         * @brief Iterates all active HealthComponents and resets their dirty state.
         *
         * @param updateContext Current frame context.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept {

            for (auto [entity, hc, active] : updateContext.view<
                THandle,
                helios::engine::mechanics::health::components::HealthComponent<THandle>,
                helios::ecs::components::Active<THandle>
            >().whereEnabled()) {
                hc->clearDirty();
            }

        }

    };

}
