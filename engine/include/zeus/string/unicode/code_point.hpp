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

#include <concepts>
#include <optional>
#include <string>

#include "zeus/core/types.hpp"
#include "zeus/string/unicode/exception.hpp"

/**
 * @file string/unicode/code_point.hpp
 *
 * Provides a lightweight support wrapper for Unicode code points.
 */

namespace Zeus {

namespace Unicode {

/**
 * A Unicode code point.
 *
 * @note This will be a valid Unicode code point.
 */
class CodePoint {
   public:
    /**
     * The fundamental representation of the Unicode code point value.
     */
    using value_type = Zeus::u32;

    /**
     * The maximum possible value for a valid Unicode code point.
     */
    static constexpr value_type g_rawMax = 0x10FFFFU;

    /**
     * The minimum possible value for a valid Unicode code point.
     */
    static constexpr value_type g_rawMin = 0x0U;

    /**
     * Default constructor.
     */
    CodePoint() noexcept = default;

    /**
     * Constructs a Unicode code point using the given value.
     *
     * @param value The value to construct the Unicode code point
     *
     * @throws Zeus::Unicode::Exception if the given value is not within the
     * valid Unicode code point range.
     *
     * @see Zeus::Unicode::CodePoint::g_rawMax
     * @see Zeus::Unicode::CodePoint::g_rawMin
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
    [[nodiscard]] static std::optional<CodePoint> create(
        value_type value) noexcept {
        if (CodePoint::is_valid(value)) {
            return CodePoint{value};
        }

        return std::nullopt;
    }

    /**
     * Default CodePoint three-way comparison with strong_ordering.
     */
    [[nodiscard]] constexpr auto operator<=>(CodePoint const&) const noexcept =
        default;

    /**
     * The three-way comparison with strong_ordering for the fundamental
     * representation of a Unicode code point.
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
     * Converts the given Unicode code point to the IntegerType.
     *
     * @tparam IntegerType The integer type to convert to
     *
     * @param point The code point to convert
     *
     * @return An IntegerType representation of the given code point
     */
    template <std::integral IntegerType>
    friend constexpr auto to_integer(Zeus::Unicode::CodePoint point) noexcept;

    /**
     * The maximum Unicode code point.
     */
    static CodePoint const g_max;

    /**
     * The minimum Unicode code point.
     */
    static CodePoint const g_min;

    /**
     * The default replacement character for replacing a invalid UTF-8
     * character.
     */
    static CodePoint const g_replacementCharacter;

    /**
    * Checks if the given value is in range of a valid Unicode code point.
    *
    * @param point The value for a Unicode code point to check
    *
    * @return True if the given value is within the valid Unicode code point range,
    * otherwise false
    */
    [[nodiscard]] static constexpr bool is_valid(value_type point) noexcept {
        return point <= CodePoint::g_rawMax && point >= CodePoint::g_rawMin;
    }

   private:
    /**
     * The value containing the Unicode code point.
     *
     * @note This invariant will always be a valid Unicode code point.
     */
    value_type m_value{};

    /**
     * Helper to validate the Unicode code point value.
     *
     * @param value The value for the Unicode code point
     *
     * @throws Zeus::Unicode::Exception if the given value is not in the Unicode
     * code point range
     *
     * @return The given value
     */
    [[nodiscard]] static constexpr value_type validate_value(value_type value) {
        if (!CodePoint::is_valid(value)) {
            throw Zeus::Unicode::Exception{"Invalid Unicode code point value."};
        }

        return value;
    }
};

template <std::integral IntegerType>
[[nodiscard]] constexpr auto to_integer(CodePoint point) noexcept {
    return IntegerType(point.m_value);
}

}  // namespace Unicode

/**
 * Converts the given Unicode code point to the IntegerType.
 *
 * @tparam IntegerType The integer type to convert to
 *
 * @param point The code point to convert
 *
 * @return An IntegerType representation of the given code point
*/
template <std::integral IntegerType>
[[nodiscard]] constexpr auto to_integer(
    Zeus::Unicode::CodePoint point) noexcept {
    return Zeus::Unicode::to_integer<IntegerType>(point);
}

/**
 * Converts the given Unicode code point into a string representation.
 *
 * @param point The Unicode code point to convert
 *
 * @return The string representation
 */
[[nodiscard]] std::string to_string(Zeus::Unicode::CodePoint point) noexcept;

}  // namespace Zeus
