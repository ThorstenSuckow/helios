module;

#include <cassert>
#include <cmath>
#include <memory>


export module helios.math.core:vec3;

export namespace helios::math {

    struct vec3 {

    private:
        float v[3];

    public:
        constexpr vec3() noexcept : v{0.0f, 0.0f, 0.0f} {}

        constexpr vec3(const float x, const float y, const float z) noexcept : v{x, y, z} {}

        constexpr const float& operator[](const size_t i) const noexcept {
            assert(i <= 2 && "vec3 - Index out of bounds.");
            return this->v[i];
        }

        constexpr float& operator[](const size_t i) noexcept {
            assert(i <= 2 && "vec3 - Index out of bounds.");
            return this->v[i];
        }

        float norm() const noexcept {
            return std::sqrt(
                this->v[0] * this->v[0] + this->v[1] * this->v[1] + this->v[2] * this->v[2]
                );
        }

        vec3 normalize() const noexcept;
    };


    constexpr vec3 operator*(const vec3& v, const float n) noexcept {
        return vec3{v[0] * n, v[1] * n, v[2] * n};
    }

    constexpr vec3 operator*(const float n, const vec3& v) noexcept {
        return vec3{v[0] * n, v[1] * n, v[2] * n};
    }

    inline vec3 vec3::normalize() const noexcept { return (*this) * (1.0f / this->norm()); }

    constexpr vec3 cross(const vec3& x, const vec3& y) noexcept {
        return vec3{
            x[1]*y[2] - x[2]*y[1],
            x[2]*y[0] - x[0]*y[2],
            x[0]*y[1] - x[1]*y[0]
        };
    }

    constexpr float dot(const vec3& x, const vec3& y) noexcept {
        return x[0]*y[0] + x[1]*y[1] + x[2]*y[2];
    }

    constexpr vec3 operator-(const vec3& x, const vec3& y) noexcept {
        return vec3{x[0] - y[0], x[1] - y[1], x[2] - y[2]};
    }
}
