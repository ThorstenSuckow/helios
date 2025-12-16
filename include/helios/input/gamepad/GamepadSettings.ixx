/**
 * @file GamepadSettings.ixx
 * @brief Configuration settings for gamepad input processing.
 */
module;

#include <algorithm>

export module helios.input.gamepad.GamepadSettings;

export namespace helios::input::gamepad {

    /**
     * @brief Configuration class for gamepad input normalization and axis behavior.
     *
     * This class encapsulates all settings that affect how raw gamepad input is processed
     * before being consumed by the application. It provides fluent setter methods for
     * convenient method chaining during configuration.
     *
     * **Configurable options:**
     * - **Deadzone values:** Per-stick thresholds for filtering out hardware drift.
     * - **Axis inversion:** Individual control over X/Y axis inversion for both sticks.
     *
     * Deadzone values are clamped to the range [0.0, 0.9] to ensure that at least 10%
     * of the stick's range remains usable.
     *
     * Example usage:
     * ```cpp
     * GamepadSettings settings;
     * settings.setLeftStickDeadzone(0.15f)
     *         .setRightStickDeadzone(0.1f)
     *         .setInvertLeftY(true);
     * ```
     *
     * @see DeadzoneStrategy for how deadzone values are applied during normalization.
     */
    class GamepadSettings {

        /**
         * @brief Deadzone threshold for the left analog stick.
         */
        float leftStickDeadzone_ = 0.0f;

        /**
         * @brief Deadzone threshold for the right analog stick.
         */
        float rightStickDeadzone_ = 0.0f;

        /**
         * @brief Flag to invert the left stick's X-axis.
         */
        bool invertLeftX_ = false;

        /**
         * @brief Flag to invert the left stick's Y-axis.
         */
        bool invertLeftY_ = false;

        /**
         * @brief Flag to invert the right stick's X-axis.
         */
        bool invertRightX_ = false;

        /**
         * @brief Flag to invert the right stick's Y-axis.
         */
        bool invertRightY_ = false;

    public:

        /**
         * @brief Default destructor.
         */
        ~GamepadSettings() = default;

        /**
         * @brief Default constructor with zero deadzone and no axis inversion.
         */
        GamepadSettings() = default;

        /**
         * @brief Sets the deadzone threshold for the left analog stick.
         *
         * @param deadzone The deadzone value, clamped to [0.0, 0.9].
         *
         * @return Reference to this object for method chaining.
         */
        GamepadSettings& setLeftStickDeadzone(float deadzone) noexcept {
            deadzone = std::clamp(deadzone, 0.0f, 0.9f);
            leftStickDeadzone_ = deadzone;
            return *this;
        }

        /**
         * @brief Sets the deadzone threshold for the right analog stick.
         *
         * @param deadzone The deadzone value, clamped to [0.0, 0.9].
         *
         * @return Reference to this object for method chaining.
         */
        GamepadSettings& setRightStickDeadzone(float deadzone) noexcept {
            deadzone = std::clamp(deadzone, 0.0f, 0.9f);
            rightStickDeadzone_ = deadzone;
            return *this;
        }

        /**
         * @brief Sets whether the left stick's X-axis should be inverted.
         *
         * @param invert True to invert, false for normal behavior.
         *
         * @return Reference to this object for method chaining.
         */
        GamepadSettings& setInvertLeftX(bool invert) noexcept {
            invertLeftX_ = invert;
            return *this;
        }

        /**
         * @brief Sets whether the left stick's Y-axis should be inverted.
         *
         * @param invert True to invert, false for normal behavior.
         *
         * @return Reference to this object for method chaining.
         */
        GamepadSettings& setInvertLeftY(bool invert) noexcept {
            invertLeftY_ = invert;
            return *this;
        }

        /**
         * @brief Sets whether the right stick's X-axis should be inverted.
         *
         * @param invert True to invert, false for normal behavior.
         *
         * @return Reference to this object for method chaining.
         */
        GamepadSettings& setInvertRightX(bool invert) noexcept {
            invertRightX_ = invert;
            return *this;
        }

        /**
         * @brief Sets whether the right stick's Y-axis should be inverted.
         *
         * @param invert True to invert, false for normal behavior.
         *
         * @return Reference to this object for method chaining.
         */
        GamepadSettings& setInvertRightY(bool invert) noexcept {
            invertRightY_ = invert;
            return *this;
        }

        /**
         * @brief Returns the configured deadzone for the left stick.
         *
         * @return The deadzone threshold in the range [0.0, 0.9].
         */
        [[nodiscard]] float leftStickDeadzone() const noexcept {
            return leftStickDeadzone_;
        }

        /**
         * @brief Returns the configured deadzone for the right stick.
         *
         * @return The deadzone threshold in the range [0.0, 0.9].
         */
        [[nodiscard]] float rightStickDeadzone() const noexcept {
            return rightStickDeadzone_;
        }

        /**
         * @brief Returns whether the left stick's X-axis is inverted.
         *
         * @return True if inverted, false otherwise.
         */
        [[nodiscard]] bool invertLeftX() const noexcept {
            return invertLeftX_;
        }

        /**
         * @brief Returns whether the left stick's Y-axis is inverted.
         *
         * @return True if inverted, false otherwise.
         */
        [[nodiscard]] bool invertLeftY() const noexcept {
            return invertLeftY_;
        }

        /**
         * @brief Returns whether the right stick's X-axis is inverted.
         *
         * @return True if inverted, false otherwise.
         */
        [[nodiscard]] bool invertRightX() const noexcept {
            return invertRightX_;
        }

        /**
         * @brief Returns whether the right stick's Y-axis is inverted.
         *
         * @return True if inverted, false otherwise.
         */
        [[nodiscard]] bool invertRightY() const noexcept {
            return invertRightY_;
        }

    };

}