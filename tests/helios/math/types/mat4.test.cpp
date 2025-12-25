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

TEST(Mat4TransposeTests, transposeIdentityMatrixRemainsIdentity) {
    math::mat4f identity = math::mat4f::identity();
    math::mat4f transposed = identity.transpose();

    EXPECT_TRUE(transposed == identity);
}

TEST(Mat4TransposeTests, transposeSwapsRowsAndColumns) {
    // Column-major: first 4 args = column 0, next 4 = column 1, etc.
    // Matrix layout (row, col):
    //   [0,0]=1  [0,1]=5  [0,2]=9   [0,3]=13
    //   [1,0]=2  [1,1]=6  [1,2]=10  [1,3]=14
    //   [2,0]=3  [2,1]=7  [2,2]=11  [2,3]=15
    //   [3,0]=4  [3,1]=8  [3,2]=12  [3,3]=16
    math::mat4f M{
        1, 2, 3, 4,     // column 0
        5, 6, 7, 8,     // column 1
        9, 10, 11, 12,  // column 2
        13, 14, 15, 16  // column 3
    };

    math::mat4f T = M.transpose();

    // After transpose: T(i,j) = M(j,i)
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            EXPECT_FLOAT_EQ(T(row, col), M(col, row));
        }
    }
}

TEST(Mat4TransposeTests, transposeOfTransposeReturnsOriginal) {
    math::mat4f M{
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16
    };

    math::mat4f T = M.transpose().transpose();

    EXPECT_TRUE(T == M);
}

TEST(Mat4TransposeTests, transposeSymmetricMatrixRemainsUnchanged) {
    // Symmetric matrix: M(i,j) = M(j,i)
    math::mat4f symmetric{
        1, 2, 3, 4,   // column 0: [1,0]=2, [2,0]=3, [3,0]=4
        2, 5, 6, 7,   // column 1: [0,1]=2, [2,1]=6, [3,1]=7
        3, 6, 8, 9,   // column 2: [0,2]=3, [1,2]=6, [3,2]=9
        4, 7, 9, 10   // column 3: [0,3]=4, [1,3]=7, [2,3]=9
    };

    math::mat4f transposed = symmetric.transpose();

    EXPECT_TRUE(transposed == symmetric);
}

TEST(Mat4TransposeTests, transposePreservesSpecificElementPositions) {
    math::mat4f M{
        1, 2, 3, 4,
        5, 6, 7, 8,
        9, 10, 11, 12,
        13, 14, 15, 16
    };

    math::mat4f T = M.transpose();

    // Verify specific transposed positions
    EXPECT_FLOAT_EQ(T(0, 1), M(1, 0));  // was 2, now at [0,1]
    EXPECT_FLOAT_EQ(T(0, 3), M(3, 0));  // was 4, now at [0,3]
    EXPECT_FLOAT_EQ(T(1, 0), M(0, 1));  // was 5, now at [1,0]
    EXPECT_FLOAT_EQ(T(2, 0), M(0, 2));  // was 9, now at [2,0]
    EXPECT_FLOAT_EQ(T(3, 0), M(0, 3));  // was 13, now at [3,0]
}

TEST(Mat4TransposeTests, transposeDiagonalMatrixRemainsUnchanged) {
    math::mat4f diagonal{
        2, 0, 0, 0,
        0, 3, 0, 0,
        0, 0, 4, 0,
        0, 0, 0, 5
    };

    math::mat4f transposed = diagonal.transpose();

    EXPECT_TRUE(transposed == diagonal);
}

