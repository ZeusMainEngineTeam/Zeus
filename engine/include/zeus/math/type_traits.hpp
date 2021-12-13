/**
 * This file is part of the Zeus Game Engine.
 * Copyright (C) 2021 Tristan F.
 *
 * The Zeus Game Engine is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The Zeus Game Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the Zeus Game Engine. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <limits>
#include <type_traits>

/**
 * @file type_traits.hpp
 */

namespace Zeus {

namespace Math {

/**
 * Checks if the given type is IEEE 754 compliant and has a representation of
 * infinity.
 *
 * @tparam T The type to check
 */
template <typename T>
struct can_use_infinity
    : std::bool_constant<std::numeric_limits<T>::is_iec559 &&
                         std::numeric_limits<T>::has_infinity> {};

/**
 * Checks if the given type is IEEE 754 compliant and has a representation of
 * infinity.
 *
 * @tparam T The type to check
 */
template <typename T>
inline constexpr bool can_use_infinity_v = can_use_infinity<T>::value;

/**
 * Alias of std::enable_if for can_use_infinity.
 *
 * @see std::enable_if
 * @see Zeus::Math::can_use_infinity
 *
 * @tparam T The type to check against
 */
template <typename T>
using enable_if_can_use_infinity = std::enable_if<can_use_infinity_v<T>, T>;

/**
 * Alias of std::enable_if for can_use_infinity.
 *
 * @see std::enable_if
 * @see Zeus::Math::can_use_infinity
 *
 * @tparam T The type to check against
 */
template <typename T>
using enable_if_can_use_infinity_t =
    typename enable_if_can_use_infinity<T>::type;

}  // namespace Math

}  // namespace Zeus
