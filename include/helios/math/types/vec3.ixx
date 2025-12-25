/**
 * @file vec3.ixx
 * @brief 3D vector type and utilities.
 */
module;

#include <cassert>
#include <cmath>
#include <memory>

export module helios.math.types:vec3;

import :vec2;
import helios.math.concepts;
import helios.math.traits.FloatingPointType;

export namespace helios::math {

    template<helios::math::Numeric T>
    struct vec4;

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
         * @brief Constructs a new vec3 with x,y components initialized to those of the vec2
         * and vec3 set to 0.
         *
         * @param v The vec2 to use for the x,y components.
         *
         */
        constexpr explicit vec3(const helios::math::vec2<T> v) noexcept : v{v[0], v[1],  static_cast<T>(0)} {}

        /**
         * @brief Constructs a new vec3 with x,y components initialized to those of the vec2
         * and the z component set to the specified value.
         *
         * @param v The vec2 to use for the x,y components.
         * @param f The value for the z component.
         */
        constexpr explicit vec3(const helios::math::vec2<T> v, T f) noexcept : v{v[0], v[1],  f} {}

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
         * @brief Computes the Euclidean norm (magnitude) of this vector and returns it.
         *
         * @return The norm (magnitude) of this vector as a value of type FloatingPointType<T>.
         */
        FloatingPointType<T> length() const noexcept {
            return static_cast<FloatingPointType<T>>(std::sqrt(
                static_cast<double>(this->v[0]) * static_cast<double>(this->v[0]) +
                static_cast<double>(this->v[1]) * static_cast<double>(this->v[1]) +
                static_cast<double>(this->v[2]) * static_cast<double>(this->v[2])
            ));
        }

        /**
         * @brief Computes the cross product of this vector and v2.
         *
         * @param v2 The second vec3<T> vector.
         *
         * @return The cross product as a value of type vec3<T>.
         */
        [[nodiscard]] vec3<T> cross(const helios::math::vec3<T>& v2) const noexcept;

        /**
         * @brief Computes the dot product of this vector and v2.
         *
         * @param v2 The second vec3<T> vector.
         *
         * @return The dot product as a value of type T.
         */
        [[nodiscard]] T dot(const helios::math::vec3<T>& v2) const noexcept;

        /**
         * @brief Converts this 3D vector to a 4D homogeneous vector.
         *
         * @details
         * Creates a vec4 with the x, y, z components from this vector and sets
         * the w component to 0, representing a direction in homogeneous coordinates.
         *
         * @return A new vec4<T> instance with components (x, y, z, 0).
         */
        [[nodiscard]] vec4<T> toVec4() const noexcept;

        /**
         * @brief Converts this 3D vector to a 4D homogeneous vector.
         *
         * @details
         * Creates a vec4 with the x, y, z components from this vector and sets
         * the w component accordingly, representing a direction in homogeneous coordinates.
         *
         * @param w The value of the w component.
         *
         * @return A new vec4<T> instance with components (x, y, z, w).
         */
        [[nodiscard]] vec4<T> toVec4(T w) const noexcept;

        /**
         * @brief Returns a normalized version of this vector.
         *
         * @return A new vec3<FloatingPointType<T>> instance representing the normalized vector.
         */
        [[nodiscard]] vec3<FloatingPointType<T>> normalize() const noexcept;

        /**
         * @brief Strictly compares the elements of this vector with the elements
         * of the rgt vector.
         *
         * @param rgt The right vector to compare for equal values.
         *
         * @return True if all elements are equal (==), false otherwise.
         */
        constexpr bool operator==(const vec3<T>& rgt) const {
            return v[0] == rgt[0] && v[1] == rgt[1] && v[2] == rgt[2];
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
        constexpr bool same(const vec3<T>& rgt, T epsilon = 0.0001) const {
            return std::fabs(v[0] - rgt[0]) <= epsilon &&
                   std::fabs(v[1] - rgt[1]) <= epsilon &&
                   std::fabs(v[2] - rgt[2]) <= epsilon;
        }

        /**
         * @brief Returns a new vector with the y-component negated.
         *
         * @return A new vec3<T> with (x, -y, z).
         */
        constexpr helios::math::vec3<T> flipY() const noexcept {
            return helios::math::vec3<T>{v[0], -v[1], v[2]};
        }

        /**
         * @brief Returns a new vector with the y-component replaced by the given value.
         *
         * @param y The new y value.
         * @return A new vec3<T> with (x, y, z).
         */
        constexpr helios::math::vec3<T> withY(T y) const noexcept {
            return helios::math::vec3<T>{v[0], y, v[2]};
        }

        /**
         * @brief Returns a new vector with the x-component negated.
         *
         * @return A new vec3<T> with (-x, y, z).
         */
        constexpr helios::math::vec3<T> flipX() const noexcept {
            return helios::math::vec3<T>{-v[0], v[1], v[2]};
        }

        /**
         * @brief Returns a new vector with the x-component replaced by the given value.
         *
         * @param x The new x value.
         * @return A new vec3<T> with (x, y, z).
         */
        constexpr helios::math::vec3<T> withX(T x) const noexcept {
            return helios::math::vec3<T>{x, v[1], v[2]};
        }
    };


