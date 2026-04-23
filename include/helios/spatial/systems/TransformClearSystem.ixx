/**
 * @file TransformClearSystem.ixx
 * @brief System for resetting dirty flags on TransformComponents.
 */
module;



export module helios.spatial.systems.TransformClearSystem;

import helios.runtime.world.GameWorld;

import helios.runtime.world.UpdateContext;

import helios.scene.components.SceneNodeComponent;
import helios.spatial.components.ComposeTransformComponent;

import helios.spatial.components.ScaleStateComponent;

import helios.ecs.components.Active;

import helios.runtime.world.tags.SystemRole;

export namespace helios::spatial::systems {

    /**
     * @brief System that clears the dirty state of TransformComponents and ScaleComponents
     * at the end of a frame.
     *
     * @details This system runs in the post-update phase. It iterates over all
     * TransformComponents and resets their dirty flag if it was set. This ensures that
     * changes are only processed once per frame by other systems (like SceneSyncSystem).
     */
    template<typename THandle>
    class TransformClearSystem {



    public:

        using EngineRoleTag = helios::runtime::world::tags::SystemRole;

        /**
         * @brief Updates the system, clearing dirty flags of Transform- and ScaleStateComponent.
         *
         * @param updateContext The update context.
         */
        void update(helios::runtime::world::UpdateContext& updateContext) noexcept {

            for (auto [entity, tc, active] : updateContext.view<
                THandle,
                helios::spatial::components::ComposeTransformComponent<THandle>,
                helios::ecs::components::Active<THandle>
                >().whereEnabled()) {
                tc->clearDirty();
            }

            for (auto [entity, sc, active] : updateContext.view<
                THandle,
                helios::spatial::components::ScaleStateComponent<THandle>,
                helios::ecs::components::Active<THandle>
            >().whereEnabled()) {
                sc->clearDirty();
            }

        }

    };

}