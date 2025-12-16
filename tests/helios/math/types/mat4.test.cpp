#include <gtest/gtest.h>
#include <numbers>
import helios.math;

namespace math = helios::math;

TEST(Mat4Tests, defaultConstructor) {

    math::mat4f M {};

    const float* m = math::value_ptr(M);

    for (int i = 0; i < 16; i++) {
        EXPECT_FLOAT_EQ(0.0f, m[i]);
    }

}

TEST(Mat4Tests, accessor) {

    math::mat4f M{
        0, 1, 2, 3,
        4, 5, 6, 7,
        8, 9, 10, 11,
        12, 13, 14, 15
    };

    EXPECT_FLOAT_EQ(4.0f, M(0, 1));
    EXPECT_FLOAT_EQ(14.0f, M(2, 3));


#ifdef HELIOS_DEBUG
    EXPECT_DEATH(M(-3, 1), "mat4 - Index out of bounds.");
    EXPECT_DEATH(M(1, 4), "mat4 - Index out of bounds.");
#endif

}

TEST(Mat4Tests, multiply) {
    math::mat4f M{
        0, 1, 2, 3,
        4, 5, 6, 7,
        8, 9, 10, 11,
        12, 13, 14, 15
    };

    math::mat4f A{
        0, 1, 2, 3,
        4, 5, 6, 7,
        8, 9, 10, 11,
        12, 13, 14, 15
    };

    math::mat4f B = A * M;

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            float sum = 0.0f;
            sum += A(row, 0) * M(0, col);
            sum += A(row, 1) * M(1, col);
            sum += A(row, 2) * M(2, col);
            sum += A(row, 3) * M(3, col);
            EXPECT_FLOAT_EQ(sum, B(row, col));
        }
    }

}


TEST(Mat4Tests, multiplyVector) {
    math::mat4f M{
        0, 1, 2, 3,
        4, 5, 6, 7,
        8, 9, 10, 11,
        12, 13, 14, 15
    };

    math::vec4f v{1, 2, 3, 1};

    math::vec4f B = M*v;

    for (int row = 0; row < 4; row++) {
        float sum = 0.0f;
        for (int col = 0; col < 4; col++) {
            sum += M(row, col) * v[col];
        }
        EXPECT_FLOAT_EQ(sum, B[row]);
    }

}

TEST(Mat4Same, same) {
    math::mat4f M{
        0, 1, 2, 3,
        4, 5, 6, 7,
        8, 9, 10, 11,
        12, 13, 14, 15
    };

    math::mat4f A{
        0, 1, 2, 3,
        4, 5, 6, 7,
        8, 9, 10, 11,
        12, 13, 14, 15
    };

    math::mat4f B{
        0, 1.00004f, 2, 3,
        4, 5, 6, 7,
        8, 9, 10, 11,
        12, 13, 14, 15
    };

    math::mat4f C{
        0, 1.000004f, 2, 3,
        4, 5, 6, 7,
        8, 9, 10, 11,
        12, 13, 14, 15
    };


    EXPECT_TRUE(A == M);
    EXPECT_TRUE(A.same(M));

    EXPECT_FALSE(B == M);
    EXPECT_FALSE(B.same(M));

    EXPECT_FALSE(B == M);
    EXPECT_TRUE(C.same(M));

}