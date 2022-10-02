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

#include "zeus/string/utf8/code_unit.hpp"
#include "zeus/string/utf8/unsafe_iterator.hpp"
#include "zeus/string/utf8/algorithm.hpp"

namespace Zeus::UTF8 {

// NOLINTNEXTLINE(readability-identifier-naming)
inline namespace cpp20_v1 {

template <std::input_or_output_iterator Iterator>
[[nodiscard]] constexpr Iterator safe_next(Iterator iterator) {
    auto const byte_size_opt = Zeus::UTF8::peek_char_size(*iterator);
    auto const last = std::ranges::next(iterator, byte_size_opt.value_or(0));

    if (byte_size_opt.has_value() &&
        Zeus::UTF8::is_valid_character_sequence(iterator, last)) {
        return Zeus::UTF8::unsafe_next(iterator);
    }

    return std::ranges::next(iterator,
                             Zeus::UTF8::leading_byte_size(*iterator));
}

template <std::bidirectional_iterator Iterator>
[[nodiscard]] constexpr Iterator safe_prev(Iterator iterator) {
    auto counter{0};

    do {
        iterator = std::ranges::prev(iterator);
        ++counter;
    } while (is_continuation_byte(*iterator) && counter < 4);

    auto const byte_count = Zeus::UTF8::peek_char_size(*iterator);

    if (!byte_count.has_value() || byte_count.value() != counter) {
        throw std::runtime_error{
            "Attempted to iterate over an invalid UTF-8 character."};
    }

    return iterator;
}

}  // namespace cpp20_v1

}  // namespace Zeus::UTF8
