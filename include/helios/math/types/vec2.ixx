/**
 * @file vec2.ixx
 * @brief 2D vector type and utility functions.
 */
module;

#include <cassert>
#include <cmath>
#include <memory>

export module helios.math.types:vec2;


import helios.math.concepts;
import helios.math.utils;
import helios.math.traits.FloatingPointType;

export namespace helios::math {

    template<helios::math::Numeric T>
    struct vec3;

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
         * @brief Converts the current vec2<T> into a 3D vector representation.
         *
         * @return A 3D vector corresponding to the converted representation of the object or input.
         */
        constexpr helios::math::vec3<T> toVec3() const {
            return helios::math::vec3<T>{v[0], v[1], static_cast<T>(0)};
        }

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

        /**
         * @brief Returns the magnitude of this vec2<T>.
         *
         * @return The magnitude of this vector as type FloatingPointType<T>.
         */
        inline FloatingPointType<T> length() const noexcept {
            if (v[0] == 0 && v[1] == 0) {
                return static_cast<FloatingPointType<T>>(0);
            }
            return std::hypot(v[0], v[1]);
        }


        /**
         * @brief Checks if this vector is normalized (unit length).
         *
         * @details A vector is considered normalized if its squared length
         * equals 1 within the tolerance defined by EPSILON_LENGTH.
         *
         * @return true if the vector is approximately unit length, false otherwise.
         */
        constexpr bool isNormalized() const noexcept {
            const auto lenSquared =
                static_cast<FloatingPointType<T>>(v[0]) * static_cast<FloatingPointType<T>>(v[0]) +
                static_cast<FloatingPointType<T>>(v[1]) * static_cast<FloatingPointType<T>>(v[1]);

            return std::abs(lenSquared - static_cast<FloatingPointType<T>>(1.0)) <= helios::math::EPSILON_LENGTH;
        }

        /**
         * @brief Normalizes this vec2<T>.
         *
         * @return The normalized FloatingPointType<T> vector.
         */
        inline vec2<FloatingPointType<T>> normalize() const noexcept {
            if (v[0] == 0 && v[1] == 0) {
                return vec2<FloatingPointType<T>>(
                    static_cast<FloatingPointType<T>>(0),
                    static_cast<FloatingPointType<T>>(0)
                );
            }
            return vec2<FloatingPointType<T>>(
                static_cast<FloatingPointType<T>>(v[0]) / length(),
                static_cast<FloatingPointType<T>>(v[1]) / length()
            );
        }

        /**
        * @brief Strictly compares the elements of this vector with the elements
        * of the rgt vector.
        *
        * @param rgt The right vector to compare for equal values
        *
        * @return True if all elements are equal (==), false otherwise.
        */
        constexpr bool operator==(const vec2<T>& rgt) const {
            return v[0] == rgt[0] && v[1] == rgt[1];
        }

        /**
         * @brief Compares this vector's elements with the rgt vector considering
         * an epsilon value.
         * Returns true if for all elements the equation |a-b| <= epsilon
         * holds.
         *
         * @param rgt The other vector to compare with this vector for equality.
         * @param epsilon The epsilon value to use for comparison. If omitted, the default epsilon (0.0001) is used.
         *
         * @return True if the elements of the vectors are considered equal,
         * otherwise false.
         *
         * @see https://realtimecollisiondetection.net/blog/?p=89
         *
         * @todo account for abs (values close to zero) and rel
         * (larger values), move epsilon to global constant?
         */
        constexpr bool same(const vec2<T>& rgt, T epsilon = 0.0001) const {
            return std::fabs(v[0] - rgt[0]) <= epsilon &&
                   std::fabs(v[1] - rgt[1]) <= epsilon;
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
    using vec2ui = vec2<unsigned int>;

}
