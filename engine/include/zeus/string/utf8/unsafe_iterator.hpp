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

namespace Zeus::UTF8 {

// NOLINTNEXTLINE(readability-identifier-naming)
inline namespace cpp20_v1 {

namespace Internal {

template <std::input_or_output_iterator Iterator>
[[nodiscard]] constexpr Iterator impl_unsafe_next(
    Iterator iterator, std::iter_difference_t<Iterator> amount) noexcept {
    return std::ranges::next(iterator, amount);
}

}  // namespace Internal

template <std::input_or_output_iterator Iterator>
[[nodiscard]] constexpr Iterator unsafe_next(Iterator iterator) {
    return std::ranges::next(iterator, Zeus::UTF8::leading_byte_size(*iterator));
}

template <std::bidirectional_iterator Iterator>
[[nodiscard]] constexpr Iterator unsafe_prev(Iterator iterator) {
    do {
        iterator = std::ranges::prev(iterator);
    } while (is_continuation_byte(*iterator));

    return iterator;
}

}  // namespace cpp20_v1

}  // namespace Zeus::UTF8
