module;

#include <cassert>

export module helios.math.core:mat4;

export namespace helios::math {

    /**
     * Column-major mat4 implementation.
     */
    struct mat4 {
    private:
        float m[16];

    public:
        explicit constexpr mat4() noexcept : m{}{};

        explicit constexpr mat4(const float f) noexcept
            : m{f, 0, 0, 0, 0, f, 0, 0, 0, 0, f, 0, 0, 0, 0, f} {}

        /**
         * fi_j (i==row, j == col)
         */
        constexpr mat4(
            const float f0_0, const float f1_0, const float f2_0, const float f3_0,
            const float f0_1, const float f1_1, const float f2_1, const float f3_1,
            const float f0_2, const float f1_2, const float f2_2, const float f3_2,
            const float f0_3, const float f1_3, const float f2_3, const float f3_3
        ) : m{
             f0_0,  f1_0,  f2_0,  f3_0,
             f0_1,  f1_1,  f2_1,  f3_1,
             f0_2,  f1_2,  f2_2,  f3_2,
             f0_3,  f1_3,  f2_3,  f3_3
        } { }

        constexpr float& operator()(const size_t row, const size_t col) {
            assert(row < 4 && col < 4 && "mat4 - Index out of bounds.");
            return m[row + col * 4];
        }

        constexpr const float& operator()(const size_t row, const size_t col) const {
            assert(row < 4 && col < 4 && "mat4 - Index out of bounds.");
            return m[row + col * 4];
        }

        constexpr mat4 operator*(const mat4& matrix) const {
            mat4 A{};
            for (int row = 0; row < 4; row++) {
                for (int col = 0; col < 4; col++) {
                    float sum = 0.0f;
                    sum += (*this)(row, 0) * matrix(0, col);
                    sum += (*this)(row, 1) * matrix(1, col);
                    sum += (*this)(row, 2) * matrix(2, col);
                    sum += (*this)(row, 3) * matrix(3, col);
                    A(row, col) = sum;
                }
            }

            return A;
        }
    };

    const float* value_ptr(const mat4& m) noexcept {
        return &m(0, 0);
    }

    float* value_ptr(mat4& m) noexcept {
        return &m(0, 0);
    }

}
