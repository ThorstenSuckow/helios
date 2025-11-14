/**
 * @file vec2.ixx
 * @brief 2D vector type and utility functions.
 */
module;

#include <cassert>
#include <memory>

export module helios.math.types:vec2;

import helios.math.concepts;


export namespace helios::math {

    /**
     * @brief Represents a 2-dimensional vector of the generic type <T>.
     *
     * The `vec2` struct provides a lightweight and efficient way to handle 2D
     * vector mathematics for the numeric types float, int, double. For convenient access,
     * the shorthands `vec2f`, `vec2d` and `vec2i` are available.
     *
     * @tparam T The numeric type of the vector components.
     */
    template<helios::math::Numeric T>
    struct vec2 {

    private:
        /**
         * @brief Internal array storing the vector components.
         */
        T v[2];

    public:

        /**
         * @brief Creates a new vec2 with its values initialized to (0, 0)
         */
        constexpr vec2() noexcept : v{static_cast<T>(0), static_cast<T>(0)} {}


        /**
         * @brief Constructs a new vec2 with the specified x, y components.
         *
         * @param x The value for the x component.
         * @param y The value for the y component.
         */
        constexpr vec2(const T x, const T y) noexcept : v{x, y} {}


        /**
         * @brief Provides read only access to the vector components.
         * Bounds checking is performed via `assert` in debug builds.
         *
         * @param i The index to query
         *
         * @return A const ref to the requested component.
         */
        constexpr const T& operator[](const size_t i) const noexcept {
            assert(i <= 1 && "vec2 - Index out of bounds.");
            return this->v[i];
        }


        /**
         * @brief Provides read-write  access to the vector components.
         * Bounds checking is performed via `assert` in debug builds.
         *
         * @param i The index to query
         *
         * @return A ref to the requested component.
         */
        constexpr T& operator[](const size_t i) noexcept {
            assert(i <= 1 && "vec2 - Index out of bounds.");
            return this->v[i];
        }

    };

    /**
     * @brief Multiplies a 2D vector by a scalar value.
     *
     * @tparam T The numeric type of the vector components.
     * @param v The vec2<T> vector to be multiplied.
     * @param n The scalar vector to multiplay the vector by.
     *
     * @return a new vec2<T> instance representing the result of the scalar
     * multiplication.
     */
    template<helios::math::Numeric T>
    constexpr vec2<T> operator*(const vec2<T>& v, const T n) noexcept {
        return vec2<T>{v[0] * n, v[1] * n};
    }


    /**
     * @brief Computes the dot product of two 2D vectors.
     *
     * @tparam T The numeric type of the vector components.
     * @param v1 The first vec2<T> vector.
     * @param v2 The second vec2<T> vector.
     *
     * @return The dot product as a value of type T.
     */
    template<helios::math::Numeric T>
    constexpr T dot(const vec2<T>& v1, const vec2<T>& v2) noexcept {
        return v1[0]*v2[0] + v1[1]*v2[1];
    }

    using vec2f = vec2<float>;
    using vec2d = vec2<double>;
    using vec2i = vec2<int>;

}
