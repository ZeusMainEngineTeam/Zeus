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

#include <cstddef>
#include <span>
#include <stdexcept>
#include <optional>
#include <array>
#include <iostream>

#include "zeus/core/types.hpp"

namespace Zeus::Memory {

[[nodiscard]] constexpr inline Zeus::u32 to_u32(std::byte a, std::byte b,
                                                std::byte c,
                                                std::byte d) noexcept {
    // NOLINTBEGIN(*-magic-numbers)
    return (static_cast<Zeus::u32>(a) << 24U) |
           (static_cast<Zeus::u32>(b) << 16U) |
           (static_cast<Zeus::u32>(c) << 8U) | static_cast<Zeus::u32>(d);
    // NOLINTEND(*-magic-numbers)
}

// NOLINTNEXTLINE(readability-identifier-length)
[[nodiscard]] constexpr inline Zeus::u32 to_u16(std::byte a, std::byte b) noexcept {
    // NOLINTNEXTLINE(*-magic-numbers)
    return (static_cast<Zeus::u32>(a) << 8U) | static_cast<Zeus::u32>(b);
}

template <std::input_iterator InputIt, std::sentinel_for<InputIt> Sentinel>
[[nodiscard]] constexpr inline Zeus::u32 to_u32(InputIt first, Sentinel last) {
    if (std::ranges::distance(first, last) > 4 || first == last) {
        throw std::invalid_argument{"Storage is too big or first == last."};
    }

    Zeus::u32 value = 0x00000000;

    // TODO(tristan): Make better
    for (; first != last; ++first) {
        auto const shift_amount = std::distance(first, last) - 1;

        // NOLINTBEGIN(*-magic-numbers)
        value |= static_cast<Zeus::u32>(*first)
                 << static_cast<Zeus::u32>(8U * shift_amount);
        // NOLINTEND(*-magic-numbers)
    }

    return value;
}

template <std::ranges::input_range Range>
[[nodiscard]] constexpr inline Zeus::u32 to_u32(Range&& range) {
    return Zeus::Memory::to_u32(std::ranges::begin(range),
                                std::ranges::end(range));
}

}  // namespace Zeus::Memory
