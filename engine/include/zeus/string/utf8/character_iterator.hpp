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

#include <memory>
#include <optional>
#include <iostream>
#include <iomanip>

#include "zeus/core/types.hpp"
#include "zeus/string/utf8/character.hpp"
#include "zeus/string/utf8/iterator.hpp"

/**
 * @file string/utf8/character_iterator.hpp
 */

namespace Zeus::UTF8 {

// NOLINTNEXTLINE(readability-identifier-naming)
inline namespace cpp20_v1 {

template <typename Container>
class CharacterIterator {
   public:
    using value_type = UTF8::Character;
    using pointer = value_type*;
    using const_pointer = value_type const*;
    using reference = value_type&;
    using const_reference = value_type const&;

    using iterator_category = std::bidirectional_iterator_tag;

    constexpr explicit CharacterIterator(
        std::ranges::iterator_t<Container> iterator)
        : m_iterator{iterator} {}

    // Prefix increment (++i)
    constexpr CharacterIterator& operator++() noexcept {
        this->m_it = UTF8::next(this->m_it);

        this->reset_char();

        return *this;
    }

    // Postfix increment (i++)
    constexpr CharacterIterator operator++(int) noexcept {
        CharacterIterator tmp = *this;

        ++(*this);

        return tmp;
    }

    // Prefix decrement (--i)
    constexpr CharacterIterator& operator--() noexcept {
        this->m_it = UTF8::prev(this->m_it);

        this->reset_char();

        return *this;
    }

    // Postfix decrement (i--)
    constexpr CharacterIterator operator--(int) noexcept {
        CharacterIterator tmp = *this;

        --(*this);

        return tmp;
    }

    constexpr pointer operator->() {
        if (!this->m_current.has_value()) {
            this->m_current = create_char(this->m_it);
        }

        return std::addressof(this->m_current.value());
    }

    constexpr reference operator*() {
        if (!this->m_current.has_value()) {
            this->m_current = create_char(this->m_it);
        }

        return this->m_current.value();
    }

    constexpr bool operator==(
        CharacterIterator const& other) const noexcept {
        return this->m_it == other.m_it;
    }

    constexpr bool operator!=(
        CharacterIterator const& other) const noexcept {
        return !(*this == other);
    }

   private:
    Character::iterator m_iterator;
    std::optional<UTF8::Character> m_current;

    constexpr void reset_char() noexcept {
        if (this->m_current.has_value()) {
            this->m_current = std::nullopt;
        }
    }

    [[nodiscard]] UTF8::Character create_char(Character::iterator it) const {
        auto const byte_count = Zeus::UTF8::peek_char_size(*it);

        using size_type = Zeus::UTF8::Character::size_type;

        return {std::to_address(it),
                static_cast<size_type>(byte_count.value())};
    }
};

}  // namespace cpp20_v1

}  // namespace Zeus::UTF8
