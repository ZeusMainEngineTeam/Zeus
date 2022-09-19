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

#include <iterator>
#include <concepts>
#include <algorithm>

namespace Zeus {

template <typename Container>
concept has_replace = requires(Container container) {
    container.replace();
    //{ container.replace }
};

template <typename Iterator, typename T>
concept input_iterator_of = std::input_iterator<Iterator> &&
    std::convertible_to<typename std::iter_value_t<Iterator>, T>;

// std::insert_iterator



template <typename Container, typename Iterator>
concept has_insert_and_erase = std::input_iterator<Iterator> &&
    requires(Container& container, typename Container::const_iterator iterator,
             std::ranges::range_value_t<Container> value) {
    container.insert(iterator, std::declval<>(value));
    container.erase();
};

template <typename Container>
concept can_update =
    Zeus::has_replace<Container> || Zeus::has_insert_and_erase<Container>;

template <typename Container, std::input_iterator InputIt,
          std::sentinel_for<InputIt> Sentinel>
//requires Zeus::has_insert_and_erase<Container>
constexpr void update(Container& container, InputIt dest_first, Sentinel dest_last,
        InputIt source_first, Sentinel source_last) {
    auto const dest_distance = std::ranges::distance(dest_first, dest_last);
    auto const source_distance =
        std::ranges::distance(source_first, source_last);

    // The end of the first (and possibly only) copy
    auto copy_end_pos = source_first + std::min(dest_distance, source_distance);

    // Overwrite data that will not be removed/added
    auto copy_result =
        std::ranges::copy(source_first, copy_end_pos, dest_first);

    if (dest_distance > source_distance) {
        // We need to shrink (erase) data since we are updating it with less
        // values
        container.erase(copy_result.out, dest_last);
    } else {  // We need to allocate memory for values
        // Insert the values that would cause a reallocation
        container.insert(copy_result.out, copy_end_pos, source_last);
    }
}

template <typename Container, std::ranges::input_range Range>
constexpr void update(Container& container, Range&& destination,
        InputIt source_first, Sentinel source_last) {
    return Zeus::update(container, std::ranges::begin(destination),
                        std::ranges::end(destination), first, last);
}

template <typename Container, std::ranges::input_range Range>
constexpr void update(Container& container, InputIt dest_first,
                      Sentinel dest_last, Range&& source) {
    return Zeus::update(container, dest_first, dest_last,
                        std::ranges::begin(source), std::ranges::end(source));
}

template <typename Container, std::ranges::input_range Range>
//requires Zeus::has_insert_and_erase<Container>
constexpr void update(Container& container, Range&& destination,
        Range&& source) {
    return Zeus::update(container, std::ranges::begin(destination),
                        std::ranges::end(destination),
                        std::ranges::begin(source), std::ranges::end(source));
}

template <typename Container, std::ranges::input_range Range>
// requires Zeus::has_insert_and_erase<Container>
constexpr void update(Container& container, Range&& source) {
    return Zeus::update(container, container, source);
}

}  // namespace Zeus
