/**
 * @file UiStyleUpdateSystem.ixx
 * @brief System for updating UI text element bounds.
 */
module;



export module helios.ui.widgets.systems.UiStyleUpdateSystem;


import helios.ui.widgets.components.UiTextComponent;
import helios.ui.widgets.components.UiStyleComponent;
import helios.ui.widgets.components.UiStateComponent;

import helios.runtime.world.GameWorld;
import helios.runtime.world.UpdateContext;

import helios.rendering.text.TextShaderPropertiesOverride;

import helios.ecs.components.Active;


import helios.runtime.world.tags.SystemRole;

export namespace helios::ui::widgets::systems {


    /**
     * @brief System for updating UI text element bounds.
     *
     * Monitors UiTextComponent for resize requests and updates the associated
     * ModelAabbComponent with the new text bounds from the underlying TextRenderable.
     */
    template<typename THandle>
    class UiStyleUpdateSystem {

    public:


        using EngineRoleTag = helios::runtime::tags::SystemRole;

        /**
         * @brief Updates AABB bounds for text components that need resizing.
         *
         * @param updateContext The current frame's update context.
         */
        void update(helios::runtime::world::UpdateContext& updateContext) noexcept {

            for (auto [entity, txt, usy, usc, active] : updateContext.view<
                THandle,
                helios::ui::widgets::components::UiTextComponent<THandle>,
                helios::ui::widgets::components::UiStyleComponent<THandle>,
                helios::ui::widgets::components::UiStateComponent<THandle>,
                helios::ecs::components::Active<THandle>
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
