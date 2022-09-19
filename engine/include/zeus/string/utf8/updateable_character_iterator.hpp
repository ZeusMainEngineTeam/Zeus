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
#include <iostream>
#include <iomanip>

#include "zeus/string/utf8/character.hpp"
#include "zeus/string/utf8/algorithm.hpp"
#include "zeus/string/utf8/iterator.hpp"

#include "zeus/string/utf8/updateable_character.hpp"

namespace Zeus::UTF8 {

// NOLINTNEXTLINE(readability-identifier-naming)
inline namespace cpp20_v1 {

template <typename String>
class UpdateableCharacterIterator {
   public:
    using this_type = UpdateableCharacterIterator<String>;
    using iterator = typename String::code_unit_const_iterator;

    UpdateableCharacterIterator(String& container, iterator it)
        : m_container{container}, m_it{it}, m_updateableChar{container, it} {}

    // Prefix increment (++i)
    constexpr UpdateableCharacterIterator& operator++() noexcept {
        std::cout << "Before Zeus::UTF8::next...\n";
        this->m_it = UTF8::next(this->m_it);
        std::cout << "After Zeus::UTF8::next...\n";

        return *this;
    }

    // Postfix increment (i++)
    constexpr UpdateableCharacterIterator operator++(int) noexcept {
        UpdateableCharacterIterator tmp = *this;

        ++(*this);

        return tmp;
    }

    // Prefix decrement (--i)
    constexpr UpdateableCharacterIterator& operator--() noexcept {
        this->m_it = UTF8::prev(this->m_it);

        return *this;
    }

    // Postfix decrement (i--)
    constexpr UpdateableCharacterIterator operator--(int) noexcept {
        UpdateableCharacterIterator tmp = *this;

        --(*this);

        return tmp;
    }

    /*
    constexpr pointer operator->() {
        if (!this->m_current.has_value()) {
            this->m_current = create_char(this->m_it);
        }

        return std::addressof(this->m_current.value());
    }
    */

    constexpr UpdateableCharacter<String>& operator*() {
        this->m_updateableChar.reset(this->m_it);
        return this->m_updateableChar; 
    }

    constexpr bool operator==(
        UpdateableCharacterIterator const& other) const noexcept {
        return this->m_it == other.m_it;
    }

    constexpr bool operator==(iterator other) const noexcept {
        return this->m_it == other;
    }

    constexpr bool operator!=(
        UpdateableCharacterIterator const& other) const noexcept {
        return !(*this == other);
    }

    constexpr bool operator!=(iterator other) const noexcept {
        return !(*this == other);
    }

   private:
    String& m_container;
    iterator m_it;
    UpdateableCharacter<String> m_updateableChar;
};

}  // namespace cpp20_v1

}  // namespace Zeus::UTF8
