/**
 * @file vec4.ixx
 * @brief 4D vector type and utilities.
 */
module;

#include <cassert>
#include <memory>


export module helios.math.types:vec4;

import helios.math.concepts;

import :vec3;


export namespace helios::math {

    template<helios::math::Numeric T>
    struct vec4;

    /**
     * @brief Represents a 4-dimensional vector of the generic type <T>.
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
         * @brief Internal array storing the vector components.
         */
        T v[4];

    public:

        /**
         * @brief Creates a new vec4 with its values initialized to (0, 0, 0, 0)
         */
        constexpr vec4() noexcept : v{static_cast<T>(0), static_cast<T>(0), static_cast<T>(0), static_cast<T>(0)} {}


        /**
         * @brief Constructs a new vec4 with the specified x, y, z, w components.
         *
         * @param x The value for the x component.
         * @param y The value for the y component.
         * @param z The value for the z component.
         * @param w The value for the w component.
         */
        constexpr vec4(const T x, const T y, const T z, const T w) noexcept : v{x, y, z, w} {}


        /**
         * @brief Constructs a new vec4 from the vec3 vector and the w value.
         *
         * @param vec The vec3<T> vector.
         * @param w The value for the w component.
         */
        constexpr vec4(const vec3<T>& vec, const T w) noexcept : v{vec[0], vec[1], vec[2], w} {}


        /**
         * @brief Constructs a new vec4 from the vec3 vector and sets w to 1.
         *
         * @param vec The vec3<T> vector.
         */
        explicit constexpr vec4(const vec3<T>& vec) noexcept : v{vec[0], vec[1], vec[2], static_cast<T>(1)} {}


        /**
         * @brief Provides read only access to the vector components.
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
         * @brief Provides read-write access to the vector components.
         * Bounds checking is performed via `assert` in debug builds.
         *
         * @param i The index to query
         *
         * @return A ref to the requested component.
         */
        constexpr T& operator[](const size_t i) noexcept {
            assert(i <= 3 && "vec4 - Index out of bounds.");
            return this->v[i];
        }

        /**
         * @brief Converts this 4D vector to a 3D vector.
         *
         * Extracts the x, y, z components from this vec4, discarding the w component.
         * This is useful when converting from homogeneous coordinates back to 3D space.
         *
         * @return A new vec3<T> instance with components (x, y, z).
         */
        [[nodiscard]] vec3<T> toVec3() const noexcept;

    };

    template<helios::math::Numeric T>
    inline vec3<T> vec4<T>::toVec3() const noexcept {
        return vec3<T>{v[0], v[1], v[2]};
    }

    /**
     * @brief Returns a const pointer to the first element of the vector's components.
     *
     * Useful for APIs that expect a pointer to vector data, like OpenGL
     * @param m A reference to the `vec4<T>` vector.
     * @tparam T The numeric type of the vector components.
     */
    template<helios::math::Numeric T>
    const T* value_ptr(const vec4<T>& m) noexcept {
        return &m[0];
    }

    /**
     * @brief Single-precision floating-point 4D vector.
     */
    using vec4f = vec4<float>;

    /**
     * @brief Double-precision floating-point 4D vector.
     */
    using vec4d = vec4<double>;

    /**
     * @brief Integer 4D vector.
     */
    using vec4i = vec4<int>;

}
