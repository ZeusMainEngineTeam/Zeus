#include "gtest/gtest.h"

#include <algorithm>
#include <string_view>

#include "zeus/string/utf8/code_unit.hpp"

/**
 * Tests for zeus/string/utf8/code_unit.hpp
 */
namespace {

TEST(CodeUnit, globals) {
    EXPECT_EQ(Zeus::UTF8::g_minLeadingByte, 0);
    EXPECT_EQ(Zeus::UTF8::g_maxLeadingByte, 0b1111'0111);
}

TEST(CodeUnit, is_ascii) {
    using Zeus::UTF8::CodeUnit;
    using Zeus::UTF8::is_ascii;

    constexpr auto min_ascii = 0;
    constexpr auto max_ascii = 127;

    for (auto i = min_ascii; i <= max_ascii; ++i) {
        auto const code_unit = static_cast<CodeUnit>(i);
        ASSERT_TRUE(is_ascii(code_unit));
    }

    constexpr auto min_invalid_ascii = 0x80U;
    constexpr auto max_invalid_ascii = 0xFFU;

    for (auto i = min_invalid_ascii; i <= max_invalid_ascii; ++i) {
        auto const code_unit = static_cast<CodeUnit>(i);
        ASSERT_FALSE(is_ascii(code_unit));
    }
}

TEST(CodeUnit, is_not_ascii) {
    using Zeus::UTF8::CodeUnit;
    using Zeus::UTF8::is_not_ascii;

    constexpr auto min_ascii = 0;
    constexpr auto max_ascii = 127;

    for (auto i = min_ascii; i <= max_ascii; ++i) {
        auto const code_unit = static_cast<CodeUnit>(i);
        ASSERT_FALSE(is_not_ascii(code_unit));
    }

    constexpr auto min_invalid_ascii = 0x80U;
    constexpr auto max_invalid_ascii = 0xFFU;

    for (auto i = min_invalid_ascii; i <= max_invalid_ascii; ++i) {
        auto const code_unit = static_cast<CodeUnit>(i);
        ASSERT_TRUE(is_not_ascii(code_unit));
    }
}

TEST(CodeUnit, is_leading_byte) {
    using Zeus::UTF8::is_leading_byte;

    // ASCII
    EXPECT_TRUE(is_leading_byte(0b0000'0000));  // First
    EXPECT_TRUE(is_leading_byte(0b0111'1111));  // Last
    EXPECT_FALSE(is_leading_byte(0b1000'0000)); // Invalid

    // Byte 2
    EXPECT_TRUE(is_leading_byte(0b1100'0000)); // First
    EXPECT_TRUE(is_leading_byte(0b1101'1111)); // Last

    // Byte 3
    EXPECT_TRUE(is_leading_byte(0b1110'0000)); // First
    EXPECT_TRUE(is_leading_byte(0b1110'1111)); // Last

    // Byte 4
    EXPECT_TRUE(is_leading_byte(0b1111'0000)); // First
    EXPECT_TRUE(is_leading_byte(0b1111'0111)); // Last
}

TEST(CodeUnit, leading_byte_size) {
    using Zeus::UTF8::CodeUnit;
    using Zeus::UTF8::leading_byte_size;

    constexpr auto min_ascii = 0;
    constexpr auto max_ascii = 127;

    for (auto i = min_ascii; i <= max_ascii; ++i) {
        auto const code_unit = static_cast<CodeUnit>(i);
        EXPECT_EQ(leading_byte_size(code_unit), 1);
    }

    constexpr auto min_invalid_ascii = 0x80U;
    constexpr auto max_invalid_ascii = 0xFFU;

    for (auto i = min_invalid_ascii; i <= max_invalid_ascii; ++i) {
        auto const code_unit = static_cast<CodeUnit>(i);

        // Need to use tmp for nodiscard
        EXPECT_THROW(auto tmp = leading_byte_size(code_unit),
                     std::invalid_argument);
    }
}

}  // namespace
