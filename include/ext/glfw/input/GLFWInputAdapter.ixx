module;

#include <array>

export module helios.ext.glfw.input.GLFWInputAdapter;

import helios.input.GamepadState;
import helios.input.InputAdapter;
import helios.input.types.Gamepad;
import helios.window.Window;
import helios.input.types.Key;

import helios.ext.glfw.input.GLFWKeyLookup;
import helios.ext.glfw.window.GLFWWindow;

export namespace helios::ext::glfw::input {

    /**
     * @brief InputAdapter implementation for a GLFWWindow.
     *
     * This class translates generic input queries into GLFW-specific API-calls.
     * Input queries are always executed against a GLFWWindow instance, which is to
     * be specified by the caller.
     *
     */
    class GLFWInputAdapter final : public helios::input::InputAdapter {



    private:
        /**
         * @brief The const array to reference GamepadState objects.
         */
        std::array<helios::input::GamepadState, std::to_underlying(helios::input::types::Gamepad::size_)> gamepadStates_= {};

    public:
        /**
         * @brief Checks if a specific key is currently pressed for the given window.
         * If the specified window is not of tyoe GLFWWindow, this method always returns false.
         *
         * @copydoc helios::input::InputAdapter::isKeyPressed()
         */
        [[nodiscard]] bool isKeyPressed(helios::input::types::Key key,
            const helios::window::Window& win) const noexcept override;

        /**
         * @brief Checks if a specific key is currently pressed for the given GLFWWindow.
         *
         * @param key The helios key to check.
         * @param win The `GLFWWindow` instance to query for the key-press.
         *
         * @return true if the key is pressed, otherwise false.
         */
        [[nodiscard]] bool isKeyPressed(helios::input::types::Key key,
            const helios::ext::glfw::window::GLFWWindow& win) const noexcept;

        /**
         * @copydoc helios::input::InputAdapter::gamepadState()
         */
        [[nodiscard]] const helios::input::GamepadState& GLFWInputAdapter::gamepadState(
            helios::input::types::Gamepad gamepadId) const noexcept override;

        /**
         * @copydoc helios::input::InputAdapter::updateGamepadState()
         */
        void GLFWInputAdapter::updateGamepadState(
            unsigned int mask) noexcept override;

        /**
         * @copydoc helios::input::InputAdapter::isConnected()
         */
        [[nodiscard]] bool GLFWInputAdapter::isConnected(
            helios::input::types::Gamepad gamepadId) const noexcept override;
    };


}