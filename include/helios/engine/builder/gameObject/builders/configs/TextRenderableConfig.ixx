/**
 * @file TextRenderableConfig.ixx
 * @brief Fluent configuration for text renderable GameObjects.
 */
module;

#include <memory>
#include <string>
#include <cassert>

export module helios.engine.builder.gameObject.builders.configs.TextRenderableConfig;

import helios.rendering;
import helios.engine.modules.ui.widgets.types.FontId;
import helios.core.types;
import helios.ext.opengl;
import helios.engine.modules.rendering;
import helios.math.types;
import helios.scene.SceneNode;
import helios.engine.modules.ui;



import helios.engine.builder.gameObject.builders.configs.SceneNodeConfig;


export namespace helios::engine::builder::gameObject::builders::configs {

    /**
     * @brief Fluent configuration for text renderable GameObjects.
     *
     * Provides a builder-style interface for configuring text rendering
     * properties including font, color, shader, and optional UI text binding.
     */
    template<typename Entity>
    class TextRenderableConfig {

        using Handle_type = typename Entity::Handle_type;

        /**
         * @brief The GameObject being configured.
         */
        Entity gameObject_;

        /**
         * @brief Pointer to the font resource provider.
         */
        helios::rendering::text::FontResourceProvider* fontResourceProvider_;

        /**
         * @brief The shader used for text rendering.
         */
        std::shared_ptr<helios::ext::opengl::rendering::shader::LegacyOpenGLShader> shader_;

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
         * @brief Format string for number display (std::vformat syntax).
         */
        std::string numberFormat_ = "{:0.f}";

        /**
         * @brief Whether this text should be configured as UI text.
         */
        bool isUiText_ = false;

        /**
         * @brief The font identifier.
         */
        helios::engine::modules::ui::widgets::types::FontId fontId_{helios::core::types::no_init};

        /**
         * @brief Format string for timestamp display (std::vformat syntax).
         */
        std::string timestampFormat_;

        /**
         * @brief Whether a TimeFormatterComponent should be attached.
         */
        bool usesTimeFormatter_;

        /**
         * @brief Whether a NumberFormatterComponent should be attached.
         */
        bool usesNumberFormatter_;

        /**
         * @brief Whether the timer should display remaining instead of elapsed time.
         */
        bool displayRemaining_;

        /**
         * @brief Label displayed instead of "00:00" when remaining time reaches zero.
         */
        std::string elapsedLabel_;

        /**
         * @brief True if an elapsed label has been configured.
         */
        bool showElapsedLabel_ = false;

        /**
         * @brief True if the output should be empty when remaining time reaches zero.
         */
        bool hideWhenZero_ = false;



    public:

        /**
         * @brief Constructs a TextRenderableConfig for the given GameObject.
         *
         * @param gameObject The GameObject to configure.
         */
        explicit TextRenderableConfig(
            Entity gameObject
        ) : gameObject_(gameObject) {}

        /**
         * @brief Sets the shader for text rendering.
         *
         * @param shader The shader to use.
         *
         * @return Reference to this config for method chaining.
         */
        TextRenderableConfig& shader(const std::shared_ptr<helios::ext::opengl::rendering::shader::LegacyOpenGLShader>& shader) {
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
        SceneNodeConfig<Entity> attachTo(Entity parent) {

            build();

            SceneNodeConfig<Entity> scn{gameObject_};

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
        SceneNodeConfig<Entity> attachTo(helios::scene::SceneNode* parent) {

            build();

            SceneNodeConfig<Entity> scn{gameObject_};

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
         * @brief Configures this text as a UI text component.
         *
         * @return Reference to this config for method chaining.
         */
        TextRenderableConfig& asUiText() {
            isUiText_ = true;
            return *this;
        }

        /**
         * @brief Attaches a NumberFormatterComponent with the given format.
         *
         * Implicitly enables UI text mode.
         *
         * @param numberFormat A std::vformat-compatible string. Defaults to "{:.0f}".
         *
         * @return Reference to this config for method chaining.
         */
        TextRenderableConfig& formattedAsNumber(std::string numberFormat = "{:.0f}") {
            numberFormat_ = std::move(numberFormat);
            usesNumberFormatter_ = true;
            return *this;
        }

        /**
         * @brief Attaches a TimeFormatterComponent with the given format.
         *
         * Implicitly enables UI text mode.
         *
         * @param timestampFormat A std::vformat-compatible string expecting minutes and seconds.
         *
         * @return Reference to this config for method chaining.
         */
        TextRenderableConfig& formattedAsTimestamp(std::string timestampFormat) {
            timestampFormat_ = std::move(timestampFormat);
            usesTimeFormatter_ = true;
            return *this;
        }

        /**
         * @brief Sets the label shown instead of "00:00" when remaining time reaches zero.
         *
         * Only effective in Remaining mode. Once the remaining time drops to
         * one second or below, format() returns this label instead of the
         * numeric output.
         *
         * @param label The label string to display (e.g. "TIME UP").
         *
         * @return Reference to this config for method chaining.
         */
        TextRenderableConfig& useElapsedLabel(std::string label) {
            elapsedLabel_ = std::move(label);
            showElapsedLabel_ = true;
            return *this;
        }

        /**
         * @brief Hides the time output when remaining time reaches zero.
         *
         * Only effective in Remaining mode. When enabled, format() returns
         * an empty string once the remaining time is exhausted.
         *
         * @return Reference to this config for method chaining.
         */
        TextRenderableConfig& hideWhenZero() {
            hideWhenZero_ = true;
            return *this;
        }

        /**
         * @brief Switches the time formatter to display remaining time.
         *
         * Only effective when formattedAsTimestamp() has been called.
         *
         * @return Reference to this config for method chaining.
         */
        TextRenderableConfig& displayRemaining() {
            displayRemaining_ = true;
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
        TextRenderableConfig& fontId(helios::engine::modules::ui::widgets::types::FontId fontId) {
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

            gameObject_.template add<helios::engine::modules::rendering::renderable::components::RenderableComponent<Handle_type>>(renderable);

            auto& msc = gameObject_.template getOrAdd<helios::engine::modules::rendering::model::components::ModelAabbComponent<Handle_type>>();
            msc.setAabb(renderable->localAABB());

            if (isUiText_ || usesNumberFormatter_ || usesTimeFormatter_) {
                gameObject_.template add<helios::engine::modules::ui::widgets::components::UiTextComponent<Handle_type>>(renderable.get());
                isUiText_ = false;
            }

            if (usesTimeFormatter_) {
                auto& tfc = gameObject_.template add<helios::engine::modules::ui::layout::components::TimeFormatterComponent<Handle_type>>();
                tfc.setFormat(
                    timestampFormat_,
                    displayRemaining_ ? modules::ui::layout::types::TimeDisplayMode::Remaining
                                      : modules::ui::layout::types::TimeDisplayMode::Elapsed
                );
                if (showElapsedLabel_) {
                    tfc.setElapsedLabel(elapsedLabel_);
                }
                tfc.setHideWhenZero(hideWhenZero_);
                usesTimeFormatter_ = false;
            }

            if (usesNumberFormatter_) {
                gameObject_.template add<helios::engine::modules::ui::layout::components::NumberFormatterComponent<Handle_type>>()
                            .setFormat(numberFormat_);
                usesNumberFormatter_ = false;
            }


            return *this;
        }

    };

}