    /**
     * @brief Multiplies a 3D vector by a scalar value.
     *
     * @tparam T The numeric type of the vector components.
     * @param v The vec3<T> vector to be multiplied.
     * @param n The scalar value to multiply the vector by.
     *
     * @return a new vec3<T> instance representing the result of the scalar
     * multiplication.
     */
    template<helios::math::Numeric T>
    constexpr vec3<T> operator*(const vec3<T>& v, const T n) noexcept {
        return vec3<T>{v[0] * n, v[1] * n, v[2] * n};
    }

    /**
     * @brief Divides a 3D vector by a scalar value.
     *
     * @tparam T The numeric type of the vector components.
     * @param v The vec3<T> vector to be divided.
     * @param s The scalar divisor. Must not be zero.
     *
     * @return A new vec3<T> instance representing the result of the scalar division.
     *
     * @pre s != 0 (asserted in debug builds).
     */
    template<helios::math::Numeric T>
    constexpr vec3<T> operator/(const vec3<T>& v, T s) noexcept {
        assert(static_cast<T>(0) != s && "s must not be 0");
        const T inv = static_cast<T>(1) / s;
        return vec3<T>{ v[0] * inv, v[1] * inv, v[2] * inv };
    }

    /**
     * @brief Multiplies a scalar value by a 3D vector.
     *
     * @tparam T The numeric type of the vector components.
     * @param n The scalar value to multiply the vector by.
     * @param v The vec3<T> vector to be multiplied.
     *
     * @return A new vec3<T> instance representing the result of the scalar
     * multiplication.
     */
    template<helios::math::Numeric T>
    constexpr vec3<T> operator*(const T n, const vec3<T>& v) noexcept {
        return vec3<T>{v[0] * n, v[1] * n, v[2] * n};
    }

    /**
     * @brief Multiplies two vectors componentwise.
     *
     * @tparam T The numeric type of the vector components.
     * @param v1 The left-hand vec3<T> vector to be multiplied.
     * @param v2 The right-hand vec3<T> vector to be multiplied.
     *
     * @return A new vec3<T> instance representing the result of the componentwise multiplication
     * of the two vectors.
     */
    template<helios::math::Numeric T>
    constexpr vec3<T> operator*(const vec3<T>& v1, const vec3<T>& v2) noexcept {
        return vec3<T>{v1[0] * v2[0], v1[1] * v2[1], v1[2] * v2[2]};
    }


    /**
     * @brief Calculates the componentwise sum of the two vectors.
     *
     * @tparam T The numeric type of the vector components.
     * @param v1 The left-hand vec3<T> vector to be added.
     * @param v2 The right-hand vec3<T> vector to be added.
     *
     * @return A new vec3<T> instance representing the sum of the two vectors.
     */
    template<helios::math::Numeric T>
    constexpr vec3<T> operator+(const vec3<T>& v1, const vec3<T>& v2) noexcept {
        return vec3<T>{v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2]};
    }


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
     * @return The dot product as a value of type T.
     */
    template<helios::math::Numeric T>
    constexpr T dot(const vec3<T>& v1, const vec3<T>& v2) noexcept {
        return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
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

    template<helios::math::Numeric T>
        inline vec3<T> vec3<T>::cross(const vec3<T>& v2) const noexcept {
        return vec3{
            v[1]*v2[2] - v[2]*v2[1],
            v[2]*v2[0] - v[0]*v2[2],
            v[0]*v2[1] - v[1]*v2[0]
        };
    }

    template<helios::math::Numeric T>
    inline vec4<T> vec3<T>::toVec4() const noexcept {
        return vec4<T>{v[0], v[1], v[2], static_cast<T>(0)};
    }

    template<helios::math::Numeric T>
   inline vec4<T> vec3<T>::toVec4(T w) const noexcept {
        return vec4<T>{v[0], v[1], v[2], w};
    }

    template<helios::math::Numeric T>
    inline T vec3<T>::dot(const vec3<T>& v2) const noexcept {
        return v[0]*v2[0] + v[1]*v2[1] + v[2]*v2[2];
    }


    template<helios::math::Numeric T>
    inline vec3<FloatingPointType<T>> vec3<T>::normalize() const noexcept {
        if (this->length() == static_cast<FloatingPointType<T>>(0)) {
            return vec3<FloatingPointType<T>>(
                static_cast<FloatingPointType<T>>(0),
                static_cast<FloatingPointType<T>>(0),
                static_cast<FloatingPointType<T>>(0)
            );
        }

        return vec3<FloatingPointType<T>>(
            static_cast<FloatingPointType<T>>(v[0]) / this->length(),
            static_cast<FloatingPointType<T>>(v[1]) / this->length(),
            static_cast<FloatingPointType<T>>(v[2]) / this->length()
        );
    }

    /**
     * @brief Single-precision floating-point 3D vector.
     */
    using vec3f = vec3<float>;

    /**
     * @brief Integer 3D vector.
     */
    using vec3i = vec3<int>;

    /**
     * @brief Double-precision floating-point 3D vector.
     */
    using vec3d = vec3<double>;

} // namespace helios::math
