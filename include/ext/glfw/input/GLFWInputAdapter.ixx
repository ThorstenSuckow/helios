/**
 * @file GLFWInputAdapter.ixx
 * @brief GLFW-based InputAdapter implementation.
 */
module;

#include <GLFW/glfw3.h>
#include <array>
#include <memory>
#include <format>
#include <stdexcept>
#include <utility>
#include <cmath>

export module helios.ext.glfw.input.GLFWInputAdapter;

import helios.input.gamepad.GamepadState;
import helios.input.gamepad.DeadzoneStrategy;
import helios.input.InputAdapter;
import helios.input.types.Gamepad;
import helios.window.Window;
import helios.input.types.Key;
import helios.input.gamepad.GamepadSettings;

import helios.ext.glfw.input.GLFWKeyLookup;
import helios.ext.glfw.input.GLFWGamepadLookup;
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
            const helios::window::Window& win) const noexcept override {
            auto const* win_ptr = dynamic_cast<helios::ext::glfw::window::GLFWWindow const*>(&win);

            if (!win_ptr) {
                logger_.warn("GLFWInput requires GLFWWindow");
                return false;
            }

            return isKeyPressed(key, *win_ptr);
        }

        /**
         * @brief Checks if a specific key is currently released for the given window.
         *
         * If the specified window is not of type GLFWWindow, this method always returns false.
         *
         * @copydoc helios::input::InputAdapter::isKeyReleased()
         */
        [[nodiscard]] bool isKeyReleased(helios::input::types::Key key,
            const helios::window::Window& win) const noexcept override {
            auto const* win_ptr = dynamic_cast<helios::ext::glfw::window::GLFWWindow const*>(&win);

            if (!win_ptr) {
                logger_.warn("GLFWInput requires GLFWWindow");
                return false;
            }

            return isKeyReleased(key, *win_ptr);
        }

        /**
         * @brief Checks if a specific key is currently pressed for the given GLFWWindow.
         *
         * @param key The helios key to check.
         * @param win The `GLFWWindow` instance to query for the key-press.
         *
         * @return true if the key is pressed, otherwise false.
         */
        [[nodiscard]] bool isKeyPressed(helios::input::types::Key key,
            const helios::ext::glfw::window::GLFWWindow& win) const noexcept {
            return glfwGetKey(
                win.nativeHandle(),
                helios::ext::glfw::input::GLFWKeyLookup::from(key)
            ) == GLFW_PRESS;
        }

        /**
         * @brief Checks if a specific key is currently released for the given GLFWWindow.
         *
         * @param key The helios key to check.
         * @param win The `GLFWWindow` instance to query for the key-release.
         *
         * @return true if the key is released, otherwise false.
         */
        [[nodiscard]] bool isKeyReleased(helios::input::types::Key key,
            const helios::ext::glfw::window::GLFWWindow& win) const noexcept {
            return glfwGetKey(
                win.nativeHandle(),
                helios::ext::glfw::input::GLFWKeyLookup::from(key)
            ) == GLFW_RELEASE;
        }

        /**
         * @copydoc helios::input::InputAdapter::gamepadState()
         */
        [[nodiscard]] const helios::input::gamepad::GamepadState& gamepadState(
            helios::input::types::Gamepad gamepadId) const noexcept override {
            return gamepadStates_[
                helios::ext::glfw::input::GLFWGamepadLookup::toArrayIndex(gamepadId)
            ];
        }

        /**
         * @copydoc helios::input::InputAdapter::updateGamepadState()
         */
        void updateGamepadState(unsigned int gamepadMask) noexcept override {
            int index = 0;
            while (gamepadMask != 0) {

                unsigned int id = gamepadMask & 1;

                if (id) {
                    // update gamepad
                    const auto gamepadId = static_cast<helios::input::types::Gamepad>(static_cast<unsigned int>(std::pow(2, index)));

                    int glfwGamepadId = helios::ext::glfw::input::GLFWGamepadLookup::from(gamepadId);

                    auto& gamepadSettings = InputAdapter::gamepadSettings(gamepadId);


                    if (glfwJoystickIsGamepad(glfwGamepadId)) {
                        GLFWgamepadstate state;
                        glfwGetGamepadState(glfwGamepadId, &state);

                        const float* axes = state.axes;
                        const unsigned char* buttons = state.buttons;

                        // left stick
                        float leftX = (gamepadSettings.invertLeftX() ? -1.0f : 1.0f) * axes[GLFW_GAMEPAD_AXIS_LEFT_X];
                        // invert left Y, glfw uses down:1, up:-1, we need down:-1, up:1
                        float leftY = (gamepadSettings.invertLeftY() ? -1.0f : 1.0f) * -axes[GLFW_GAMEPAD_AXIS_LEFT_Y];
                        deadzoneStrategy_->normalize(gamepadSettings.leftStickDeadzone(), leftX, leftY);

                        // right stick
                        float rightX = (gamepadSettings.invertRightX() ? -1.0f : 1.0f) * axes[GLFW_GAMEPAD_AXIS_RIGHT_X];
                            // invert right Y, same logic as left Y
                        float rightY = (gamepadSettings.invertRightY() ? -1.0f : 1.0f) * -axes[GLFW_GAMEPAD_AXIS_RIGHT_Y];
                        deadzoneStrategy_->normalize(gamepadSettings.rightStickDeadzone(), rightX, rightY);

                        gamepadStates_[index].updateAxes(
                            leftX,
                            leftY,
                            rightX,
                            rightY,
                            // normalize trigger values, since glfw returns them in the range
                            // [-1, 1]
                            axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER]/2 + 0.5f ,
                            axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER]/2 + 0.5f,

                            // button section
                            !!buttons[GLFW_GAMEPAD_BUTTON_A],
                            !!buttons[GLFW_GAMEPAD_BUTTON_B],
                            !!buttons[GLFW_GAMEPAD_BUTTON_X],
                            !!buttons[GLFW_GAMEPAD_BUTTON_Y],
                            !!buttons[GLFW_GAMEPAD_BUTTON_START],
                            !!buttons[GLFW_GAMEPAD_BUTTON_BACK],
                            !!buttons[GLFW_GAMEPAD_BUTTON_GUIDE],
                            !!buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER],
                            !!buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER],
                            !!buttons[GLFW_GAMEPAD_BUTTON_LEFT_THUMB],
                            !!buttons[GLFW_GAMEPAD_BUTTON_RIGHT_THUMB],
                            !!buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP],
                            !!buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT],
                            !!buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN],
                            !!buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT]
                        );

                    } else {
                        // could also be a reset() method in GamepadState-object,
                        // but since we use it only once a manual assignment will do for now
                        gamepadStates_[index].updateAxes(
                            0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                            false, false, false, false, false,
                            false, false, false, false,
                            false, false, false, false,
                            false, false

                        );
                    }

                } // if id

                index++;
                gamepadMask >>= 1;
            }
        }

        /**
         * @copydoc helios::input::InputAdapter::isConnected()
         */
        [[nodiscard]] bool isConnected(
            helios::input::types::Gamepad gamepadId) const noexcept override {
            const int glfwGamepadId = helios::ext::glfw::input::GLFWGamepadLookup::from(gamepadId);
            return glfwJoystickPresent(glfwGamepadId) != 0;
        }
    };


}