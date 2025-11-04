module;

#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>

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

            if (!id) {
                // update iteration state
                goto next_it;
            }

            {
                // update gamepad
                const auto gamepadId = static_cast<helios::input::types::Gamepad>(pow(2, index));

                int glfwGamepadId = helios::ext::glfw::input::GLFWGamepadLookup::from(gamepadId);
                int count;
                const float* axes = glfwGetJoystickAxes(glfwGamepadId, &count);

                if (count != 6) {
                    logger_.warn(std::format(
                        "Querying gamepad {0} failed, retrieved {1} input states",
                        std::to_underlying(gamepadId) >> 1, count)
                    );
                    // could also be a reset() method in GamepadState-object,
                    // but since we use it only once a manual assignment will do for now
                    gamepadStates_[index].updateAxes(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

                    goto next_it;
                }

                gamepadStates_[index].updateAxes(
                    axes[0],
                    // invert left Y, glfw uses down:1, up:-1, we need donw:-1, up:1
                    -axes[1],
                    axes[2],
                    // invert right Y, same logic as left Y
                    -axes[3],
                    // normalize trigger values, since glfw returns them in the range
                    // [-1, 1]
                    axes[4]/2 + 0.5f ,
                    axes[5]/2 + 0.5f
                );
            }
            // end

            next_it:
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