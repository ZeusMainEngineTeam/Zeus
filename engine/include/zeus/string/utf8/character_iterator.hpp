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

#include <iomanip>
#include <iostream>
#include <memory>
#include <optional>

#include "zeus/core/types.hpp"
#include "zeus/string/utf8/character.hpp"
#include "zeus/string/utf8/iterator.hpp"

/**
 * @file string/utf8/character_iterator.hpp
 */

namespace Zeus::UTF8 {

// NOLINTNEXTLINE(readability-identifier-naming)
inline namespace cpp20_v1 {

//std::convertible_to<std::iter_value_t<Iterator>, Zeus::UTF8::CodeUnit>

template <typename Iterator>
requires std::bidirectional_iterator<Iterator>
class CharacterIterator {
   public:
    using this_type = CharacterIterator<Iterator>;

    using value_type = UTF8::Character;
    using pointer = value_type*;
    using reference = value_type&;
    using const_pointer = value_type const*;
    using const_reference = value_type const&;

    using iterator_category = std::bidirectional_iterator_tag;

    // Copy constructor can throw D: - CANNOT be noexcept
    constexpr explicit CharacterIterator(Iterator iterator)
        : m_it{iterator} {}

    // Prefix increment (++i)
    constexpr CharacterIterator& operator++() noexcept {
        // TODO(tristan): Replace this one with safe version
        this->m_it = Zeus::UTF8::next(this->m_it);

        this->reset_character();

        return *this;
    }

    // Postfix increment (i++)
    constexpr CharacterIterator operator++(int) noexcept {
        CharacterIterator tmp{*this};

        ++(*this);

        return tmp;
    }

    // Prefix decrement (--i)
    constexpr CharacterIterator& operator--() noexcept {
        this->m_it = Zeus::UTF8::prev(this->m_it);

        this->reset_character();

        return *this;
    }

    // Postfix decrement (i--)
    constexpr CharacterIterator operator--(int) {
        CharacterIterator tmp{*this};

        --(*this);

        return tmp;
    }

    constexpr pointer operator->() {
        this->init_character_once();

        return std::addressof(this->m_current.value());
    }

    constexpr reference operator*() {
        this->init_character_once();

        return this->m_current.value();
    }

    constexpr bool operator==(CharacterIterator const& other) const noexcept {
        return this->m_it == other.m_it;
    }

    constexpr bool operator!=(CharacterIterator const& other) const noexcept {
        return !(*this == other);
    }

   private:
    Iterator m_it;
    std::optional<UTF8::Character> m_current;

    constexpr void reset_character() noexcept {
        this->m_current.reset();
    }

    void init_character_once() noexcept {
        if (!this->m_current.has_value()) {
            this->m_current = create_character(this->m_it);
        }
    }

    [[nodiscard]] UTF8::Character create_character(Iterator iter) const {
        auto const byte_size = Zeus::UTF8::leading_byte_size(*iter);

        using size_type = Zeus::UTF8::Character::size_type;

        return {std::to_address(iter), static_cast<size_type>(byte_size)};
    }
};

}  // namespace cpp20_v1

}  // namespace Zeus::UTF8
