/**
 * @file DelayedComponentEnablerSystem.ixx
 * @brief System that processes delayed component activations.
 */
module;


#include <vector>

export module helios.gameplay.lifecycle.systems.DelayedComponentEnablerSystem;


import helios.runtime.world.GameWorld;
import helios.runtime.world.UpdateContext;
import helios.gameplay.lifecycle.components.DelayedComponentEnabler;
import helios.ecs.types.ComponentTypeId;

import helios.ecs.ComponentOpsRegistry;

import helios.ecs.components.Active;

import helios.engine.common.tags.SystemRole;

export namespace helios::gameplay::lifecycle::systems {

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
    template<typename THandle>
    class DelayedComponentEnablerSystem {

        /**
         * @brief Temporary buffer for components that completed their delay.
         */
        std::vector<helios::ecs::types::ComponentTypeId<THandle>> sync_;


    public:

        using EngineRoleTag = helios::engine::common::tags::SystemRole;
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
        void update(helios::runtime::world::UpdateContext& updateContext) noexcept {

            const float delta = updateContext.deltaTime();

            for (auto [entity, dce, active] : updateContext.view<
                THandle,
                helios::gameplay::lifecycle::components::DelayedComponentEnabler<THandle>,
                helios::ecs::components::Active<THandle>
            >().whereEnabled()) {

                sync_.clear();

                auto deferredComponents = dce->deferredComponents();

                for (auto& deferredComponent : deferredComponents) {

                    deferredComponent.delta = std::max(deferredComponent.delta - delta, 0.0f);

                    const auto componentTypeId = deferredComponent.componentTypeId;

                    if (deferredComponent.delta <= 0) {
                        auto* rawCmp = entity.raw(componentTypeId);
                        auto ops = helios::ecs::ComponentOpsRegistry<THandle>::ops(componentTypeId);

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