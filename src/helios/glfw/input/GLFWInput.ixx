module;

#include <GLFW/glfw3.h>
#include <stdexcept>


export module helios.glfw.input:GLFWInput;

import helios.platform.input.core;
import helios.platform.input.types;
import helios.platform.window.core;
import helios.glfw.window;
import :GLFWKeyLookup;

using namespace helios::platform::window::core;
using namespace helios::glfw::window;

export namespace helios::glfw::input {

    class GLFWInput : public core::InputAdapter {

        [[nodiscard]] bool isKeyPressed(Key key, const Window& win) const override {
            auto const* win_ptr = dynamic_cast<GLFWWindow const*>(&win);

            if (!win_ptr) {
                throw std::runtime_error("GLFWInput requires GLFWWindow");
            }

            return isKeyPressed(key, *win_ptr);
        }


        [[nodiscard]] bool isKeyPressed(Key key,
            const GLFWWindow& win) const noexcept {

            return glfwGetKey(
                win.nativeHandle(),
                GLFWKeyLookup::from(key)
            ) == GLFW_PRESS;
        }

    };


}