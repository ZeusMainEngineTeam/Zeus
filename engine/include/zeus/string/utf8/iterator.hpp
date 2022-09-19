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

namespace Zeus::UTF8 {

// NOLINTNEXTLINE(readability-identifier-naming)
inline namespace cpp20_v1 {

template <std::input_or_output_iterator Iterator>
[[nodiscard]] constexpr Iterator next(Iterator iterator) {
    return std::ranges::next(iterator,
                             Zeus::UTF8::peek_char_size(*iterator).value());
}

template <std::input_or_output_iterator Iterator>
[[nodiscard]] constexpr Iterator next(Iterator iterator,
                                      std::iter_difference_t<Iterator> num) {
    ZEUS_ASSERT(num > 0, "Number has to be positive.");

    while (num > 0) {
        iterator = Zeus::UTF8::next(iterator);
        --num;
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
