#include "gtest/gtest.h"
#include "zeus/math/matrix.hpp"

/**
 * Super simple test for matrices.
 */
namespace {

TEST(matrix_test, simple_test) {
    Zeus::Math::Matrix <Zeus::i64, 3, 4> mat;
    

    EXPECT_EQ(mat.size(), 12);
}

}  // namespace
