module;

#include <GLFW/glfw3.h>
#include <array>

export module helios.ext.glfw.input.GLFWKeyLookup;

import helios.input.types.Key;

export namespace helios::ext::glfw::input::GLFWKeyLookup {

    /**
     * @brief Provides a lookup for converting helios key codes to glfw-specific key codes.
     *
     * @param key The helios key to map to the key codes registered by GLFW
     *
     * @return A key-code in glfw space.
     */
    inline int from(const helios::input::types::Key key) noexcept {

        static constexpr std::array<int, helios::input::types::Key::size_> HELIOS_TO_GLFW=[] {
            std::array<int, helios::input::types::Key::size_> map{};

            map[helios::input::types::Key::ESC] = GLFW_KEY_ESCAPE;

            return map;
        }();

        return HELIOS_TO_GLFW[key];
    }

}