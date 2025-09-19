module;

#include <cassert>
#include <cmath>
#include <memory>


export module helios.math.types:vec4;

import :vec3;

export namespace helios::math {

    struct vec4 {

    private:
        float v[4];

    public:
        constexpr vec4() noexcept : v{0.0f, 0.0f, 0.0f, 0.0f} {}

        constexpr vec4(const float x, const float y, const float z, const float w) noexcept : v{x, y, z, w} {}

        constexpr vec4(const vec3& vec, const float w) noexcept : v{vec[0], vec[1], vec[2], w} {}

        explicit constexpr vec4(const vec3& vec) noexcept : v{vec[0], vec[1], vec[2], 1.0f} {}

        constexpr const float& operator[](const size_t i) const noexcept {
            assert(i <= 3 && "vec4 - Index out of bounds.");
            return this->v[i];
        }

        constexpr float& operator[](const size_t i) noexcept {
            assert(i <= 3 && "vec4 - Index out of bounds.");
            return this->v[i];
        }

    };

}
