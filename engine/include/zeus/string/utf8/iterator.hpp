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
#include <iostream>
#include <iterator>

#include "zeus/core/assert.hpp"
#include "zeus/string/utf8/code_unit.hpp"
#include "zeus/string/utf8/unsafe_iterator.hpp"

namespace Zeus::UTF8 {

// NOLINTNEXTLINE(readability-identifier-naming)
inline namespace cpp20_v1 {

namespace Internal {

template <std::input_or_output_iterator Iterator>
[[nodiscard]] constexpr bool starts_with_valid_char(Iterator iterator) {
    auto const byte_size_opt = Zeus::UTF8::peek_char_size(*iterator);

    return byte_size_opt.has_value() &&
           (byte_size_opt == 1 || // ASCII
            std::ranges::all_of(iterator + 1, iterator + byte_size_opt.value(),
                                Zeus::UTF8::is_continuation_byte));
}

}  // namespace Internal

/**
 * Iterates to the first code unit of a valid UTF-8 character.
 *
 * @precondition iterator Currently points to a leading code unit byte
 * @precondition iterator Iterates over a valid UTF-8 character
 */
template <std::input_or_output_iterator Iterator>
[[nodiscard]] constexpr Iterator next(Iterator iterator) {
    ZEUS_ASSERT(Zeus::UTF8::is_leading_byte(*iterator),
        "@precondition iterator Currently points to a leading code unit byte");
    ZEUS_ASSERT(Internal::starts_with_valid_char(iterator),
                "@precondition iterator Iterates over a valid UTF-8 character");

    return std::ranges::next(iterator,
                             Zeus::UTF8::leading_byte_size(*iterator));
}

template <std::input_or_output_iterator Iterator>
[[nodiscard]] constexpr Iterator next(Iterator iterator,
                                      std::iter_difference_t<Iterator> num) {
    ZEUS_ASSERT(num >= 0, "@precondition num Number has to be non-negative.");

    while (num-- > 0) {
        iterator = Zeus::UTF8::next(iterator);
    }

    return iterator;
}

template <std::input_or_output_iterator Iterator,
          std::sentinel_for<Iterator> Sentinel>
[[nodiscard]] constexpr Iterator next(Iterator iterator, Sentinel bound) {
    return std::ranges::next(
        iterator, Zeus::UTF8::peek_char_size(*iterator).value(), bound);
}

template <std::input_or_output_iterator Iterator,
          std::sentinel_for<Iterator> Sentinel>
[[nodiscard]] constexpr Iterator next(Iterator iterator,
                                      std::iter_difference_t<Iterator> num,
                                      Sentinel bound) {
    ZEUS_ASSERT(num > 0, "Number has to be positive.");

    while (num > 0 && iterator != bound) {
        iterator = Zeus::UTF8::next(iterator, bound);
        --num;
    }

    return iterator;
}

template <std::bidirectional_iterator Iterator>
[[nodiscard]] constexpr Iterator prev(Iterator iterator) {
    do {
        iterator = std::ranges::prev(iterator);
    } while (is_continuation_byte(*iterator));

    return iterator;
}

template <std::bidirectional_iterator Iterator>
[[nodiscard]] constexpr Iterator prev(Iterator iterator,
        std::iter_difference_t<Iterator> num) {
    ZEUS_ASSERT(num > 0, "Number has to be positive.");

    while (num > 0) {
        iterator = Zeus::UTF8::prev(iterator);
        --num;
    }

    return iterator;
}

template <std::bidirectional_iterator Iterator>
[[nodiscard]] constexpr Iterator prev(Iterator iterator,
                                      std::iter_difference_t<Iterator> num,
                                      Iterator bound) {
    ZEUS_ASSERT(num > 0, "Number has to be positive.");

    while (num > 0 && iterator != bound) {
        iterator = Zeus::UTF8::prev(iterator, bound);
        --num;
    }

    return iterator;
}

template <std::input_or_output_iterator Iterator>
constexpr void advance(Iterator& iterator,
        std::iter_difference_t<Iterator> num) {
    if (num > 0) {
        iterator = Zeus::UTF8::next(iterator, num);
    }

    if (num < 0) {
        iterator = Zeus::UTF8::prev(iterator, num);
    }
}

template <std::input_or_output_iterator Iterator,
          std::sentinel_for<Iterator> Sentinel>
constexpr void advance(Iterator& iterator, Sentinel bound) {
    std::ranges::advance(iterator, bound);
}

template <std::input_or_output_iterator Iterator,
          std::sentinel_for<Iterator> Sentinel>
constexpr std::iter_difference_t<Iterator> advance(
    Iterator& iterator, std::iter_difference_t<Iterator> num, Sentinel bound) {
    if (num > 0) {
        iterator = Zeus::UTF8::next(iterator, num, bound);

        // TODO(tristan): Figure this one out.
//        return std::ranges::distance(iterator, );
        return num;
    }

    if (num < 0) {
        iterator = Zeus::UTF8::prev(iterator, num, bound);
        return num;
    }
}

}  // namespace cpp20_v1

}  // namespace Zeus::UTF8
