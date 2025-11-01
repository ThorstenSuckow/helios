module;

#include <cassert>
#include <cmath>
#include <memory>

export module helios.math.types:vec3;

import helios.math.concepts;

export namespace helios::math {


    /**
     * @brief Represents a 3-dimensional vector of the generic type <T>.
     *
     * The `vec3` struct provides a lightweight and efficient way to handle 3D
     * vector mathematics for the numeric types float, int, double. For convenient access,
     * the shorthands `vec3f`, `vec3d` and `vec3i` are available.
     *
     * @tparam T The numeric type of the vector components.
     */
    template<helios::math::Numeric T>
    struct vec3 {

    private:
        /**
         * @brief Internal array storing the vector components.
         */
        T v[3];

    public:


        /**
         * @brief Creates a new vec3 with its values initialized to (0, 0, 0)
         */
        constexpr vec3() noexcept : v{static_cast<T>(0), static_cast<T>(0), static_cast<T>(0)} {}


        /**
         * @brief Constructs a new vec3 with the specified x, y, z components.
         *
         * @param x The value for the x component.
         * @param y The value for the y component.
         * @param z The value for the z component.
         *
         */
        constexpr vec3(const T x, const T y, const T z) noexcept : v{x, y, z} {}


        /**
         * @brief Constructs a new vec3 with the specified value as the x,y,z components.
         *
         * @param v The value for the components.
         *
         */
        constexpr explicit vec3(const T v) noexcept : v{v, v, v} {}


        /**
         * @brief Provides read only access to the vector components.
         * Bounds checking is performed via `assert` in debug builds.
         *
         * @param i The index to query
         *
         * @return A const ref to the requested component.
         */
        constexpr const T& operator[](const size_t i) const noexcept {
            assert(i <= 2 && "vec3 - Index out of bounds.");
            return this->v[i];
        }


        /**
         * @brief Provides read-write access to the vector components.
         * Bounds checking is performed via `assert` in debug builds.
         *
         * @param i The index to query
         *
         * @return A const ref to the requested component.
         */
        constexpr T& operator[](const size_t i) noexcept {
            assert(i <= 2 && "vec3 - Index out of bounds.");
            return this->v[i];
        }


        /**
         * @brief Computes the Euclidean norm of this vector and returns it.
         *
         * @return The norm (magnitude) of this vector,
         */
        float norm() const noexcept {
            return std::sqrt(
                this->v[0] * this->v[0] +
                this->v[1] * this->v[1] +
                this->v[2] * this->v[2]
            );
        }



        vec3<T> normalize() const noexcept;
    };


    /**
     * @brief Multiplies a 3D vector by a scalar value.
     *
     * @tparam T The numeric type of the vector components.
     * @param v The vec3<T> vector to be multiplied.
     * @param n The scalar vector to multiplay the vector by.
     *
     * @return a new vec3<T> instance representing the result of the scalar
     * multiplication.
     */
    template<helios::math::Numeric T>
    constexpr vec3<T> operator*(const vec3<T>& v, const T n) noexcept {
        return vec3<T>{v[0] * n, v[1] * n, v[2] * n};
    }


    /**
     * @brief Multiplies a a scalar value by a 3D vector.
     *
     * @tparam T The numeric type of the vector components.
     * @param n The scalar vector to multiplay the vector by.
     * @param v The vec3<T> vector to be multiplied.
     *
     * @return a new vec3<T> instance representing the result of the scalar
     * multiplication.
     */
    template<helios::math::Numeric T>
    constexpr vec3<T> operator*(const float n, const vec3<T>& v) noexcept {
        return vec3{v[0] * n, v[1] * n, v[2] * n};
    }


    /**
     * @brief Returns a normalized version of this vector.
     *
     * @tparam T The numeric type of the vector components.
     *
     * @return A new vec3<T> instance representing the normalized vector.
     */
    template<helios::math::Numeric T>
    inline vec3<T> vec3<T>::normalize() const noexcept { return (*this) * (1.0f / this->norm()); }


    /**
     * @brief Computes the cross product of two 3D vectors.
     *
     * @tparam T The numeric type of the vector components.
     * @param v1 The first vec3<T> vector.
     * @param v2 The second vec3<T> vector.
     *
     * @return A new vec3<T> instance representing the cross product.
     */
    template<helios::math::Numeric T>
    constexpr vec3<T> cross(const vec3<T>& v1, const vec3<T>& v2) noexcept {
        return vec3{
            v1[1]*v2[2] - v1[2]*v2[1],
            v1[2]*v2[0] - v1[0]*v2[2],
            v1[0]*v2[1] - v1[1]*v2[0]
        };
    }


    /**
     * @brief Computes the dot product of two 3D vectors.
     *
     * @tparam T The numeric type of the vector components.
     * @param v1 The first vec3<T> vector.
     * @param v2 The second vec3<T> vector.
     *
     * @return A new vec3<T> instance representing the dot product.
     */
    template<helios::math::Numeric T>
    constexpr float dot(const vec3<T>& x, const vec3<T>& y) noexcept {
        return x[0]*y[0] + x[1]*y[1] + x[2]*y[2];
    }


    /**
     * @brief Computes the difference between two vectors (vector subtraction).
     *
     * @tparam T The numeric type of the vector components.
     * @param v1 The first vec3<T> vector.
     * @param v2 The second vec3<T> vector.
     *
     * @return A new vec3<T> instance representing the difference between v1 and v2.
     */
    template<helios::math::Numeric T>
    constexpr vec3<T> operator-(const vec3<T>& v1, const vec3<T>& v2) noexcept {
        return vec3{v1[0] - v2[0], v1[1] - v2[1], v1[2] - v2[2]};
    }


    using vec3f = vec3<float>;
    using vec3i = vec3<int>;
    using vec3d = vec3<double>;
}
