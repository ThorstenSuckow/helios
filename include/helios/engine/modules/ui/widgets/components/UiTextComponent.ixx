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
         * @brief Pointer to the underlying text renderable. Not owned.
         */
        helios::rendering::text::TextRenderable* renderable_;

        /**
         * @brief The current text content.
         */
        std::string text_;

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
        UiTextComponent(const UiTextComponent& other) :renderable_(other.renderable_) {
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
         * @brief Resets the component to its initial state.
         */
        void reset() noexcept {
            isDirty_ = true;
        }

        /**
         * @brief Called when the component is acquired from an object pool.
         *
         * Resets the component to its initial dirty state.
         */
        void onAcquire() noexcept {
            reset();
        }

        /**
         * @brief Called when the component is released back to an object pool.
         *
         * Resets the component to its initial dirty state.
         */
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

            renderable_->setText(text_);

            needsResize_ = true;
            isDirty_ = false;
        }

        /**
         * @brief Sets the text content and immediately updates the renderable.
         *
         * No-op if the new text is identical to the current content.
         * Otherwise marks the component as dirty and calls update().
         *
         * @param text The new text content.
         */
        void setText(const std::string& text) {
            if (text == text_) {
                return;
            }
            text_ = text;
            isDirty_ = true;
            update();
        }

        /**
         * @brief Sets the text content by move and immediately updates the renderable.
         *
         * @copydoc setText(const std::string&)
         */
        void setText(std::string&& text) {
            if (text == text_) {
                return;
            }
            text_ = std::move(text);
            isDirty_ = true;
            update();
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

    };

}