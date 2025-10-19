module;

#include <GLFW/glfw3.h>
#include <stdexcept>

module helios.ext.glfw.input.GLFWInputAdapter;

import helios.input.InputAdapter;
import helios.window.Window;
import helios.input.types.Key;

import helios.ext.glfw.input.GLFWKeyLookup;
import helios.ext.glfw.window.GLFWWindow;

namespace helios::ext::glfw::input {

    [[nodiscard]] bool GLFWInputAdapter::isKeyPressed(helios::input::types::Key key, const helios::window::Window& win) const noexcept {
        auto const* win_ptr = dynamic_cast<helios::ext::glfw::window::GLFWWindow const*>(&win);

        if (!win_ptr) {
            logger_.warn("GLFWInput requires GLFWWindow");
            return false;
        }

        return isKeyPressed(key, *win_ptr);
    }


    [[nodiscard]] bool GLFWInputAdapter::isKeyPressed(helios::input::types::Key key,
        const helios::ext::glfw::window::GLFWWindow& win) const noexcept {
        return glfwGetKey(
            win.nativeHandle(),
            helios::ext::glfw::input::GLFWKeyLookup::from(key)
        ) == GLFW_PRESS;
    }

}