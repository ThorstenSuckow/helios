/**
 * @file UiStateComponent.ixx
 * @brief Component for managing UI element interaction states.
 */
export module helios.engine.modules.ui.widgets.components.UiStateComponent;

export namespace helios::engine::modules::ui::widgets::components {

/**
 * @class UiStateComponent
 * @brief Tracks interaction state for UI widgets.
 *
 * Manages common UI states such as selection, hover, focus,
 * enabled/disabled, and pressed states.
 */
class UiStateComponent {

    private:
        bool isEnabled_ = true;

        bool isSelected_ = false;

        bool isHovered_ = false;

        bool isFocused_ = false;

        bool isPressed_ = false;

    public:

        /**
         * @brief Sets the selected state.
         *
         * @param selected True if selected.
         */
        void setSelected(bool selected) noexcept {
            isSelected_ = selected;
        }

        /**
         * @brief Returns the selected state.
         */
        [[nodiscard]] bool isSelected() const noexcept {
            return isSelected_;
        }

        /**
         * @brief Sets the hovered state.
         *
         * @param hovered True if hovered.
         */
        void setHovered(bool hovered) noexcept {
            isHovered_ = hovered;
        }

        /**
         * @brief Returns the hovered state.
         */
        [[nodiscard]] bool isHovered() const noexcept {
            return isHovered_;
        }

        /**
         * @brief Sets the focused state.
         *
         * @param focused True if focused.
         */
        void setFocused(bool focused) noexcept {
            isFocused_ = focused;
        }

        /**
         * @brief Returns the focused state.
         */
        [[nodiscard]] bool isFocused() const noexcept {
            return isFocused_;
        }

        /**
         * @brief Sets the enabled state.
         *
         * @param enabled True if enabled.
         */
        void setEnabled(bool enabled) noexcept {
            isEnabled_ = enabled;
        }

        /**
         * @brief Returns the enabled state.
         */
        [[nodiscard]] bool isEnabled() const noexcept {
            return isEnabled_;
        }

        /**
         * @brief Sets the pressed state.
         *
         * @param pressed True if pressed.
         */
        void setPressed(bool pressed) noexcept {
            isPressed_ = pressed;
        }

        /**
         * @brief Returns the pressed state.
         */
        [[nodiscard]] bool isPressed() const noexcept {
            return isPressed_;
        }
    };

} // namespace helios::engine::modules::ui::widgets::components
