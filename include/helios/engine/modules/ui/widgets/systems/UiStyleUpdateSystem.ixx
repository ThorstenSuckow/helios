/**
 * @file UiStyleUpdateSystem.ixx
 * @brief System for updating UI text element bounds.
 */
module;



export module helios.engine.modules.ui.widgets.systems.UiStyleUpdateSystem;
import helios.engine.ecs.System;

import helios.engine.modules.ui.widgets.components.UiTextComponent;
import helios.engine.modules.ui.widgets.components.UiStyleComponent;
import helios.engine.modules.ui.widgets.components.UiStateComponent;

import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.world.UpdateContext;

import helios.rendering.text.TextShaderPropertiesOverride;

import helios.engine.mechanics.lifecycle.components.Active;


export namespace helios::engine::modules::ui::widgets::systems {


    /**
     * @brief System for updating UI text element bounds.
     *
     * Monitors UiTextComponent for resize requests and updates the associated
     * ModelAabbComponent with the new text bounds from the underlying TextRenderable.
     */
    class UiStyleUpdateSystem : public helios::engine::ecs::System {


    public:

        /**
         * @brief Updates AABB bounds for text components that need resizing.
         *
         * @param updateContext The current frame's update context.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept override {

            for (auto [entity, txt, usy, usc, active] : gameWorld_->view<
                helios::engine::modules::ui::widgets::components::UiTextComponent,
                helios::engine::modules::ui::widgets::components::UiStyleComponent,
                helios::engine::modules::ui::widgets::components::UiStateComponent,
                helios::engine::mechanics::lifecycle::components::Active
            >().whereEnabled()) {


                auto* renderable = txt->renderable();
                auto& mesh = renderable->textMesh();

                auto properties = renderable->textShaderPropertiesOverride();

                if (!properties) {
                    properties = helios::rendering::text::TextShaderPropertiesOverride();
                    properties->baseColor = usy->normalColor();
                } else {
                    properties = properties->withBaseColor(usy->normalColor());
                }

                mesh.setFontScale(usy->normalScale());

                if (usc->isSelected()) {
                    properties = properties->withBaseColor(usy->selectedColor());
                    mesh.setFontScale(usy->selectedScale());
                }

                renderable->setTextShaderPropertiesOverride(properties.value());

            }
        }

    };

}
