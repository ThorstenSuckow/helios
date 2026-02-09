/**
 * @file TransformClearSystem.ixx
 * @brief System for resetting dirty flags on TransformComponents.
 */
module;



export module helios.engine.modules.spatial.transform.systems.TransformClearSystem;

import helios.engine.runtime.world.GameWorld;
import helios.engine.ecs.System;
import helios.engine.runtime.world.UpdateContext;

import helios.engine.modules.scene.components.SceneNodeComponent;
import helios.engine.modules.spatial.transform.components.ComposeTransformComponent;

import helios.engine.modules.spatial.transform.components.ScaleStateComponent;

import helios.engine.mechanics.lifecycle.components.Active;

export namespace helios::engine::modules::spatial::transform::systems {

    /**
     * @brief System that clears the dirty state of TransformComponents and ScaleComponents
     * at the end of a frame.
     *
     * @details This system runs in the post-update phase. It iterates over all
     * TransformComponents and resets their dirty flag if it was set. This ensures that
     * changes are only processed once per frame by other systems (like SceneSyncSystem).
     */
    class TransformClearSystem : public helios::engine::ecs::System {


    public:

        /**
         * @brief Updates the system, clearing dirty flags of Transform- and ScaleStateComponent.
         *
         * @param updateContext The update context.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept override {

            for (auto [entity, tc, active] : gameWorld_->view<
                helios::engine::modules::spatial::transform::components::ComposeTransformComponent,
                helios::engine::mechanics::lifecycle::components::Active
                >().whereEnabled()) {
                tc->clearDirty();
            }

            for (auto [entity, sc, active] : gameWorld_->view<
                helios::engine::modules::spatial::transform::components::ScaleStateComponent,
                helios::engine::mechanics::lifecycle::components::Active
            >().whereEnabled()) {
                sc->clearDirty();
            }

        }

    };

}