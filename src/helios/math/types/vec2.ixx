module;

#include <cassert>
#include <memory>

export module helios.math.types:vec2;

export namespace helios::math {

    struct vec2 {

    private:
        float v[4];

    public:
        constexpr vec2() noexcept : v{0.0f, 0.0f} {}

        constexpr vec2(const float x, const float y) noexcept : v{x, y} {}

        constexpr const float& operator[](const size_t i) const noexcept {
            assert(i <= 1 && "vec2 - Index out of bounds.");
            return this->v[i];
        }

        constexpr float& operator[](const size_t i) noexcept {
            assert(i <= 1 && "vec2 - Index out of bounds.");
            return this->v[i];
        }

    };

}
