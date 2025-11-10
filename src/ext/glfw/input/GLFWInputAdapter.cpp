module;

#include <GLFW/glfw3.h>
#include <format>
#include <iostream>
#include <stdexcept>
#include <utility>

module helios.ext.glfw.input.GLFWInputAdapter;

import helios.input.GamepadState;
import helios.input.InputAdapter;
import helios.input.types.Gamepad;
import helios.window.Window;
import helios.input.types.Key;

import helios.ext.glfw.input.GLFWKeyLookup;
import helios.ext.glfw.window.GLFWWindow;
import helios.ext.glfw.input.GLFWGamepadLookup;

namespace helios::ext::glfw::input {


    bool GLFWInputAdapter::isKeyPressed(helios::input::types::Key key, const helios::window::Window& win) const noexcept {
        auto const* win_ptr = dynamic_cast<helios::ext::glfw::window::GLFWWindow const*>(&win);

        if (!win_ptr) {
            logger_.warn("GLFWInput requires GLFWWindow");
            return false;
        }

        return isKeyPressed(key, *win_ptr);
    }


    bool GLFWInputAdapter::isKeyPressed(helios::input::types::Key key,
        const helios::ext::glfw::window::GLFWWindow& win) const noexcept {
        return glfwGetKey(
            win.nativeHandle(),
            helios::ext::glfw::input::GLFWKeyLookup::from(key)
        ) == GLFW_PRESS;
    }


    void GLFWInputAdapter::updateGamepadState(unsigned int gamepadMask) noexcept {

        int index = 0;
        while (gamepadMask != 0) {

            unsigned int id = gamepadMask & 1;

            if (id) {
                // update gamepad
                const auto gamepadId = static_cast<helios::input::types::Gamepad>(pow(2, index));

                int glfwGamepadId = helios::ext::glfw::input::GLFWGamepadLookup::from(gamepadId);

                if (glfwJoystickIsGamepad(glfwGamepadId)) {
                    GLFWgamepadstate state;
                    glfwGetGamepadState(glfwGamepadId, &state);

                    const float* axes = state.axes;
                    const unsigned char* buttons = state.buttons;

                    gamepadStates_[index].updateAxes(
                        axes[GLFW_GAMEPAD_AXIS_LEFT_X],
                        // invert left Y, glfw uses down:1, up:-1, we need donw:-1, up:1
                        -axes[GLFW_GAMEPAD_AXIS_LEFT_Y],
                        axes[GLFW_GAMEPAD_AXIS_RIGHT_X],
                        // invert right Y, same logic as left Y
                        -axes[GLFW_GAMEPAD_AXIS_RIGHT_Y],
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
                    logger_.warn(std::format("Querying gamepad {0} as gamepad failed", std::to_underlying(gamepadId)));
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

    bool GLFWInputAdapter::isConnected(helios::input::types::Gamepad gamepadId) const noexcept {
        const int glfwGamepadId = helios::ext::glfw::input::GLFWGamepadLookup::from(gamepadId);
        return  glfwJoystickPresent(glfwGamepadId) != 0;
    }


    const helios::input::GamepadState& GLFWInputAdapter::gamepadState(
        const helios::input::types::Gamepad gamepadId) const noexcept {

        return gamepadStates_[
            helios::ext::glfw::input::GLFWGamepadLookup::toArrayIndex(gamepadId)
        ];
    }

}