/**
 * @file UiTextBoundsUpdateSystem.ixx
 * @brief System for updating UI text element bounds.
 */
module;



export module helios.engine.modules.ui.widgets.systems.UiTextBoundsUpdateSystem;

import helios.engine.modules.ui.widgets.components.UiTextComponent;

import helios.runtime.world.GameWorld;
import helios.runtime.world.UpdateContext;

import helios.ecs.components.Active;

import helios.engine.modules.rendering.model.components.ModelAabbComponent;

import helios.engine.common.tags.SystemRole;

export namespace helios::engine::modules::ui::widgets::systems {


    /**
     * @brief System for updating UI text element bounds.
     *
     * Monitors UiTextComponent for resize requests and updates the associated
     * ModelAabbComponent with the new text bounds from the underlying TextRenderable.
     */
    template <typename THandle>
    class UiTextBoundsUpdateSystem {

    public:


        using EngineRoleTag = helios::engine::common::tags::SystemRole;


        /**
         * @brief Updates AABB bounds for text components that need resizing.
         *
         * @param updateContext The current frame's update context.
         */
        void update(helios::runtime::world::UpdateContext& updateContext) noexcept {

            for (auto [entity, txt, mac, active] : updateContext.view<
                THandle,
                helios::engine::modules::ui::widgets::components::UiTextComponent<THandle>,
                helios::engine::modules::rendering::model::components::ModelAabbComponent<THandle>,
                helios::ecs::components::Active<THandle>
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
