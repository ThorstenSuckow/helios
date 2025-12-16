/**
 * @file DeadzoneStrategy.cpp
 * @brief Implementation of the deadzone strategy.
 */
module;

#include <algorithm>

module helios.input.gamepad.DeadzoneStrategy;

namespace helios::input::gamepad {

    void DeadzoneStrategy::setStickNoiseThreshold(const float threshold) noexcept {
        stickNoiseThreshold_ = std::clamp(threshold, 0.0f, 0.9f);
    }

    float DeadzoneStrategy::stickNoiseThreshold() const noexcept {
        return stickNoiseThreshold_;
    }

}

