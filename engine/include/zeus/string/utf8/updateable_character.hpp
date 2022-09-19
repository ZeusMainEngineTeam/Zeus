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

namespace Zeus::UTF8 {

// NOLINTNEXTLINE(readability-identifier-naming)
inline namespace cpp20_v1 {

// Forward declare
template <typename String>
class UpdateableCharacterIterator;

template <typename String>
class UpdateableCharacter {
   public:
    using this_type = UpdateableCharacter<String>;
    using iterator = typename String::code_unit_const_iterator;

    UpdateableCharacter() noexcept = default;

    UpdateableCharacter(String& container, iterator iter)
        : m_container{container},
          m_it{iter},
          m_char{iter, Zeus::UTF8::next(iter)} {}

    UpdateableCharacter& operator=(UTF8::Character const& character) {
        std::cout << "'=' operator called\n";

        this->m_container.replace(
            this->m_it, std::ranges::next(this->m_it, this->m_char.size()),
            character);

        this->m_char = character;

        return *this;
    }

    UpdateableCharacter& operator=(std::u8string_view view) {
        std::cout << "'=' operator called using std::u8string_view\n";

        this->m_container.replace(
            this->m_it, std::ranges::next(this->m_it, this->m_char.size()),
            view);

        std::cout << "After replace\n";

        this->m_char = view;

        std::cout << "After assignment...\n";

        return *this;
    }

    [[nodiscard]] bool operator==(
        UTF8::Character const& character) const noexcept {
        return this->m_char == character;
    }

    [[nodiscard]] bool operator==(this_type const&) const noexcept = default;

    [[nodiscard]] bool operator==(std::u8string_view view) const noexcept {
        return this->m_char == view;
    }

   private:
    String& m_container;
    iterator m_it;
    Zeus::UTF8::Character m_char;

    void reset(iterator iter) {
        this->m_it = iter;
        this->m_char = Zeus::UTF8::Character{iter, Zeus::UTF8::next(iter)};
    }

    template <typename T>
    friend class UpdateableCharacterIterator;
};

}  // namespace cpp20_v1

}  // namespace Zeus::UTF8
