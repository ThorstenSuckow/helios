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

import helios.engine.ecs.System;

import helios.engine.mechanics.lifecycle.components.Active;

import helios.engine.mechanics.health.components;


export namespace helios::engine::mechanics::health::systems {

    /**
     * @brief Clears the dirty flag on every active HealthComponent.
     *
     * Should run after all health-related observers have processed the
     * current frame so that stale change notifications are discarded.
     */
    class HealthUpdateClearSystem : public helios::engine::ecs::System {

    public:

        /**
         * @brief Iterates all active HealthComponents and resets their dirty state.
         *
         * @param updateContext Current frame context.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept override {

            for (auto [entity, hc, active] : gameWorld_->view<
                helios::engine::mechanics::health::components::HealthComponent,
                helios::engine::mechanics::lifecycle::components::Active
            >().whereEnabled()) {
                hc->clearDirty();
            }

        }

    };

}
