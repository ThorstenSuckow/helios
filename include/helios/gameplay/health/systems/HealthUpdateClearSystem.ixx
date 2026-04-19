/**
 * @file HealthUpdateClearSystem.ixx
 * @brief System that clears the dirty flag on HealthComponents each frame.
 */
module;

#include <cassert>
#include <format>


export module helios.gameplay.health.systems.HealthUpdateClearSystem;

import helios.runtime.world.GameWorld;
import helios.runtime.world.UpdateContext;



import helios.ecs.components.Active;

import helios.gameplay.health.components;


import helios.runtime.world.tags.SystemRole;

export namespace helios::gameplay::health::systems {

    /**
     * @brief Clears the dirty flag on every active HealthComponent.
     *
     * Should run after all health-related observers have processed the
     * current frame so that stale change notifications are discarded.
     */
    template<typename THandle>
    class HealthUpdateClearSystem {

    public:

        using EngineRoleTag = helios::runtime::tags::SystemRole;
        /**
         * @brief Iterates all active HealthComponents and resets their dirty state.
         *
         * @param updateContext Current frame context.
         */
        void update(helios::runtime::world::UpdateContext& updateContext) noexcept {

            for (auto [entity, hc, active] : updateContext.view<
                THandle,
                helios::gameplay::health::components::HealthComponent<THandle>,
                helios::ecs::components::Active<THandle>
            >().whereEnabled()) {
                hc->clearDirty();
            }

        }

    };

}
