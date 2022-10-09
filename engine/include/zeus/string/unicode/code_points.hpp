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

#include "zeus/string/unicode/code_point.hpp"

/**
 * @file string/unicode/code_points.hpp
 *
 * Contains commonly constructed Unicode code points for reuse.
 */

namespace Zeus::Unicode::CodePoints {

// NOLINTNEXTLINE(readability-identifier-naming)
inline namespace cpp20_v1 {

/**
* The maximum Unicode code point.
*/
inline constexpr CodePoint g_max{CodePoint::g_maxValue};

/**
* The minimum Unicode code point.
*/
inline constexpr CodePoint g_min{CodePoint::g_minValue};

/**
* The default replacement character for replacing an invalid UTF-8 character.
*/
inline constexpr CodePoint g_replacementCharacter{0xFFFDU};

/**
 * The Unicode null code point.
 */
inline constexpr CodePoint g_null{0};

}  // namespace cpp20_v1

}  // namespace Zeus::Unicode::CodePoints
