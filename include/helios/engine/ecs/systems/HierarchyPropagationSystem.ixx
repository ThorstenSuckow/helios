/**
 * @file HierarchyPropagationSystem.ixx
 * @brief System for propagating activation state through entity hierarchies.
 */
module;

#include <cassert>
#include <utility>
#include <vector>

export module helios.engine.ecs.systems.HierarchyPropagationSystem;


import helios.engine.ecs.EntityHandle;
import helios.engine.ecs.GameObject;

import helios.engine.ecs.components.HierarchyComponent;

import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.world.GameWorld;


import helios.engine.mechanics.lifecycle.components.Active;
import helios.engine.mechanics.lifecycle.components.Inactive;



import helios.engine.common.tags.SystemRole;

export namespace helios::engine::ecs::systems {


    using namespace helios::engine::ecs;

    /**
     * @brief Propagates activation state from parent to child entities.
     *
     * @details This system processes entities with HierarchyComponent and
     * propagates their active/inactive state to all descendants. Only root
     * entities (those without a parent) that are marked dirty trigger
     * propagation.
     *
     * The system runs in two passes:
     * - Active roots: propagate active state to children
     * - Inactive roots: propagate inactive state to children
     */
    class HierarchyPropagationSystem {

        /**
         * @brief Recursively updates activation state for an entity and its children.
         *
         * @param go The entity to update.
         * @param active The activation state to apply.
         */
        void updateEntityHierarchy(GameObject go, helios::engine::runtime::world::UpdateContext& updateContext, bool active) {

            auto* hc = go.get<helios::engine::ecs::components::HierarchyComponent>();
            if (!hc) {
                return;
            }
            hc->clearDirty();
            for (auto child : hc->children()) {
                if (auto ent = updateContext.find(child)) {
                    ent->setActive(active);
                    updateEntityHierarchy(ent.value(), updateContext, active);
                }
            }
        }

        /**
         * @brief Temporary storage for root entities pending propagation.
         */
        std::vector<GameObject> roots_;


    public:

        using EngineRoleTag = helios::engine::common::tags::SystemRole;
        /**
         * @brief Processes hierarchy propagation for dirty root entities.
         *
         * @param updateContext The current frame's update context.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept {

            for (auto [entity, hc, active] : updateContext.view<
                helios::engine::ecs::components::HierarchyComponent,
                helios::engine::mechanics::lifecycle::components::Active
            >().whereEnabled()) {
                if (hc->parent() || !hc->isDirty()) {
                    continue;
                }
                roots_.push_back(entity);
            }

            for (auto& entity : roots_) {
                updateEntityHierarchy(entity, updateContext, true);
            }
            roots_.clear();

            for (auto [entity, hc, active] : updateContext.view<
                helios::engine::ecs::components::HierarchyComponent,
                helios::engine::mechanics::lifecycle::components::Inactive
            >().whereEnabled()) {
                if (hc->parent() || !hc->isDirty()) {
                    continue;
                }
                roots_.push_back(entity);
            }

            for (auto& entity : roots_) {
                updateEntityHierarchy(entity, updateContext, false);
            }
            roots_.clear();
        }
    };
}


