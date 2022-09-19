/**
 * This file is part of the Zeus Game Engine.
 * Copyright (C) 2021 Tristan F.
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

#include <cstdint>
#include <type_traits>

namespace Zeus {

/**
 * C++
 * https://en.cppreference.com/w/cpp/types/integer
 *
 * These types are technically optional for the compiler to implement. Luckily
 * most, if not all, modern platforms today support them.
 */
using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

/**
 * C++
 * https://en.cppreference.com/w/cpp/types/integer
 *
 * These types are technically optional for the compiler to implement. Luckily
 * most, if not all, modern platforms today support them.
 */
using i8 = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

/**
 * float is usually 32-bit on most platforms.
 */
using f32 = float;

/**
 * double is usually 64-bit on most platforms.
 */
using f64 = double;

using ssize = std::make_signed_t<std::size_t>;

}  // namespace Zeus

/**
 * Checking types.
 *
 * There is a lot of specific language in the C++ standard.  This is why it's
 * important to check the cpp reference for possible bugs. I believe part of
 * best practices are to be aware of your current types. The lack of guarantees
 * will cause bugs in the future if not checked. Trust me.
 *
 *                                                               - Tristan
 */

/**
 * C++20
 * https://en.cppreference.com/w/cpp/language/types#char8_t
 *
 * char8_t - type for UTF-8 character representation, required to be **large
 * enough** to represent any UTF-8 code unit (8 bits).
 *
 * Zeus::UTF8::CodeUnit is an alias of char8_t.  UTF-8 string literals (u8) are
 * in char8_t and we obviously want to support a main language feature.
 */
static_assert(sizeof(char8_t) == 1, "char8_t is more than 8-bits (UTF-8).");

/**
 * C++
 * https://docs.microsoft.com/en-us/cpp/cpp/fundamental-types-cpp?view=msvc-170
 *
 * TODO(tristan): Try to find cppreference page to replace the one below
 *
 * The absolute size of built-in floating-point types isn't specified in the
 * standard.
 */
// NOLINTBEGIN(*-magic-numbers)
static_assert(sizeof(Zeus::f32) == 4, "32-bit floating type not guaranteed.");
static_assert(sizeof(Zeus::f64) == 8, "64-bit floating type not guaranteed.");
// NOLINTEND(*-magic-numbers)
