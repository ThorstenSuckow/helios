/**
 * @file DelayedComponentEnablerSystem.ixx
 * @brief System that processes delayed component activations.
 */
module;


#include <vector>

export module helios.engine.mechanics.lifecycle.systems.DelayedComponentEnablerSystem;

import helios.engine.ecs.System;
import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.world.UpdateContext;
import helios.engine.mechanics.lifecycle.components.DelayedComponentEnabler;
import helios.engine.core.data.ComponentTypeId;

import helios.engine.ecs.ComponentOpsRegistry;

import helios.engine.mechanics.lifecycle.components.Active;

export namespace helios::engine::mechanics::lifecycle::systems {

    /**
     * @brief System that processes delayed component activations each frame.
     *
     * @details DelayedComponentEnablerSystem iterates over all entities with a
     * DelayedComponentEnabler component and decrements the delay timers. When a
     * timer reaches zero, the corresponding component is enabled.
     *
     * This system enables gameplay patterns such as:
     * - **Spawn immunity:** Projectiles/enemies ignore collisions briefly
     * - **Staggered activation:** Wave spawns activate progressively
     * - **Effect sequencing:** Components enable in a defined order
     *
     * @note Should be scheduled in the gameplay phase to ensure proper timing.
     *
     * @see DelayedComponentEnabler
     * @see DelayedComponentEnablerInitializer
     */
    class DelayedComponentEnablerSystem : public helios::engine::ecs::System {

        /**
         * @brief Temporary buffer for components that completed their delay.
         */
        std::vector<helios::engine::core::data::ComponentTypeId> sync_;

    public:

        /**
         * @brief Processes all deferred components and activates expired ones.
         *
         * @details For each entity with DelayedComponentEnabler:
         * 1. Decrements all delay timers by deltaTime
         * 2. Enables components whose timers reach zero
         * 3. Removes activated components from the deferred list
         *
         * @param updateContext Provides deltaTime for timer updates.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept override {

            const float delta = updateContext.deltaTime();

            for (auto [entity, dce, active] : gameWorld_->view<
                helios::engine::mechanics::lifecycle::components::DelayedComponentEnabler,
                helios::engine::mechanics::lifecycle::components::Active
            >().whereEnabled()) {

                sync_.clear();

                auto deferredComponents = dce->deferredComponents();

                for (auto& deferredComponent : deferredComponents) {

                    deferredComponent.delta = std::max(deferredComponent.delta - delta, 0.0f);

                    const auto componentTypeId = deferredComponent.componentTypeId;

                    if (deferredComponent.delta <= 0) {
                        auto* rawCmp = entity.raw(componentTypeId);
                        auto ops = helios::engine::ecs::ComponentOpsRegistry::ops(componentTypeId);

                        if (rawCmp && ops.enable) {
                            ops.enable(rawCmp);
                        }

                        sync_.push_back(componentTypeId);
                    }
                }

                dce->sync(sync_);
            }
        }
    };

}