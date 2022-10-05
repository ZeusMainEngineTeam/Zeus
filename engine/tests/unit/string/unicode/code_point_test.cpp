#include "gtest/gtest.h"

#include "zeus/string/unicode/code_point.hpp"

/**
 * Tests for zeus/string/unicode/code_point.hpp
 */
namespace {

constexpr Zeus::Unicode::CodePoint g_maxCodePoint{
    Zeus::Unicode::CodePoint::g_maxValue};
constexpr Zeus::Unicode::CodePoint g_minCodePoint{
    Zeus::Unicode::CodePoint::g_minValue};
constexpr Zeus::Unicode::CodePoint g_zeroCodePoint{0};

TEST(CodePoint, defaultConstructor) {
    using Zeus::Unicode::CodePoint;

    constexpr CodePoint zero_code_point{};
    auto constexpr expected{0x0};
    
    ASSERT_EQ(zero_code_point, expected);
}

TEST(CodePoint, valueConstructorSuccess) {
    using Zeus::Unicode::CodePoint;

    ASSERT_EQ(g_maxCodePoint, CodePoint::g_maxValue);
    ASSERT_EQ(g_minCodePoint, CodePoint::g_minValue);
    ASSERT_EQ(g_zeroCodePoint, 0);
}

TEST(CodePoint, valueConstructorFailure) {
    using Zeus::Unicode::CodePoint;

    // Test edge cases
    EXPECT_THROW(CodePoint(CodePoint::g_maxValue + 1), Zeus::Unicode::Exception);
    EXPECT_THROW(CodePoint(-1), Zeus::Unicode::Exception);
}

TEST(CodePoint, copyConstructor) {
    using Zeus::Unicode::CodePoint;

    ASSERT_EQ(g_maxCodePoint, CodePoint{g_maxCodePoint});
    ASSERT_EQ(g_minCodePoint, CodePoint{g_minCodePoint});
    ASSERT_EQ(g_zeroCodePoint, CodePoint{g_zeroCodePoint});
}

TEST(CodePoint, createSuccess) {
    using Zeus::Unicode::CodePoint;

    auto constexpr max_code_point = CodePoint::create(CodePoint::g_maxValue);
    auto constexpr min_code_point = CodePoint::create(CodePoint::g_minValue);
    auto constexpr zero_code_point = CodePoint::create(0);

    ASSERT_TRUE(max_code_point.has_value());
    ASSERT_TRUE(min_code_point.has_value());
    ASSERT_TRUE(zero_code_point.has_value());

    EXPECT_EQ(max_code_point.value(), g_maxCodePoint);
    EXPECT_EQ(min_code_point.value(), g_minCodePoint);
    EXPECT_EQ(zero_code_point.value(), g_zeroCodePoint);
}

TEST(CodePoint, createFailure) {
    using Zeus::Unicode::CodePoint;

    auto constexpr over_constrained_max =
        CodePoint::create(CodePoint::g_maxValue + 1);
    auto constexpr negative_value = CodePoint::create(-1);

    EXPECT_FALSE(over_constrained_max.has_value());
    EXPECT_FALSE(negative_value.has_value());
}

TEST(CodePoint, copyAssignmentOperator) {
    using Zeus::Unicode::CodePoint;

    // Just initialize with random values
    CodePoint current_max_code_point{4};
    CodePoint current_min_code_point{23};
    CodePoint current_zero_code_point{32};

    // Try to enforce copy assignment operator
    {
        current_max_code_point = g_maxCodePoint;
        current_min_code_point = g_minCodePoint;
        current_zero_code_point = g_zeroCodePoint;
    }

    ASSERT_EQ(current_max_code_point, g_maxCodePoint);
    ASSERT_EQ(current_min_code_point, g_minCodePoint);
    ASSERT_EQ(current_zero_code_point, g_zeroCodePoint);
}

TEST(CodePoint, toInteger) {
    using Zeus::Unicode::CodePoint;
    using value_type = CodePoint::value_type;

    ASSERT_EQ(Zeus::to_integer<value_type>(g_maxCodePoint),
              CodePoint::g_maxValue);
    ASSERT_EQ(Zeus::to_integer<value_type>(g_minCodePoint),
              CodePoint::g_minValue);
    ASSERT_EQ(Zeus::to_integer<value_type>(g_zeroCodePoint), 0);
}

TEST(CodePoint, CodePoints) {
    using Zeus::Unicode::CodePoint;
    namespace CodePoints = Zeus::Unicode::CodePoints;

    using value_type = CodePoint::value_type;

    ASSERT_EQ(CodePoints::g_max, CodePoint::g_maxValue);
    ASSERT_EQ(CodePoints::g_min, CodePoint::g_minValue);
    ASSERT_EQ(CodePoints::g_replacementCharacter, 0xFFFDU);
}

}  // namespace
