#include "gtest/gtest.h"

#include "zeus/string/unicode/code_point.hpp"
#include "zeus/string/unicode/code_points.hpp"

/**
 * Tests for zeus/string/unicode/code_points.hpp
 */
namespace {

TEST(CodePoint, CodePoints) {
    namespace CodePoints = Zeus::Unicode::CodePoints;
    using Zeus::Unicode::CodePoint;

    ASSERT_EQ(CodePoints::g_max, CodePoint::g_maxValue);
    ASSERT_EQ(CodePoints::g_min, CodePoint::g_minValue);
    ASSERT_EQ(CodePoints::g_replacementCharacter, 0xFFFDU);
    ASSERT_EQ(CodePoints::g_null, 0);
}

}  // namespace
