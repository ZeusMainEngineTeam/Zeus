#include "gtest/gtest.h"

#include "zeus/string/unicode/code_point.hpp"

/**
 * Tests for zeus/string/unicode/code_point.hpp
 */
namespace {

TEST(code_point_test, default_constructor) {
    using Zeus::Unicode::CodePoint;

    constexpr CodePoint zero_code_point{};

    auto const actual{Zeus::to_integer<CodePoint::value_type>(zero_code_point)};
    auto const expected{0};
    
    ASSERT_EQ(actual, expected);
}

}  // namespace
