module;

#include <cassert>
#include <memory>


export module helios.math.types:vec4;

import helios.math.concepts;

import :vec3;


export namespace helios::math {

    /**
     * Represents a 4-dimensional vector of the generic type <T>.
     *
     * The `vec4` struct provides a lightweight and efficient way to handle 4D
     * vector mathematics for the numeric types float, int, double. For convenient access,
     * the shorthands `vec4f`, `vec4d` and `vec4i` are available.
     *
     * @tparam T The numeric type of the vector components.
     */
    template<helios::math::Numeric T>
    struct vec4 {

    private:
        /**
         * Internal array storing the vector components.
         */
        T v[4];

    public:

        /**
         * Creates a new vec3 with its values initialized to (0, 0, 0, 0)
         */
        constexpr vec4() noexcept : v{static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0)} {}


        /**
         * Constructs a new vec4 with the specified x, y, z, w components.
         *
         * @param x The value for the x component.
         * @param y The value for the y component.
         * @param z The value for the z component.
         * @param w The value for the w component.
         *
         */
        constexpr vec4(const T x, const T y, const T z, const T w) noexcept : v{x, y, z, w} {}


        /**
         * Constructs a new vec4 from the vec3 vector and the w value.
         *
         * @param vec The vec3<T> vector.
         * @param w The value for the w component.
         */
        constexpr vec4(const vec3<T>& vec, const T w) noexcept : v{vec[0], vec[1], vec[2], w} {}


        /**
         * Constructs a new vec4 from the vec3 vector and the w value.
         *
         * @param vec The vec3<T> vector.
         * @param w The value for the w component.
         */
        explicit constexpr vec4(const vec3<T>& vec) noexcept : v{vec[0], vec[1], vec[2], 1.0f} {}


        /**
         * Provides read only access to the vector components.
         * Bounds checking is performed via `assert` in debug builds.
         *
         * @param i The index to query
         *
         * @return A const ref to the requested component.
         */
        constexpr const T& operator[](const size_t i) const noexcept {
            assert(i <= 3 && "vec4 - Index out of bounds.");
            return this->v[i];
        }


        /**
         * Provides read-write access to the vector components.
         * Bounds checking is performed via `assert` in debug builds.
         *
         * @param i The index to query
         *
         * @return A const ref to the requested component.
         */
        constexpr T& operator[](const size_t i) noexcept {
            assert(i <= 3 && "vec4 - Index out of bounds.");
            return this->v[i];
        }

    };

    using vec4f = vec4<float>;
    using vec4d = vec4<double>;
    using vec4i = vec4<int>;

}
