module;

#include <numbers>

export module helios.math.core:utils;

export namespace helios::math {

    constexpr float radians(const float degrees) noexcept {
        return degrees * static_cast<float>(std::numbers::pi)/180.0f;
    }

}