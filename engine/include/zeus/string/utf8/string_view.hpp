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

#include <string_view>

#include "zeus/core/types.hpp"
#include "zeus/string/utf8/code_unit.hpp"
#include "zeus/string/utf8/algorithm.hpp"

namespace Zeus {

namespace UTF8 {

// Forward declare
class String;

class StringView {
   public:
    using storage_type = std::u8string_view;

    using value_type = typename storage_type::value_type;
    using const_pointer = typename storage_type::const_pointer;
    using const_reference = typename storage_type::const_reference;

    using const_iterator = typename storage_type::const_iterator;
    using const_reverse_iterator =
        typename storage_type::const_reverse_iterator;

    using code_unit_const_iterator = typename storage_type::const_iterator;
    using code_unit_const_reverse_iterator =
        typename storage_type::const_reverse_iterator;

    //using character_iterator = Zeus::UTF8::CharacterIterator<String>;

    using size_type = Zeus::ssize;

    StringView() noexcept = default;

    [[nodiscard]] static std::optional<Zeus::UTF8::StringView> create(
        std::u8string_view view) {
        if (Zeus::UTF8::is_valid_character_sequence(view)) {
            return StringView{view};
        }

        return std::nullopt;
    }

    explicit StringView(Zeus::UTF8::String const& string)
        : m_data{string.code_unit_begin(), string.size_bytes()},
          m_len{string.length()} {}

    [[nodiscard]] constexpr size_type length() const noexcept {
        return this->m_len;
    }

    [[nodiscard]] constexpr code_unit_const_iterator code_unit_begin()
        const noexcept {
        return std::begin(this->m_data);
    }

    [[nodiscard]] constexpr code_unit_const_iterator code_unit_cbegin()
        const noexcept {
        return std::cbegin(this->m_data);
    }

    [[nodiscard]] constexpr code_unit_const_iterator code_unit_end()
        const noexcept {
        return std::end(this->m_data);
    }

    [[nodiscard]] constexpr code_unit_const_iterator code_unit_cend()
        const noexcept {
        return std::cend(this->m_data);
    }

    [[nodiscard]] constexpr code_unit_const_reverse_iterator code_unit_rbegin()
        const noexcept {
        return std::rbegin(this->m_data);
    }

    [[nodiscard]] constexpr code_unit_const_reverse_iterator code_unit_crbegin()
        const noexcept {
        return std::crbegin(this->m_data);
    }

    [[nodiscard]] constexpr code_unit_const_reverse_iterator code_unit_rend()
        const noexcept {
        return std::rend(this->m_data);
    }

    [[nodiscard]] constexpr code_unit_const_reverse_iterator code_unit_crend()
        const noexcept {
        return std::crend(this->m_data);
    }
   private:
    /**
     * Contains the code units.
     */
    std::u8string_view m_data;

    /**
     * The UTF-8 character length.
     */
    size_type m_len{};

    explicit StringView(std::u8string_view view)
        : m_data{view}, m_len{Zeus::UTF8::calculate_length(view)} {}
};

}  // namespace UTF8

}  // namespace Zeus
