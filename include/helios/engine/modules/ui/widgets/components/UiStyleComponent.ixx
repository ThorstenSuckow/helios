/**
 * @file UiStyleComponent.ixx
 * @brief Component for managing UI element visual styles.
 */
export module helios.engine.modules.ui.widgets.components.UiStyleComponent;

import helios.math.types;

export namespace helios::engine::modules::ui::widgets::components {

    /**
     * @class UiStyleComponent
     * @brief Stores color styles for different UI widget states.
     *
     * Defines colors for normal, selected, and disabled states
     * of UI elements.
     */
    class UiStyleComponent {

        bool isEnabled_ = true;

        helios::math::vec4f normalColor_{1.0f, 1.0f, 1.0f, 1.0f};
        helios::math::vec4f selectedColor_{0.8f, 0.8f, 1.0f, 1.0f};
        helios::math::vec4f disabledColor_{0.5f, 0.5f, 0.5f, 1.0f};

        float normalScale_ = 1.0f;
        float selectedScale_ = 1.0f;

    public:

        /**
         * @brief Returns the enabled state.
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
         * @brief Sets the scale for normal state.
         *
         * @param scale The scale factor.
         */
        void setNormalScale(const float scale) noexcept {
            normalScale_ = scale;
        }

        /**
         * @brief Sets the scale for selected state.
         *
         * @param scale The scale factor.
         */
        void setSelectedScale(const float scale) noexcept {
            selectedScale_ = scale;
        }

        /**
         * @brief Sets the color for normal state.
         *
         * @param color The RGBA color.
         */
        void setNormalColor(const helios::math::vec4f color) noexcept {
            normalColor_ = color;
        }

        /**
         * @brief Returns the normal state color.
         */
        [[nodiscard]] const helios::math::vec4f normalColor() const noexcept {
            return normalColor_;
        }

        /**
         * @brief Sets the color for selected state.
         *
         * @param color The RGBA color.
         */
        void setSelectedColor(const helios::math::vec4f color) noexcept {
            selectedColor_ = color;
        }

        /**
         * @brief Returns the selected state color.
         */
        [[nodiscard]] const helios::math::vec4f selectedColor() const noexcept {
            return selectedColor_;
        }

        /**
         * @brief Sets the color for disabled state.
         *
         * @param color The RGBA color.
         */
        void setDisabledColor(const helios::math::vec4f color) noexcept {
            disabledColor_ = color;
        }

        /**
         * @brief Returns the disabled state color.
         */
        [[nodiscard]] const helios::math::vec4f disabledColor() const noexcept {
            return disabledColor_;
        }

        /**
         * @brief Returns the selected state scale.
         */
        [[nodiscard]] float selectedScale() const noexcept {
            return selectedScale_;
        }

        /**
         * @brief Returns the normal state scale.
         */
        [[nodiscard]] float normalScale() const noexcept {
            return normalScale_;
        }
    };

} // namespace helios::engine::modules::ui::widgets::components
