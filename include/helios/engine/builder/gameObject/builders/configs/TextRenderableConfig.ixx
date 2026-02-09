/**
 * @file TextRenderableConfig.ixx
 * @brief Configuration for renderable components (mesh, material, shader).
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

    class TextRenderableConfig {

        helios::engine::ecs::GameObject gameObject_;

        helios::rendering::text::FontResourceProvider* fontResourceProvider_;

        std::shared_ptr<helios::ext::opengl::rendering::shader::OpenGLShader> shader_;

        helios::math::vec4f color_;

        std::string text_;

        float fontScale_;

        std::string template_;

        bool isUiText_ = false;



        helios::engine::core::data::FontId fontId_{helios::core::types::no_init};

    public:


        explicit TextRenderableConfig(
            helios::engine::ecs::GameObject gameObject
        ) : gameObject_(gameObject) {}


        TextRenderableConfig& shader(const std::shared_ptr<helios::ext::opengl::rendering::shader::OpenGLShader>& shader) {
            shader_ = std::move(shader);

            return *this;
        }


        TextRenderableConfig& color(const helios::math::vec4f color) {
            color_ = color;

            return *this;
        }


        void attachTo(helios::scene::SceneNode* parent) {

            build();

            SceneNodeConfig scn{gameObject_};

            scn.parent(parent);
        }

        TextRenderableConfig& fontResourceProvider(helios::rendering::text::FontResourceProvider& provider) {
            fontResourceProvider_ = &provider;
            return *this;
        }

        TextRenderableConfig& asUiText(std::string templateTxt) {
            template_ = std::move(templateTxt);
            isUiText_ = true;
            return *this;
        }

        TextRenderableConfig& text(std::string text) {
            text_ = std::move(text);
            return *this;
        }

        TextRenderableConfig& fontId(helios::engine::core::data::FontId fontId) {
            fontId_ = fontId;
            return *this;
        }

        TextRenderableConfig& fontScale(const float fontScale) {
            fontScale_ = fontScale;
            return *this;
        }


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
