#include "gtest/gtest.h"
#include "zeus/math/vector_2d.hpp"

/**
 * Super simple test for 2D vectors.
 */
namespace {

TEST(vector2d_test, initializer_list) {
    Zeus::Math::Vector2D vec = {1, 2};

    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 2);
}

TEST(vector2d_test, copy_initialization) {
    Zeus::Math::Vector2D vec{Zeus::Math::Vector2D{1, 2}};

    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 2);
}

}  // namespace
