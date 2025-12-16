/**
 * @file GLFWInputAdapter.ixx
 * @brief GLFW-based InputAdapter implementation.
 */
module;

#include <array>
#include <memory>

export module helios.ext.glfw.input.GLFWInputAdapter;

import helios.input.gamepad.GamepadState;
import helios.input.gamepad.DeadzoneStrategy;
import helios.input.InputAdapter;
import helios.input.types.Gamepad;
import helios.window.Window;
import helios.input.types.Key;
import helios.input.gamepad.GamepadSettings;

import helios.ext.glfw.input.GLFWKeyLookup;
import helios.ext.glfw.window.GLFWWindow;

export namespace helios::ext::glfw::input {

    /**
     * @brief InputAdapter implementation for a GLFWWindow.
     *
     * This class translates generic input queries into GLFW-specific API calls.
     * Input queries are always executed against a GLFWWindow instance, which is to
     * be specified by the caller.
     *
     * The adapter applies the configured `DeadzoneStrategy` and `GamepadSettings`
     * during gamepad state updates to normalize analog stick input and handle
     * axis inversion.
     *
     * @see InputAdapter for the abstract interface.
     * @see GamepadSettings for per-controller configuration.
     * @see DeadzoneStrategy for input normalization strategies.
     */
    class GLFWInputAdapter final : public helios::input::InputAdapter {



    private:
        /**
         * @brief Array storing the current state of each connected gamepad.
         *
         * Indexed by gamepad ID, each entry holds the most recently polled
         * button and axis states for that controller.
         */
        std::array<helios::input::gamepad::GamepadState, std::to_underlying(helios::input::types::Gamepad::size_)> gamepadStates_= {};

    public:

        /**
         * @brief Constructs a GLFWInputAdapter with the specified deadzone strategy.
         *
         * @param deadzoneStrategy The strategy used for analog stick normalization.
         *                         Ownership is transferred to the base InputAdapter.
         */
        GLFWInputAdapter(std::unique_ptr<helios::input::gamepad::DeadzoneStrategy> deadzoneStrategy) :
        helios::input::InputAdapter(std::move(deadzoneStrategy))
        {}

        /**
         * @brief Checks if a specific key is currently pressed for the given window.
         * If the specified window is not of type GLFWWindow, this method always returns false.
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
        [[nodiscard]] const helios::input::gamepad::GamepadState& gamepadState(
            helios::input::types::Gamepad gamepadId) const noexcept override;

        /**
         * @copydoc helios::input::InputAdapter::updateGamepadState()
         */
        void updateGamepadState(unsigned int mask) noexcept override;

        /**
         * @copydoc helios::input::InputAdapter::isConnected()
         */
        [[nodiscard]] bool isConnected(
            helios::input::types::Gamepad gamepadId) const noexcept override;
    };


}