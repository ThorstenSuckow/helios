/**
 * @file utils.ixx
 * @brief Misc math utility functions.
 */
module;

#include <numbers>

export module helios.math.utils;

export namespace helios::math {

    /**
     * @brief A small epsilon value used for length-based comparisons.
     *
     * @details This constant represents the minimum threshold value for length
     * comparisons to account for numerical precision errors in floating-point
     * calculations. It is often used to avoid treating very small values as
     * significant, effectively filtering out noise in computations that involve
     * vector-lengths.
     */
    constexpr float EPSILON_LENGTH = 1e-6f;

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