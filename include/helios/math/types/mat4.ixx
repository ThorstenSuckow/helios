/**
 * @file mat4.ixx
 * @brief 4x4 matrix type and utilities.
 */
module;

#include <cassert>
#include <functional>

export module helios.math.types:mat4;

import :vec3;
import helios.math.concepts;


export namespace helios::math {

    /**
     * @brief Represents a 4x4 matrix, stored in column-major order.
     *
     * The `mat4` struct provides a lightweight and efficient way to handle 4D
     * matrix mathematics for numeric data types.
     * It stores its components as type `T`. For convenient access, type aliases
     * `mat4f`, `mat4d` and `mat4i` are available, providing float/double/integer
     * matrix representatives.
     *
     * @tparam T the numeric type of the matrix components.
     */
    template<helios::math::Numeric T>
    struct mat4 {
    private:
        /**
         * @brief Internal array storing matrix components.
         *
         * Components are stored in column-major order, that is, each 4 subsequent elements
         * represent a matrix column.
         */
        T m[16];

    public:
        /**
         * @brief Default constructor. Initializes all components to 0.
         */
        explicit constexpr mat4() noexcept : m{} {};

        /**
         * @brief Creates a diagonal matrix. The diagonal components are initialized
         * with the value f.
         *
         * @param f The scalar value for the diagonal components.
         */
        explicit constexpr mat4(const T f) noexcept
            : m{ f,  T{}, T{}, T{},
                T{},  f,  T{}, T{},
                T{}, T{},  f,  T{},
                T{}, T{}, T{},  f} {}

        /**
        * @brief Creates a diagonal matrix with the components of vec3<T> as the
        * diagonal elements. Element at [4, 4] is set to 1.
        *
        * @param f The scalar value for the diagonal components.
        */
        explicit constexpr mat4(const vec3<T> f) noexcept
            : m{ f[0],  T{}, T{}, T{},
                T{},  f[1],  T{}, T{},
                T{}, T{},  f[2],  T{},
                T{}, T{}, T{},  static_cast<T>(1)} {}

        /**
         * @brief Constructs a new `mat4` with all 16 components explicitly
         * specified.
         * The values are stored in column major order, that is, the first 4 arguments
         * represent the first column, and so on.
         *
         * @param fi_j (i==row, j == col)
         */
        constexpr mat4(
            const T f0_0, const T f1_0, const T f2_0, const T f3_0,
            const T f0_1, const T f1_1, const T f2_1, const T f3_1,
            const T f0_2, const T f1_2, const T f2_2, const T f3_2,
            const T f0_3, const T f1_3, const T f2_3, const T f3_3
        ) : m{
             f0_0,  f1_0,  f2_0,  f3_0,
             f0_1,  f1_1,  f2_1,  f3_1,
             f0_2,  f1_2,  f2_2,  f3_2,
             f0_3,  f1_3,  f2_3,  f3_3
        } { }


        /**
         * @brief Convenient method to construct a 4x4 identity matrix
         *
         * @tparam T The numeric type of the matrix components.
         *
         * @return A new mat4<T>-identity matrix-
         */
        static mat4<T> identity() noexcept {
            return mat4<T>(1);
        }


        /**
         * @brief Provides read-only access to a matrix component.
         * Elements are accessed in column major order: `m[row + col * 4]`.
         * Bounds checking is performed via `assert` in debug builds.
         *
         * @param row The zero based row index.
         * @param col The zero based col index.
         *
         * @return A const ref to the component at the specified position.
         */
        constexpr const T& operator()(const size_t row, const size_t col) const {
            assert(row < 4 && col < 4 && "mat4 - Index out of bounds.");
            return m[row + col * 4];
        }


        /**
         * @brief Provides read-write access to a matrix component.
         * Elements are accessed in column major order: `m[row + col * 4]`.
         * Bounds checking is performed via `assert` in debug builds.
         *
         * @param row The zero based row index.
         * @param col The zero based col index.
         *
         * @return A ref to the component at the specified position.
         */
        constexpr T& operator()(const size_t row, const size_t col) {
            assert(row < 4 && col < 4 && "mat4 - Index out of bounds.");
            return m[row + col * 4];
        }


        /**
         * @brief Compares this matrix element's with the rgt matrix considering
         * an epsilon value.
         * Returns true if for all elements the equation |a-b| <= EPSILON
         * holds.
         *
         * EPSILON is set to 0.00001
         *
         * @param rgt The other matix to compare with this matrix for equality.
         *
         * @return True is the elements of the matrices are considered equal,
         * otherwise false.
         *
         * @see https://realtimecollisiondetection.net/blog/?p=89
         *
         * @todo account for abs (values close to zero) and rel
         * (larger values), move EPSILON to global constant
         */
        constexpr bool same(const mat4<T>& rgt) const {

            static const T EPSILON = static_cast<T>(0.00001);

            const auto* leftPtr = value_ptr(*this);
            const auto* rgtPtr  = value_ptr(rgt);

            for (int i = 0; i < 16; i++) {
                if (std::fabs(leftPtr[i] - rgtPtr[i])  > EPSILON) {
                    return false;
                }
            }

            return true;
        }


        /**
         * @brief Strictly compares the elements of this matrix with the elements
         * of the rgt matrix.
         *
         * @param rgt The right matrix to compare for equal values
         *
         * @return True if all elements are equal (==), false otherwise.
         */
        constexpr bool operator==(const mat4<T>& rgt) const {

            const auto* leftPtr = value_ptr(*this);
            const auto* rgtPtr  = value_ptr(rgt);

            for (int i = 0; i < 16; i++) {
                if (leftPtr[i] != rgtPtr[i]) {
                    return false;
                }
            }

            return true;
        }


        /**
         * @brief Performs matrix-multiplication with another `mat4`.
         * This matrix is the left operand, while `m` is the right operand.
         *
         * @param m The right-hand side `mat4<T>` for multiplication.
         *
         * @return A new `mat4<T>`, representing the result of the matrix-multiplication.
         */
        constexpr mat4<T> operator*(const mat4<T>& m) const {
            mat4<T> A{};
            for (int row = 0; row < 4; row++) {
                for (int col = 0; col < 4; col++) {
                    T sum = T{};
                    sum += (*this)(row, 0) * m(0, col);
                    sum += (*this)(row, 1) * m(1, col);
                    sum += (*this)(row, 2) * m(2, col);
                    sum += (*this)(row, 3) * m(3, col);
                    A(row, col) = sum;
                }
            }

            return A;
        }
    };


    /**
     * @brief Returns a const pointer to the first element of the matrix's components.
     *
     * Useful for APIs that expect a pointer to matrix data, like OpenGL.
     *
     * @tparam T The numeric type of the matrix components.
     *
     * @param m A reference to the `mat4<T>` matrix.
     *
     * @return A const pointer to the element at [0, 0].
     */
    template<helios::math::Numeric T>
    const T* value_ptr(const mat4<T>& m) noexcept {
        return &m(0, 0);
    }


    /**
     * @brief Returns a pointer to the first element of the matrix's components.
     *
     * Useful for APIs that expect a pointer to matrix data, like OpenGL.
     *
     * @tparam T The numeric type of the matrix components.
     *
     * @param m A reference to the `mat4<T>` matrix.
     *
     * @return A pointer to the element at [0, 0].
     */
    template<helios::math::Numeric T>
    T* value_ptr(mat4<T>& m) noexcept {
        return &m(0, 0);
    }

    using mat4f = mat4<float>;
    using mat4d = mat4<double>;
    using mat4i = mat4<int>;

} // namespace helios::math
