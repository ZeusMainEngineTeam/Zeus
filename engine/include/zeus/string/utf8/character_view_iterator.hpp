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
#include <concepts>
#include <iterator>

#include "zeus/string/utf8/character_view.hpp"
#include "zeus/string/utf8/iterator.hpp"

namespace Zeus::UTF8 {

template <std::bidirectional_iterator Iterator>
requires std::same_as<std::iter_value_t<Iterator>, CodeUnit>
class CharacterViewIterator {
   public:
    using value_type = UTF8::CharacterView;
    using pointer = value_type*;
    using const_pointer = value_type const*;
    using reference = value_type&;
    using const_reference = value_type const&;

    using iterator_category = std::bidirectional_iterator_tag;

    constexpr CharacterViewIterator(Iterator it) noexcept : m_it{it} {}

    // Prefix increment (++i)
    constexpr CharacterViewIterator& operator++() noexcept {
        this->m_it = UTF8::next(this->m_it);

        this->reset_char();

        return *this;
    }

    // Postfix increment (i++)
    constexpr CharacterViewIterator operator++(int) noexcept {
        CharacterViewIterator tmp = *this;

        ++(*this);

        return tmp;
    }

    // Prefix decrement (--i)
    constexpr CharacterViewIterator& operator--() noexcept {
        this->m_it = UTF8::prev(this->m_it);

        this->reset_char();

        return *this;
    }

    // Postfix decrement (i--)
    constexpr CharacterViewIterator operator--(int) noexcept {
        CharacterViewIterator tmp = *this;

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
        CharacterViewIterator const& other) const noexcept {
        return this->m_it == other.m_it;
    }

    constexpr bool operator!=(
        CharacterViewIterator const& other) const noexcept {
        return !(*this == other);
    }

   private:
    Iterator m_it;
    std::optional<UTF8::CharacterView> m_current;

    constexpr void reset_char() noexcept {
        if (this->m_current.has_value()) {
            this->m_current = std::nullopt;
        }
    }

    [[nodiscard]] constexpr UTF8::CharacterView create_char(Iterator it) const {
        auto const byte_count = Zeus::UTF8::peek_char_size(*it);

        return UTF8::CharacterView(std::to_address(it), byte_count.value());
    }
};

}  // namespace Zeus::UTF8
