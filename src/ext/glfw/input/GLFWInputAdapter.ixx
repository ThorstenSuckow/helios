module;

export module helios.ext.glfw.input.GLFWInputAdapter;

import helios.input.InputAdapter;
import helios.window.Window;
import helios.input.types.Key;

import helios.ext.glfw.input.GLFWKeyLookup;
import helios.ext.glfw.window.GLFWWindow;

export namespace helios::ext::glfw::input {

    /**
     * InputAdapter implementation for a GLFWWindow.
     *
     * This class translates generic input queries into GLFW-specific API-calls.
     * Input queries are always executed against a GLFWWindow instance, which is to
     * be specified by the caller.
     *
     */
    class GLFWInputAdapter final : public helios::input::InputAdapter {

        /**
         * Checks if a specific key is currently pressed for the given window.
         * If the specified window is not of tyoe GLFWWindow, this method always returns false.
         *
         * @copydoc helios::input::InputAdapter::isKeyPressed()
         */
        [[nodiscard]] bool isKeyPressed(helios::input::types::Key key,
            const helios::window::Window& win) const noexcept override;

        /**
         * Checks if a specific key is currently pressed for the given GLFWWindow.
         *
         * @param key The helios key to check.
         * @param win The `GLFWWindow` instance to query for the key-press.
         *
         * @return true if the key is pressed, otherwise false.
         */
        [[nodiscard]] bool isKeyPressed(helios::input::types::Key key,
            const helios::ext::glfw::window::GLFWWindow& win) const noexcept;

    };


}