/**
 * @file RadialDeadzoneStrategy.cpp
 * @brief Implementation of the radial deadzone strategy.
 */
module;

#include <cmath>

module helios.input.gamepad.RadialDeadzoneStrategy;

import helios.math.utils;

namespace helios::input::gamepad {

    void RadialDeadzoneStrategy::normalize(const float deadzone, float& x, float& y) const noexcept {

        auto len = std::hypot(x, y);

        if (len < helios::math::EPSILON_LENGTH || len <= deadzone) {
            x = 0;
            y = 0;
            return;
        }

        const float clampedLen = len > 1.0f ? 1.0f : len;

        const float normalizedLen = (clampedLen - deadzone)/(1.0f - deadzone);
        const float scale = normalizedLen / clampedLen;

        x *= scale;
        y *= scale;

    }

}

