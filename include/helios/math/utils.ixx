module;

#include <numbers>

export module helios.math.utils;

export namespace helios::math {

    /**
     * @brief Converts an angle from degrees to radians.
     *
     * @param angle The angle value in degrees.
     *
     * @return The converted angle value in radians.
     */
    constexpr float radians(const float angle) noexcept {
        return angle * static_cast<float>(std::numbers::pi)/180.0f;
    }

}