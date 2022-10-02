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

#include <concepts>
#include <cstddef>
#include <iterator>
#include <optional>
#include <span>
#include <stdexcept>
#include <type_traits>

#include "zeus/core/types.hpp"
#include "zeus/string/unicode/exception.hpp"

namespace Zeus {

namespace Unicode {

class CodePoint {
   public:
    // TODO(tristan): Remember why I made this unsigned vs signed?  Unicode
    // standard?
    using value_type = Zeus::u32;

    static constexpr value_type g_rawMax = 0x10FFFFU;
    static constexpr value_type g_rawMin = 0x0U;

    CodePoint() noexcept = default;

    constexpr explicit CodePoint(value_type value)
        : m_value{value > g_rawMax
                      ? throw Zeus::Unicode::Exception{"Invalid code point."}
                      : value} {}

    [[nodiscard]] constexpr auto operator<=>(CodePoint const&) const noexcept =
        default;

    [[nodiscard]] constexpr auto operator<=>(value_type value) const noexcept {
        return this->m_value <=> value;
    }

    template <std::integral IntegerType>
    friend constexpr auto to_integer(Zeus::Unicode::CodePoint point);

    static CodePoint const g_max;
    static CodePoint const g_min;
    static CodePoint const g_replacementCharacter;

   private:
    value_type m_value;
};

template <std::integral IntegerType>
[[nodiscard]] constexpr auto to_integer(CodePoint point) {
    return IntegerType(point.m_value);
}

[[nodiscard]] constexpr bool is_valid(CodePoint::value_type point) noexcept {
    return point <= CodePoint::g_rawMax && point >= CodePoint::g_rawMin;
}

}  // namespace Unicode

template <std::integral IntegerType>
[[nodiscard]] constexpr auto to_integer(Zeus::Unicode::CodePoint point) {
    return Zeus::Unicode::to_integer<IntegerType>(point);
}

[[nodiscard]] std::string to_string(Zeus::Unicode::CodePoint point) noexcept;

}  // namespace Zeus
