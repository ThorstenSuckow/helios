module;

#include <GLFW/glfw3.h>
#include <array>

export module helios.platform.input.glfw:GLFWKeyLookup;

import helios.platform.input.types;

export namespace helios::platform::input::glfw::GLFWKeyLookup {

    inline int from(const Key key) noexcept {

        static constexpr std::array<int,  SIZE> HELIOS_TO_GLFW=[] {
            std::array<int, SIZE> map{};

            map[ESC] = GLFW_KEY_ESCAPE;

            return map;
        }();

        return HELIOS_TO_GLFW[key];
    }

}