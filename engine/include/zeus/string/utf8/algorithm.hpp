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

#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>

#include "zeus/string/unicode/code_point.hpp"
#include "zeus/string/utf8/code_unit.hpp"
#include "zeus/string/utf8/iterator.hpp"

namespace Zeus::UTF8 {

// NOLINTNEXTLINE(readability-identifier-naming)
inline namespace cpp20_v1 {

// =============================== VALIDATION =============================== //

template <Zeus::UTF8::code_unit_input_iterator InputIt,
          std::sentinel_for<InputIt> Sentinel>
[[nodiscard]] constexpr bool is_valid_character(InputIt first, Sentinel last) {
    auto const count_opt = Zeus::UTF8::peek_char_size(*first);
    auto const distance = std::ranges::distance(first, last);

    return (count_opt.has_value() && distance == count_opt) &&
           (Zeus::UTF8::is_ascii(*first) ||
            std::ranges::all_of(++first, last,
                                Zeus::UTF8::is_continuation_byte));
}

[[nodiscard]] constexpr bool is_valid_character(
    Zeus::UTF8::code_unit_input_range auto&& range) {
    return Zeus::UTF8::is_valid_character(std::ranges::begin(range),
                                          std::ranges::end(range));
}

template <Zeus::UTF8::code_unit_input_iterator InputIt,
          std::sentinel_for<InputIt> Sentinel>
[[nodiscard]] constexpr bool starts_with_valid_character(InputIt first,
                                                         Sentinel last) {
    auto const count_opt = Zeus::UTF8::peek_char_size(*first);
    auto const distance = std::distance(first, last);

    return (count_opt.has_value() && distance >= count_opt) &&
           (Zeus::UTF8::is_ascii(*first) ||
            std::ranges::all_of(++first, last,
                                Zeus::UTF8::is_continuation_byte));
}

[[nodiscard]] constexpr bool starts_with_valid_character(
    Zeus::UTF8::code_unit_input_range auto&& range) {
    return Zeus::UTF8::starts_with_valid_character(std::ranges::begin(range),
                                                   std::ranges::end(range));
}

template <Zeus::UTF8::code_unit_input_iterator InputIt,
          std::sentinel_for<InputIt> Sentinel>
[[nodiscard]] constexpr bool is_valid_character_sequence(InputIt first,
                                                         Sentinel last) {
    while (first != last) {
        if (!Zeus::UTF8::starts_with_valid_character(first, last)) {
            return false;
        }

        Zeus::UTF8::advance(first, 1);
    }

    return true;
}

[[nodiscard]] constexpr bool is_valid_character_sequence(
    Zeus::UTF8::code_unit_input_range auto&& range) {
    return Zeus::UTF8::is_valid_character_sequence(std::ranges::begin(range),
                                                   std::ranges::end(range));
}

// =========================== END OF VALIDATION ============================ //

// ============================== INSPECTION ================================ //

template <Zeus::UTF8::code_unit_input_iterator InputIt,
          std::sentinel_for<InputIt> Sentinel>
[[nodiscard]] constexpr Zeus::ssize calculate_length(InputIt first,
                                                     Sentinel last) {
    Zeus::ssize length{0};

    while (first != last && *first != '\0') {
        Zeus::UTF8::advance(first, 1);
        ++length;
    }

    return length;
}

template <Zeus::UTF8::code_unit_input_range Range>
[[nodiscard]] constexpr Zeus::ssize calculate_length(Range&& range) {
    return Zeus::UTF8::calculate_length(std::ranges::begin(range),
                                        std::ranges::end(range));
}

/**
 *
 *
 * @note Zeus::Unicode::CodePoint *has to* be valid so the range will be
 * valid.
 */
[[nodiscard]] constexpr Zeus::ssize encode_length(
    Zeus::Unicode::CodePoint code_point) noexcept {

    // NOLINTBEGIN(*-magic-numbers)
    if (code_point <= 0x007FU) return 1;
    if (code_point <= 0x07FFU) return 2;
    if (code_point <= 0xFFFFU) return 3;
    // NOLINTEND(*-magic-numbers)

    // data <= 0x10FFFFU
    return 4;
}

template <std::input_iterator InputIt, std::sentinel_for<InputIt> Sentinel>
requires std::same_as<Unicode::CodePoint, std::iter_value_t<InputIt>>
[[nodiscard]] constexpr Zeus::ssize encode_length(InputIt first,
                                                  Sentinel last) noexcept {
    // TODO(tristan): Figure out if this is undefined behavior
    return std::accumulate(first, last, 0, encode_length);
}

template <std::ranges::input_range Range, std::weakly_incrementable OutputIt>
requires std::same_as<Unicode::CodePoint, std::iter_value_t<Range>>
[[nodiscard]] constexpr Zeus::ssize encode_length(Range&& range) noexcept {
    return Zeus::UTF8::encode_length(std::ranges::begin(range),
                                     std::ranges::end(range));
}

// ============================ END OF INSPECTION =========================== //

// ============================== TRANSFORM ================================= //

template <std::weakly_incrementable OutputIt>
requires std::convertible_to<CodeUnit, std::iter_value_t<OutputIt>>
constexpr OutputIt encode(Zeus::Unicode::CodePoint code_point,
                          OutputIt result) noexcept {
    auto const data = Zeus::to_integer<Zeus::u32>(code_point);

    // NOLINTBEGIN(*-magic-numbers)
    if (data <= 0x007FU) {
        // Encode output: 0xxx xxxx
        *result = static_cast<Zeus::UTF8::CodeUnit>(data);
    } else if (data <= 0x07FFU) {
        // Before encoded: 0000 0xxx xxxx xxxx
        // Encoded output: 110x xxxx 10xx xxxx
        *result =
            static_cast<Zeus::UTF8::CodeUnit>((data >> 6U & 0x1FU) | 0xC0U);
        ++result;
        *result = static_cast<Zeus::UTF8::CodeUnit>((data & 0x3FU) | 0x80U);
    } else if (data <= 0xFFFFU) {
        // Before encoded: 0000 0000 xxxx xxxx xxxx xxxx
        // Encoded output: 1110 xxxx 10xx xxxx 10xx xxxx
        *result = static_cast<Zeus::UTF8::CodeUnit>(data >> 12 | 0xE0U);
        ++result;
        *result =
            static_cast<Zeus::UTF8::CodeUnit>((data >> 6U & 0x3FU) | 0x80U);
        ++result;
        *result = static_cast<Zeus::UTF8::CodeUnit>((data & 0x3FU) | 0x80U);
    } else {  // data <= 0x10FFFFU
        // Before encoded: 0000 0000 000x xxxx xxxx xxxx xxxx xxxx
        // Encoded output: 1111 0xxx 10xx xxxx 10xx xxxx 10xx xxxx
        *result = static_cast<Zeus::UTF8::CodeUnit>(data >> 18U | 0xF0U);
        ++result;
        *result =
            static_cast<Zeus::UTF8::CodeUnit>((data >> 12U & 0x3FU) | 0x80U);
        ++result;
        *result =
            static_cast<Zeus::UTF8::CodeUnit>((data >> 6U & 0x3FU) | 0x80U);
        ++result;
        *result = static_cast<Zeus::UTF8::CodeUnit>((data & 0x3FU) | 0x80U);
    }
    // NOLINTEND(*-magic-numbers)

    return std::ranges::next(result);
}

[[nodiscard]] inline std::u8string encode(
    Zeus::Unicode::CodePoint code_point) noexcept {
    std::u8string str{};
    str.reserve(Zeus::UTF8::encode_length(code_point));
    
    encode(code_point, std::ranges::begin(str));

    return str;
}

template <typename In, typename Out>
using encode_result = std::ranges::in_out_result<In, Out>;

template <typename In, typename Out>
using decode_result = std::ranges::in_out_result<In, Out>;

template <std::input_iterator InputIt, std::sentinel_for<InputIt> Sentinel,
          std::weakly_incrementable OutputIt>
requires std::convertible_to<CodeUnit, std::iter_value_t<OutputIt>>
constexpr encode_result<InputIt, OutputIt> encode(InputIt first, Sentinel last,
                                                  OutputIt result) {
    for (; first != last; ++first) {
        result = encode(*first, result);
    }

    return {std::move(first), std::move(result)};
}

template <std::ranges::input_range Range, std::weakly_incrementable OutputIt>
requires std::convertible_to<CodeUnit, std::iter_value_t<OutputIt>>
constexpr encode_result<std::ranges::borrowed_iterator_t<Range>, OutputIt>
encode(Range&& range, OutputIt result) {
    return encode(std::ranges::begin(range), std::ranges::end(range),
                  std::move(result));
}

template <std::input_iterator InputIt, std::sentinel_for<InputIt> Sentinel>
[[nodiscard]] constexpr std::optional<Zeus::Unicode::CodePoint> decode(
    InputIt first, Sentinel last) noexcept {
    using value_type = Unicode::CodePoint::value_type;

    auto const distance = std::ranges::distance(first, last);

    value_type code_point_value{};

    if (distance == 1) {  // ASCII
        code_point_value = static_cast<value_type>(*first);
    } else if (distance == 2) {
        auto const byte1 = static_cast<value_type>(*first & 0x1FU) << 6U;
        ++first;
        auto const byte2 = static_cast<value_type>(*first & 0x3FU);

        code_point_value = byte1 | byte2;
    } else if (distance == 3) {
        auto const byte1 = static_cast<value_type>(*first & 0x0FU) << 12U;
        ++first;
        auto const byte2 = static_cast<value_type>(*first & 0x3FU) << 6U;
        ++first;
        auto const byte3 = static_cast<value_type>(*first & 0x3FU);

        code_point_value = byte1 | byte2 | byte3;
    } else if (distance == 4) {
        auto const byte1 = static_cast<value_type>(*first & 0x07U) << 18U;
        ++first;
        auto const byte2 = static_cast<value_type>(*first & 0x3FU) << 12U;
        ++first;
        auto const byte3 = static_cast<value_type>(*first & 0x3FU) << 6U;
        ++first;
        auto const byte4 = static_cast<value_type>(*first & 0x3FU);

        code_point_value = byte1 | byte2 | byte3 | byte4;
    } else [[unlikely]] {
        return std::nullopt;
    }

    if (!Zeus::Unicode::is_valid(code_point_value)) [[unlikely]] {
        return std::nullopt;
    }

    return Zeus::Unicode::CodePoint{code_point_value};
}

template <std::ranges::input_range Range>
[[nodiscard]] constexpr std::optional<Zeus::Unicode::CodePoint> decode(
    Range&& range) {
    return Zeus::UTF8::decode(std::ranges::begin(range),
                              std::ranges::end(range));
}

template <std::input_iterator InputIt, std::sentinel_for<InputIt> Sentinel,
          std::weakly_incrementable OutputIt>
requires std::convertible_to<Unicode::CodePoint, std::iter_value_t<OutputIt>>
constexpr decode_result<InputIt, OutputIt> decode(
    InputIt first, Sentinel last, OutputIt result,
    Unicode::CodePoint replace_character =
        Unicode::CodePoint::g_replacementCharacter) noexcept {
    while (first != last) {
        auto const count = peek_char_size(*first);
        auto const distance = std::ranges::distance(first, last);

        if (!count.has_value()) {
            *result = replace_character;
            ++result;
            ++first;
            continue;
        }

        auto const current_last = first + count.value();

        // Make sure we have enough space left for character
        if (distance >= count) {
            if (is_ascii(*first)) {
                auto const decoded = decode(first, current_last);
                *result = std::move(decoded.value());
                ++result;
                ++first;
                continue;
            }

            auto const non_cont_byte_pos = std::ranges::find_if_not(
                first, current_last, is_continuation_byte);

            // We did not find a non-continuation byte in the sequence
            if (non_cont_byte_pos == current_last) {
                *result = replace_character;
                ++result;

                // Advance to invalid byte in sequence
                // The invalid byte in this case might be the start of
                // another UTF-8 character
                std::ranges::advance(
                    first, std::ranges::distance(first, non_cont_byte_pos));
                continue;
            }

            // else we only found continuation bytes

            auto const decoded = decode(first, current_last);
            *result = std::move(decoded.value());
            ++result;

            std::ranges::advance(first, count.value());
            continue;
        }

        *result = replace_character;
        ++result;
        ++first;
    }

    return {std::move(first), std::move(result)};
}

template <std::ranges::input_range Range, std::weakly_incrementable OutputIt>
requires std::convertible_to<Unicode::CodePoint, std::iter_value_t<OutputIt>>
constexpr decode_result<std::ranges::borrowed_iterator_t<Range>, OutputIt>
decode(Range&& range, OutputIt result,
       Unicode::CodePoint replace_character =
           Unicode::CodePoint::g_replacementCharacter) noexcept {
    return decode(std::ranges::begin(range), std::ranges::end(range),
                  std::move(result), replace_character);
}

// ============================ END OF TRANSFORM ============================ //

}  // namespace cpp20_v1

}  // namespace Zeus::UTF8
