module;

#include <cassert>
#include <utility>

export module helios.math.types:mat4;

import helios.math.concepts;

export namespace helios::math {

    /**
     * Represents a 4x4 matrix, stored in column-major order.
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
         * Internal array storing matrix components.
         *
         * Components are stored in column-major order, that is, each 4 subsequent elements
         * represent a matrix column.
         */
        T m[16];

    public:
        /**
         * Default constructor. Initializes all components to 0.
         */
        explicit constexpr mat4() noexcept : m{}{};

        /**
         * Creates a diagonal matrix. The diagonal components are initialized
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
         * Constructs a new `mat4` with all 16 components explicitly
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
         * Provides read-only access to a matrix component.
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
         * Provides read-write access to a matrix component.
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
         * Performs matrix-multiplication with another `mat4`.
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
                    float sum = 0.0f;
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
     * Returns a const pointer to the first element of the matrix's components.
     *
     * Useful for APIs that expect a pointer to matrix data, like OpenGL.
     *
     * @tparam T The numeric type of the matrix components.
     *
     * @param m A reference to the `mat4<T>` matrix.
     *
     * @return A const pointer to the element at [0, 0]
     */
    template<helios::math::Numeric T>
    const float* value_ptr(const mat4<T>& m) noexcept {
        return &m(0, 0);
    }


    /**
     * Returns a pointer to the first element of the matrix's components.
     *
     * Useful for APIs that expect a pointer to matrix data, like OpenGL.
     *
     * @tparam T The numeric type of the matrix components.
     *
     * @param m A reference to the `mat4<T>` matrix.
     *
     * @return A pointer to the element at [0, 0]
     */
    template<helios::math::Numeric T>
    float* value_ptr(mat4<T>& m) noexcept {
        return &m(0, 0);
    }

    using mat4f = mat4<float>;
    using mat4d = mat4<double>;
    using mat4i = mat4<int>;

}
