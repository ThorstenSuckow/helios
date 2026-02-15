/**
 * @file TextRenderableConfig.ixx
 * @brief Fluent configuration for text renderable GameObjects.
 */
module;

#include <memory>
#include <string>
#include <cassert>

export module helios.engine.builder.gameObject.builders.configs.TextRenderableConfig;

import helios.engine.ecs.GameObject;
import helios.rendering;
import helios.engine.core.data;
import helios.core.types;
import helios.ext.opengl;
import helios.engine.modules.rendering;
import helios.math.types;
import helios.scene.SceneNode;
import helios.engine.modules.ui.widgets.components.UiTextComponent;


import helios.engine.builder.gameObject.builders.configs.SceneNodeConfig;


export namespace helios::engine::builder::gameObject::builders::configs {

    /**
     * @brief Fluent configuration for text renderable GameObjects.
     *
     * Provides a builder-style interface for configuring text rendering
     * properties including font, color, shader, and optional UI text binding.
     */
    class TextRenderableConfig {

        /**
         * @brief The GameObject being configured.
         */
        helios::engine::ecs::GameObject gameObject_;

        /**
         * @brief Pointer to the font resource provider.
         */
        helios::rendering::text::FontResourceProvider* fontResourceProvider_;

        /**
         * @brief The shader used for text rendering.
         */
        std::shared_ptr<helios::ext::opengl::rendering::shader::OpenGLShader> shader_;

        /**
         * @brief The text color.
         */
        helios::math::vec4f color_;

        /**
         * @brief The text content to display.
         */
        std::string text_;

        /**
         * @brief The font scale factor.
         */
        float fontScale_;

        /**
         * @brief Format template for UI text binding.
         */
        std::string template_ = "";

        /**
         * @brief Whether this text should be configured as UI text.
         */
        bool isUiText_ = false;

        /**
         * @brief The font identifier.
         */
        helios::engine::core::data::FontId fontId_{helios::core::types::no_init};

    public:

        /**
         * @brief Constructs a TextRenderableConfig for the given GameObject.
         *
         * @param gameObject The GameObject to configure.
         */
        explicit TextRenderableConfig(
            helios::engine::ecs::GameObject gameObject
        ) : gameObject_(gameObject) {}

        /**
         * @brief Sets the shader for text rendering.
         *
         * @param shader The shader to use.
         *
         * @return Reference to this config for method chaining.
         */
        TextRenderableConfig& shader(const std::shared_ptr<helios::ext::opengl::rendering::shader::OpenGLShader>& shader) {
            shader_ = std::move(shader);

            return *this;
        }

        /**
         * @brief Sets the text color.
         *
         * @param color The RGBA color.
         *
         * @return Reference to this config for method chaining.
         */
        TextRenderableConfig& color(const helios::math::vec4f color) {
            color_ = color;

            return *this;
        }

        /**
         * @brief Builds the text renderable and attaches to a parent GameObject.
         *
         * @param parent The parent GameObject to attach to.
         *
         * @return A SceneNodeConfig for further configuration.
         */
        SceneNodeConfig attachTo(helios::engine::ecs::GameObject parent) {

            build();

            SceneNodeConfig scn{gameObject_};

            scn.parent(parent);

            return scn;
        }

        /**
         * @brief Builds the text renderable and attaches to a parent SceneNode.
         *
         * @param parent The parent SceneNode to attach to.
         *
         * @return A SceneNodeConfig for further configuration.
         */
        SceneNodeConfig attachTo(helios::scene::SceneNode* parent) {

            build();

            SceneNodeConfig scn{gameObject_};

            scn.parent(parent);

            return scn;
        }

        /**
         * @brief Sets the font resource provider.
         *
         * @param provider The font resource provider.
         *
         * @return Reference to this config for method chaining.
         */
        TextRenderableConfig& fontResourceProvider(helios::rendering::text::FontResourceProvider& provider) {
            fontResourceProvider_ = &provider;
            return *this;
        }

        /**
         * @brief Configures this text as UI text with optional template.
         *
         * @param templateTxt The format template (uses std::format syntax).
         *
         * @return Reference to this config for method chaining.
         */
        TextRenderableConfig& asUiText(std::string templateTxt = "") {
            template_ = std::move(templateTxt);
            isUiText_ = true;
            return *this;
        }

        /**
         * @brief Sets the text content.
         *
         * @param text The text to display.
         *
         * @return Reference to this config for method chaining.
         */
        TextRenderableConfig& text(std::string text) {
            text_ = std::move(text);
            return *this;
        }

        /**
         * @brief Sets the font identifier.
         *
         * @param fontId The font ID to use.
         *
         * @return Reference to this config for method chaining.
         */
        TextRenderableConfig& fontId(helios::engine::core::data::FontId fontId) {
            fontId_ = fontId;
            return *this;
        }

        /**
         * @brief Sets the font scale factor.
         *
         * @param fontScale The scale factor.
         *
         * @return Reference to this config for method chaining.
         */
        TextRenderableConfig& fontScale(const float fontScale) {
            fontScale_ = fontScale;
            return *this;
        }

        /**
         * @brief Builds the text renderable and adds components to the GameObject.
         *
         * Creates a TextRenderable with the configured properties and adds
         * RenderableComponent and ModelAabbComponent. If configured as UI text,
         * also adds UiTextComponent.
         *
         * @return Reference to this config for method chaining.
         */
        TextRenderableConfig& build() {

            assert(shader_ && "Unexpected missing shape and/or shader");

            auto shaderProperties = std::make_shared<helios::rendering::text::TextShaderProperties>(color_);

            const auto textPrototype = std::make_shared<helios::rendering::text::TextRenderPrototype>(
                shader_, shaderProperties, fontResourceProvider_
            );

            auto renderable = std::make_shared<helios::rendering::text::TextRenderable>(
                    std::make_unique<helios::rendering::text::TextMesh>(std::move(text_), fontScale_, fontId_),
                    textPrototype
            );

            gameObject_.add<helios::engine::modules::rendering::renderable::components::RenderableComponent>(renderable);

            auto& msc = gameObject_.getOrAdd<helios::engine::modules::rendering::model::components::ModelAabbComponent>();
            msc.setAabb(renderable->localAABB());

            if (isUiText_) {
                gameObject_.add<helios::engine::modules::ui::widgets::components::UiTextComponent>(renderable.get())
                            .setTemplate(template_);

                isUiText_ = false;
            }

            return *this;
        }

    };

}
