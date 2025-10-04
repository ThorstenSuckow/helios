module;

export module helios.ext.glfw.input.GLFWInput;

import helios.input.InputAdapter;
import helios.window.Window;
import helios.input.types.Key;

import helios.ext.glfw.input.GLFWKeyLookup;
import helios.ext.glfw.window.GLFWWindow;

export namespace helios::ext::glfw::input {

    class GLFWInput : public helios::input::InputAdapter {

        [[nodiscard]] bool isKeyPressed(helios::input::types::Key key,
            const helios::window::Window& win) const noexcept override;

        [[nodiscard]] bool isKeyPressed(helios::input::types::Key key,
            const helios::ext::glfw::window::GLFWWindow& win) const noexcept;

    };


}