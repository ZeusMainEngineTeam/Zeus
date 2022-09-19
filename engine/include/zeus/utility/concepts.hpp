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

#include <ranges>
#include <iterator>
#include <algorithm>

namespace Zeus {

template <typename Range, typename T>
concept input_range_of = std::ranges::input_range<Range> &&
    std::convertible_to<std::ranges::range_value_t<Range>, T>;

template <typename Iterator, typename T>
concept input_iterator_of = std::input_iterator<Iterator> &&
    std::convertible_to<typename std::iter_value_t<Iterator>, T>;

template <typename Container>
concept has_replace = requires(Container container) {
    container.replace();
    //{ container.replace }
};

template <typename Container>
concept has_insert_and_erase = requires(Container container) {
    container.insert();
    container.erase();
};

template <typename Container>
concept can_update =
    Zeus::has_replace<Container> || Zeus::has_insert_and_erase<Container>;

}  // namespace Zeus
