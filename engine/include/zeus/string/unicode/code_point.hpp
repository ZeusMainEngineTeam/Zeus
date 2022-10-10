/**
 * This file is part of the Zeus Game Engine.
 * Copyright (C) 2022 Tristan F.
 *
 * The Zeus Game Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * The Zeus Game Engine is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * the Zeus Game Engine. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <compare>
#include <concepts>
#include <optional>
#include <ostream>
#include <string>

#include "zeus/core/types.hpp"
#include "zeus/string/unicode/exception.hpp"

/**
 * @file string/unicode/code_point.hpp
 *
 * Provides a lightweight wrapper for Unicode code points.
 */

namespace Zeus {

namespace Unicode {

// NOLINTNEXTLINE(readability-identifier-naming)
inline namespace cpp20_v1 {

// Forward declare for non-member friend function(s)
class CodePoint;

}  // namespace cpp20_v1

}  // namespace Unicode

// NOLINTNEXTLINE(readability-identifier-naming)
inline namespace cpp20_v1 {

template <std::integral IntegerType>
[[nodiscard]] constexpr auto to_integer(Unicode::CodePoint point) noexcept;

}  // namespace cpp20_v1

namespace Unicode {

// NOLINTNEXTLINE(readability-identifier-naming)
inline namespace cpp20_v1 {

/**
 * A Unicode code point.
 *
 * @note This will **always** be a valid Unicode code point.
 */
class CodePoint {
   public:
    /**
     * The underlying representation of the Unicode code point value.
     */
    using value_type = Zeus::u32;

    /**
     * The maximum possible value for a valid Unicode code point.
     */
    static constexpr value_type g_maxValue = 0x10FFFFU;

    /**
     * The minimum possible value for a valid Unicode code point.
     */
    static constexpr value_type g_minValue = 0x0U;

    /**
     * Default constructor.
     */
    CodePoint() noexcept = default;

    /**
     * Constructs a Unicode code point using the given value.
     *
     * @param value The value to construct the Unicode code point
     *
     * @throws Zeus::Unicode::Exception::InvalidCodePoint if the given value is
     * not within the valid Unicode code point range.
     *
     * @see Zeus::Unicode::CodePoint::g_maxValue
     * @see Zeus::Unicode::CodePoint::g_minValue
     * @see Zeus::Unicode::CodePoint::is_valid
     */
    constexpr explicit CodePoint(value_type value)
        : m_value{CodePoint::validate_value(value)} {}

    /**
     * Constructs an optional Unicode code point using the given value if valid.
     *
     * @param value The value to use to create a Unicode code point
     *
     * @return The Unicode code point if the given value is valid, otherwise
     * std::nullopt
     */
    [[nodiscard]] static constexpr std::optional<CodePoint> create(
        value_type value) noexcept {
        if (CodePoint::is_valid(value)) {
            // Have to construct this way to avoid additional validation check
            CodePoint code_point;
            code_point.m_value = value;

            return code_point;
        }

        return std::nullopt;
    }

    /**
     * CodePoint three-way comparison.
     */
    [[nodiscard]] constexpr auto operator<=>(CodePoint const&) const noexcept =
        default;

    /**
     * Compares this Unicode code point against the given code point.
     */
    [[nodiscard]] constexpr bool operator==(CodePoint const&) const noexcept =
        default;

    /**
     * Compares this Unicode code point against the given value.
     *
     * @param value The value to compare against this Unicode code point
     *
     * @return The strong ordering comparison between the given value and this
     * Unicode code point
     */
    [[nodiscard]] constexpr auto operator<=>(value_type value) const noexcept {
        return this->m_value <=> value;
    }

    /**
     * Compares this Unicode code point against the given value.
     *
     * @param value The value to compare to this Unicode code point
     *
     * @return True if equal, otherwise false
     */
    [[nodiscard]] constexpr auto operator==(value_type value) const noexcept {
        return this->m_value == value;
    }

    /**
     * Converts the given Unicode code point to the given IntegerType.
     *
     * @tparam IntegerType The integer type to convert to
     *
     * @param point The code point to convert
     *
     * @return An IntegerType representation of the given code point
     */
    template <std::integral IntegerType>
    friend constexpr auto Zeus::cpp20_v1::to_integer(CodePoint point) noexcept;

    /**
     * Checks if the given value is in range of a valid Unicode code point.
     *
     * @param point The value for a Unicode code point to check
     *
     * @return True if the given value is within the valid Unicode code point
     * range, otherwise false
     */
    [[nodiscard]] static constexpr bool is_valid(value_type point) noexcept {
        return point <= CodePoint::g_maxValue && point >= CodePoint::g_minValue;
    }

   private:
    /**
     * The value containing the Unicode code point.
     *
     * @note Default is the same as a Null Unicode code point (U+0000).
     */
    value_type m_value{};

    /**
     * Helper to validate the Unicode code point value.
     *
     * @param value The value for the Unicode code point
     *
     * @throws Zeus::Unicode::Exception::InvalidCodePoint if the given value is
     * not in the Unicode code point range
     *
     * @return The given value
     */
    [[nodiscard]] static constexpr value_type validate_value(value_type value) {
        if (CodePoint::is_valid(value)) [[likely]] {
            return value;
        }

        using Unicode::Exception::InvalidCodePoint;
        throw InvalidCodePoint{"Invalid Unicode code point value."};
    }
};

}  // namespace cpp20_v1

}  // namespace Unicode

// NOLINTNEXTLINE(readability-identifier-naming)
inline namespace cpp20_v1 {

/**
 * Converts the given Unicode code point to the given IntegerType.
 *
 * @tparam IntegerType The integer type to convert to
 *
 * @param point The code point to convert
 *
 * @return An IntegerType representation of the given code point
 */
template <std::integral IntegerType>
[[nodiscard]] constexpr auto to_integer(Unicode::CodePoint point) noexcept {
    return static_cast<IntegerType>(point.m_value);
}

/**
 * Converts the given Unicode code point to its underlying type.
 *
 * @param point The code point to convert
 *
 * @return The converted Unicode code point
 */
[[nodiscard]] constexpr auto to_integer(Unicode::CodePoint point) noexcept {
    return Zeus::to_integer<Unicode::CodePoint::value_type>(point);
}

/**
 * Converts the given Unicode code point into a string representation.
 *
 * @example U+006E
 *
 * @param point The Unicode code point to convert
 *
 * @return The string representation
 */
[[nodiscard]] std::string to_string(Unicode::CodePoint point);

/**
 * Sends the given Unicode code point in a string representation to the given
 * stream.
 *
 * @param stream The output stream for the Unicode code point
 * @param point  The Unicode code point to send to the given stream
 *
 * @return A reference to the given stream
 */
std::ostream& operator<<(std::ostream& stream, Unicode::CodePoint point);

}  // namespace cpp20_v1

}  // namespace Zeus
