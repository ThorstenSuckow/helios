/**
 * @file utils.ixx
 * @brief Misc math utility functions.
 */
module;

#include <numbers>

export module helios.math.utils;

export namespace helios::math {

    /**
     * @brief Converts an angle from degrees to radians.
     *
     * @param angle The angle value in degrees.
     * @return The converted angle value in radians.
     */
    constexpr float radians(const float angle) noexcept {
        return angle * static_cast<float>(std::numbers::pi)/180.0f;
    }

    /**
     * @brief Converts an angle from radians to degrees.
     *
     * @param radians The angle value in radians.
     * @return The converted angle value in degrees.
     */
    constexpr float degrees(const float radians) noexcept {
        return radians * 180.0f/static_cast<float>(std::numbers::pi);
    }


}