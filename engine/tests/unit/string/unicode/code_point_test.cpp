#include "gtest/gtest.h"

#include <type_traits>
#include <sstream>

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

    EXPECT_EQ(current_max_code_point, g_maxCodePoint);
    EXPECT_EQ(current_min_code_point, g_minCodePoint);
    EXPECT_EQ(current_zero_code_point, g_zeroCodePoint);
}

// ============================= FREE FUNCTIONS ============================= //

TEST(CodePoint, isValid) {
    using Zeus::Unicode::CodePoint;

    EXPECT_TRUE(CodePoint::is_valid(0));
    EXPECT_TRUE(CodePoint::is_valid(CodePoint::g_minValue));
    EXPECT_TRUE(CodePoint::is_valid(CodePoint::g_maxValue));

    EXPECT_TRUE(CodePoint::is_valid(1));
    EXPECT_TRUE(CodePoint::is_valid(CodePoint::g_minValue + 1));
    EXPECT_TRUE(CodePoint::is_valid(CodePoint::g_maxValue - 1));

    EXPECT_FALSE(CodePoint::is_valid(-1));
    EXPECT_FALSE(CodePoint::is_valid(CodePoint::g_minValue - 1));
    EXPECT_FALSE(CodePoint::is_valid(CodePoint::g_maxValue + 1));
}

TEST(CodePoint, toInteger) {
    using Zeus::Unicode::CodePoint;
    using value_type = CodePoint::value_type;

    EXPECT_EQ(Zeus::to_integer<value_type>(g_maxCodePoint),
              CodePoint::g_maxValue);
    EXPECT_EQ(Zeus::to_integer<value_type>(g_minCodePoint),
              CodePoint::g_minValue);
    EXPECT_EQ(Zeus::to_integer<value_type>(g_zeroCodePoint), 0);

    EXPECT_EQ(Zeus::to_integer(g_maxCodePoint), CodePoint::g_maxValue);
    EXPECT_EQ(Zeus::to_integer(g_minCodePoint), CodePoint::g_minValue);
    EXPECT_EQ(Zeus::to_integer(g_zeroCodePoint), 0);
}

TEST(CodePoint, toString) {
    using Zeus::Unicode::CodePoint;

    auto constexpr latin_small_letter_N = CodePoint{0x006EU};

    EXPECT_EQ(Zeus::to_string(latin_small_letter_N), "U+006E");
    EXPECT_EQ(Zeus::to_string(g_maxCodePoint), "U+10FFFF");
    EXPECT_EQ(Zeus::to_string(g_minCodePoint), "U+0");
}

TEST(CodePoint, ostream) {
    using Zeus::cpp20_v1::operator<<;
    using Zeus::Unicode::CodePoint;

    std::ostringstream stream;

    {
        auto constexpr latin_small_letter_n = CodePoint{0x006EU};
        stream << latin_small_letter_n;

        EXPECT_EQ(stream.str(), "U+006E");
        stream.str("");
    }

    {
        stream << g_maxCodePoint;
        EXPECT_EQ(stream.str(), "U+10FFFF");
        stream.str("");
    }

    {
        stream << g_maxCodePoint;
        EXPECT_EQ(stream.str(), "U+0");
        stream.str("");
    }
}

}  // namespace
