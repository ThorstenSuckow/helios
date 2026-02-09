/**
 * @file UiTextBoundsUpdateSystem.ixx
 * @brief System for updating UI text element bounds.
 */
module;



export module helios.engine.modules.ui.widgets.systems.UiTextBoundsUpdateSystem;
import helios.engine.ecs.System;
import helios.engine.modules.ui.widgets.components.UiTextComponent;

import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.world.UpdateContext;

import helios.engine.mechanics.lifecycle.components.Active;

import helios.engine.modules.rendering.model.components.ModelAabbComponent;

export namespace helios::engine::modules::ui::widgets::systems {


    /**
     * @brief System for updating UI text element bounds.
     *
     * Monitors UiTextComponent for resize requests and updates the associated
     * ModelAabbComponent with the new text bounds from the underlying TextRenderable.
     */
    class UiTextBoundsUpdateSystem : public helios::engine::ecs::System {


    public:

        /**
         * @brief Updates AABB bounds for text components that need resizing.
         *
         * @param updateContext The current frame's update context.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept override {

            for (auto [entity, txt, mac, active] : gameWorld_->view<
                helios::engine::modules::ui::widgets::components::UiTextComponent,
                helios::engine::modules::rendering::model::components::ModelAabbComponent,
                helios::engine::mechanics::lifecycle::components::Active
            >().whereEnabled()) {

                if (txt->needsResize()) {
                    auto aabb = txt->renderable()->localAABB();
                    mac->setAabb(aabb);
                    txt->resizeComplete();
                }
            }
        }

    };

}
