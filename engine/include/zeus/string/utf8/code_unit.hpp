/**
 * This file is part of the Zeus Game Engine.
 * Copyright (C) 2022 Tristan F.
 *
 * The Zeus Game Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * The Zeus Game Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * the Zeus Game Engine. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <optional>
#include <stdexcept>

#include "zeus/core/types.hpp"
#include "zeus/utility/concepts.hpp"

/**
 * Contains ways to inspect a single UTF-8 code unit.
 *
 * @file string/utf8/code_unit.hpp
 */

namespace Zeus::UTF8{

// NOLINTNEXTLINE(readability-identifier-naming)
inline namespace cpp20_v1 {

/**
 * A UTF-8 code unit.
 */
using CodeUnit = char8_t;

template <typename Range>
concept code_unit_input_range = Zeus::input_range_of<Range, CodeUnit>;

template <typename Iterator>
concept code_unit_input_iterator = Zeus::input_iterator_of<Iterator, CodeUnit>;

// =============================== INSPECTING =============================== //

/**
 * Checks if the given code unit is ASCII.
 *
 * @param code_unit The code unit to check
 *
 * @return True if ASCII otherwise false
 */
[[nodiscard]] constexpr bool is_ascii(CodeUnit code_unit) noexcept {
    // Check if format is in 0xxx xxxx
    // NOLINTNEXTLINE(*-magic-numbers)
    return (code_unit & 0x80U) == 0x00U;
}

/**
 * Checks if the given code unit is *not* ASCII.
 *
 * @param code_unit The code unit to check
 *
 * @return True if not ASCII otherwise false
 */
[[nodiscard]] constexpr bool is_not_ascii(CodeUnit code_unit) noexcept {
    return !is_ascii(code_unit);
}

/**
 * Returns the *possible* size of the UTF-8 character based on the first
 * byte/code unit.
 *
 * @note This does not mean the following bytes are continuation bytes, if there
 * are any, will be valid.
 *
 * @param code_unit The UTF-8 code unit to check
 *
 * @return The size of the UTF-8 character based on the code unit if valid,
 * otherwise std::nullopt
 */
[[nodiscard]] constexpr std::optional<Zeus::ssize> peek_char_size(
    CodeUnit code_unit) noexcept {
    if (is_ascii(code_unit))            return 1;
    if ((code_unit & 0xE0U) == 0xC0U)   return 2;
    if ((code_unit & 0xF0U) == 0xE0U)   return 3;
    if ((code_unit & 0xF8U) == 0xF0U)   return 4;
    
    return std::nullopt;
}

/**
 * Returns the size of the leading code unit byte.
 *
 * @precondition The given code unit is a leading byte.
 *
 * @throws std::invalid_argument if the given code unit is not a leading byte
 * code unit.
 */
[[nodiscard]] constexpr Zeus::i8 leading_byte_size(CodeUnit code_unit) {
    // NOLINTNEXTLINE(*-magic-numbers)
    if (is_ascii(code_unit))            return 1;
    if ((code_unit & 0xE0U) == 0xC0U)   return 2;
    if ((code_unit & 0xF0U) == 0xE0U)   return 3;
    if ((code_unit & 0xF8U) == 0xF0U)   return 4;

    // TODO(tristan): Replace with ZEUS_PRECONDITION
    // Programmer error - throw exception
    throw std::invalid_argument{
        "The given UTF-8 code unit is not a leading byte."};
}

[[nodiscard]] constexpr bool is_leading_byte(CodeUnit code_unit) noexcept {
    return Zeus::UTF8::peek_char_size(code_unit).has_value();
}

/**
 * Checks if the given code unit is a continuation byte.
 *
 * @note A continuation byte has the following format: 10xx xxxx
 *
 * @param code_unit The code unit to check
 *
 * @return True if a continuation byte, otherwise false
 */
[[nodiscard]] constexpr bool is_continuation_byte(CodeUnit code_unit) noexcept {
    // NOLINTNEXTLINE(*-magic-numbers)
    return (code_unit & 0xC0U) == 0x80U;
}

/**
 * Checks if the given code unit is a valid UTF-8 character code unit.
 *
 * @param code_unit The code unit to check
 *
 * @return True if the given code unit is a valid UTF-8 character code unit
 * otherwise false
 */
[[nodiscard]] constexpr bool is_valid_byte(CodeUnit code_unit) noexcept {
    return Zeus::UTF8::is_continuation_byte(code_unit) ||
           Zeus::UTF8::is_leading_byte(code_unit);
}

/**
 * Checks if the given code unit is an invalid UTF-8 code unit.
 *
 * @param code_unit The code unit to check
 *
 * @return True if the given code unit is an invalid UTF-8 character code unit
 * otherwise false
 */
[[nodiscard]] constexpr bool is_invalid_byte(CodeUnit code_unit) noexcept {
    return !is_valid_byte(code_unit);
}

}  // namespace cpp20_v1

}  // namespace Zeus::UTF8
