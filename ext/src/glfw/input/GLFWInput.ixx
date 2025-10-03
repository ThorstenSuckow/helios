module;

#include <GLFW/glfw3.h>
#include <stdexcept>

export module helios.ext.glfw.input.GLFWInput;

import helios.input.InputAdapter;
import helios.window.Window;
import helios.input.types.Key;

import helios.ext.glfw.input.GLFWKeyLookup;
import helios.ext.glfw.window.GLFWWindow;

export namespace helios::ext::glfw::input {

    class GLFWInput : public helios::input::InputAdapter {

        [[nodiscard]] bool isKeyPressed(helios::input::types::Key key, const helios::window::Window& win) const override {
            auto const* win_ptr = dynamic_cast<helios::ext::glfw::window::GLFWWindow const*>(&win);

            if (!win_ptr) {
                throw std::runtime_error("GLFWInput requires GLFWWindow");
            }

            return isKeyPressed(key, *win_ptr);
        }


        [[nodiscard]] bool isKeyPressed(helios::input::types::Key key,
            const helios::ext::glfw::window::GLFWWindow& win) const noexcept {

            return glfwGetKey(
                win.nativeHandle(),
                helios::ext::glfw::input::GLFWKeyLookup::from(key)
            ) == GLFW_PRESS;
        }

    };


}