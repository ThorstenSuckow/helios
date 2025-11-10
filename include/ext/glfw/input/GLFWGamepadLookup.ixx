module;

#include <GLFW/glfw3.h>
#include <array>
#include <bit>

export module helios.ext.glfw.input.GLFWGamepadLookup;

import helios.input.types.Gamepad;

export namespace helios::ext::glfw::input::GLFWGamepadLookup {

    /**
     * @brief Internal method to safely compute an array index for the specified gamepadId,
     * adhering to the logic arrayIndex = log_2(gamepadId)
     *
     * @param gamepadId The gamepadId for which the array index should be returned.
     *
     * @return the array index for the specified gamepadId.
     */
    constexpr unsigned int toArrayIndex(const helios::input::types::Gamepad gamepadId) noexcept {
        return std::bit_width(static_cast<unsigned int>(gamepadId))-1;
    }


    /**
     * @brief Provides a lookup for converting helios gamepad ids glfw-specific gamepad ids.
     *
     * @param gamepadId The helios gamepad id to map to the gamepad ids registered by GLFW
     *
     * @return A gamepad-id in glfw space.
     */
    inline int from(const helios::input::types::Gamepad gamepadId) noexcept {

        static constexpr std::array<int, helios::input::types::Gamepad::size_> HELIOS_GAMEPAD_TO_GLFW=[] {
            std::array<int, helios::input::types::Gamepad::size_> map{};

            map[toArrayIndex(helios::input::types::Gamepad::ONE)] = GLFW_JOYSTICK_1;
            map[toArrayIndex(helios::input::types::Gamepad::TWO)] = GLFW_JOYSTICK_2;
            map[toArrayIndex(helios::input::types::Gamepad::THREE)] = GLFW_JOYSTICK_3;
            map[toArrayIndex(helios::input::types::Gamepad::FOUR)] = GLFW_JOYSTICK_4;

            return map;
        }();

        return HELIOS_GAMEPAD_TO_GLFW[toArrayIndex(gamepadId)];
    }

}