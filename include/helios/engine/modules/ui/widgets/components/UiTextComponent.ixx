/**
 * @file UiTextComponent.ixx
 * @brief Component for displaying formatted text in the UI.
 */
module;

#include <cassert>
#include <format>
#include <memory>
#include <string>


export module helios.engine.modules.ui.widgets.components.UiTextComponent;

import helios.rendering.text.TextRenderable;
import helios.math.types;
import helios.core.units.Unit;


import helios.engine.ecs.GameObject;

import helios.engine.modules.rendering.renderable.components.RenderableComponent;


export namespace helios::engine::modules::ui::widgets::components {


/**
 * @brief Component for displaying formatted text in the UI.
 *
 * Wraps a TextRenderable and provides template-based value formatting.
 * Supports dirty tracking to minimize rendering updates.
 */
class UiTextComponent {

    protected:

        /**
         * @brief Format template using std::format syntax.
         */
        std::string template_ = "{0}";

        /**
         * @brief Pointer to the underlying text renderable. Not owned.
         */
        helios::rendering::text::TextRenderable* renderable_;

        /**
         * @brief Current numeric value to display.
         */
        double doubleValue_ = {0.0f};

        /**
         * @brief True if the text content needs to be updated.
         */
        bool isDirty_ = true;

        /**
         * @brief True if the AABB bounds need to be recalculated.
         */
        bool needsResize_ = true;

        /**
         * @brief Whether this component is enabled.
         */
        bool isEnabled_ = true;

    public:

        /**
         * @brief Checks whether this component is enabled.
         *
         * @return True if enabled, false otherwise.
         */
        [[nodiscard]] bool isEnabled() const noexcept {
            return isEnabled_;
        }

        /**
         * @brief Enables this component.
         */
        void enable() noexcept {
            isEnabled_ = true;
        }

        /**
         * @brief Disables this component.
         */
        void disable() noexcept {
            isEnabled_ = false;
        }

        /**
         * @brief Constructs a UiTextComponent with the given renderable.
         *
         * @param renderable Pointer to the TextRenderable. Must not be nullptr.
         */
        explicit UiTextComponent(helios::rendering::text::TextRenderable* renderable) :
            renderable_(renderable)  {

            assert(renderable_ != nullptr && "TextRenderable must not be nullptr");
        }

        /**
         * @brief Copy constructor.
         *
         * @param other The component to copy from.
         */
        UiTextComponent(const UiTextComponent& other) : template_(other.template_), renderable_(other.renderable_) {
            assert(renderable_ != nullptr && "TextRenderable must not be nullptr");
        }

        UiTextComponent& operator=(const UiTextComponent&) = default;
        UiTextComponent(UiTextComponent&&) noexcept = default;
        UiTextComponent& operator=(UiTextComponent&&) noexcept = default;

        /**
         * @brief Returns the underlying text renderable as const pointer.
         *
         * @return Const pointer to the TextRenderable. Never nullptr.
         */
        [[nodiscard]] const helios::rendering::text::TextRenderable* renderable() const noexcept {
            return renderable_;
        }

        /**
         * @brief Returns the underlying text renderable as a pointer.
         *
         * @return Pointer to the TextRenderable. Never nullptr.
         */
        [[nodiscard]] helios::rendering::text::TextRenderable* renderable() noexcept {
            return renderable_;
        }

        /**
         * @brief Sets the format template.
         *
         * Uses std::format syntax. The value is inserted at `{0}`.
         *
         * @param templateTxt The format template string.
         */
        void setTemplate(std::string templateTxt) {
            template_ = std::move(templateTxt);
        }


        /**
         * @brief Resets the component to its initial state.
         */
        void reset() noexcept {
            isDirty_ = true;
            doubleValue_ = 0.0f;
        }

        void onAcquire() noexcept {
            reset();
        }

        void onRelease() noexcept {
            reset();
        }

        /**
         * @brief Updates the text content if dirty.
         *
         * Applies the template to the current value and updates the renderable.
         */
        void update() {

            if (!isDirty_) {
                return;
            }

            if (template_ == "{0}" || template_.empty()) {
                renderable_->setText(std::to_string(doubleValue_));
                needsResize_ = true;
                isDirty_ = false;
                return;
            }

            renderable_->setText(std::vformat(std::string_view(template_), std::make_format_args(doubleValue_)));
            needsResize_ = true;
            isDirty_ = false;
        }

        /**
         * @brief Checks if the AABB bounds need recalculation.
         *
         * @return True if a resize is needed, false otherwise.
         */
        [[nodiscard]] bool needsResize() const noexcept {
            return needsResize_;
        }

        /**
         * @brief Marks the resize as complete.
         */
        void resizeComplete() noexcept {
            needsResize_ = false;
        }


        /**
         * @brief Sets the numeric value to display.
         *
         * Triggers an update if the value has changed.
         *
         * @param value The value to display.
         */
        void setDouble(const double value)  {

            if (doubleValue_ == value) {
                return;
            }
            isDirty_ = true;
            doubleValue_ = value;

            update();
        }
    };

}