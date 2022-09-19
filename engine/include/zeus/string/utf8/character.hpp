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

#include <optional>
#include <array>
#include <stdexcept>
#include <iostream>

#include "zeus/memory/utility.hpp"
#include "zeus/core/types.hpp"
#include "zeus/string/utf8/code_unit.hpp"
#include "zeus/string/unicode/code_point.hpp"
#include "zeus/string/utf8/algorithm.hpp"
#include "zeus/core/assert.hpp"

namespace Zeus {

/**
 * @file string/utf8/character.hpp
 */

namespace UTF8 {

// NOLINTNEXTLINE(readability-identifier-naming)
inline namespace cpp20_v1 {

/*
 * An encoded UTF-8 character.
 */
class Character {
   public:
    using size_type = Zeus::i8;
    using value_type = Zeus::UTF8::CodeUnit;
    using pointer = value_type*;
    using const_pointer = value_type const*;
    using reference = value_type&;
    using const_reference = value_type const&;

    static constexpr auto g_minValidSize = 1;
    static constexpr auto g_maxValidSize = 4;

    using storage_type = std::array<value_type, Character::g_maxValidSize>;

    using iterator = typename storage_type::iterator;
    using const_iterator = typename storage_type::const_iterator;
    using reverse_iterator = typename storage_type::reverse_iterator;
    using const_reverse_iterator =
        typename storage_type::const_reverse_iterator;

    constexpr Character() noexcept : m_data{0}, m_len{0} {}

    constexpr explicit Character(Zeus::Unicode::CodePoint code_point) noexcept
        : m_data{},
          m_len{static_cast<size_type>(Zeus::UTF8::encode_length(code_point))} {
        Zeus::UTF8::encode(code_point, std::ranges::begin(this->m_data));
    }

    Character(const_pointer cstr, size_type len)
        : Character{std::u8string_view{
              cstr, len > Character::g_maxValidSize
                        ? throw std::length_error{"Character is too large."}
                        : static_cast<std::string_view::size_type>(len)}} {}

    Character(std::u8string_view view)
        : Character{std::ranges::cbegin(view), std::ranges::cend(view)} {}

    template <Zeus::UTF8::code_unit_input_iterator InputIt,
              std::sentinel_for<InputIt> Sentinel>
    constexpr Character(InputIt first, Sentinel last)
        : m_data{},
          m_len{static_cast<size_type>(std::ranges::distance(first, last))} {
        ZEUS_ASSERT(Character::is_valid_length(first, last),
                    "UTF-8 Character is too big or empty.");

        std::ranges::copy(first, last, std::ranges::begin(this->m_data));
    }

    // NOLINTNEXTLINE(google-explicit-constructor, hicpp-explicit-conversions)
    template <typename Range, typename DRange = std::remove_cvref_t<Range>>
    requires (!std::is_same_v<DRange, std::u8string_view> 
            || !std::is_same_v<DRange, std::u8string>)
    && Zeus::UTF8::code_unit_input_range<Range>
    && (!std::is_convertible_v<Range, const_pointer>)
    constexpr Character(Range&& range)
        : Character{std::ranges::begin(range), std::ranges::end(range)} {}

    [[nodiscard]] constexpr bool operator==(
        Character const& character) const noexcept = default;

    [[nodiscard]] constexpr reference operator[](size_type index) noexcept {
        return this->m_data[index];
    }

    [[nodiscard]] constexpr value_type operator[](
        size_type index) const noexcept {
        return this->m_data[index];
    }

    [[nodiscard]] constexpr size_type size() const noexcept {
        return this->m_len;
    }

    [[nodiscard]] constexpr iterator begin() noexcept {
        return std::begin(this->m_data);
    }

    [[nodiscard]] constexpr const_iterator begin() const noexcept {
        return std::begin(this->m_data);
    }

    [[nodiscard]] constexpr const_iterator cbegin() const noexcept {
        return std::cbegin(this->m_data);
    }

    [[nodiscard]] constexpr iterator end() noexcept {
        return std::begin(this->m_data) + this->m_len;
    }

    [[nodiscard]] constexpr const_iterator end() const noexcept {
        return std::cbegin(this->m_data) + this->m_len;
    }

    [[nodiscard]] constexpr const_iterator cend() const noexcept {
        return std::cbegin(this->m_data) + this->m_len;
    }

    [[nodiscard]] constexpr reverse_iterator rbegin() noexcept {
        return std::rbegin(this->m_data);
    }

    [[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept {
        return std::rbegin(this->m_data);
    }

    [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept {
        return std::crbegin(this->m_data);
    }

    [[nodiscard]] constexpr reverse_iterator rend() noexcept {
        return std::rbegin(this->m_data) + this->m_len;
    }

    [[nodiscard]] constexpr const_reverse_iterator rend() const noexcept {
        return std::rbegin(this->m_data) + this->m_len;
    }

    [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept {
        return std::crbegin(this->m_data) + this->m_len;
    }

    /**
    * Checks if the given UTF-8 character has a valid character length.
    *
    * @note UTF-8 characters range from 1 to 4 bytes in length. The argument is
    * passed in as a const-reference because the length of the given character
    * could be of a large size.
    */
    template <std::input_iterator InputIt, std::sentinel_for<InputIt> Sentinel>
    [[nodiscard]] static constexpr bool is_valid_length(InputIt first,
                                                        Sentinel last) {
        auto const distance = std::ranges::distance(first, last);

        return distance >= Zeus::UTF8::Character::g_minValidSize &&
               distance <= Zeus::UTF8::Character::g_maxValidSize;
    }

    template <std::ranges::input_range Range>
    [[nodiscard]] static constexpr bool is_valid_length(Range&& range) {
        return Zeus::UTF8::Character::is_valid_length(std::ranges::begin(range),
                                                      std::ranges::end(range));
    }

   private:
    storage_type m_data;
    size_type m_len;
};

// =============================== INSPECTING =============================== //

/**
 * Checks if the given UTF-8 character is valid.
 *
 * @note This is the same as Zeus::UTF8::is_valid_character.  This non-member
 * function is here to enforce more strict type checking and to increase
 * readability.
 *
 * @see Zeus::UTF8::is_valid_character
 */
[[nodiscard]] constexpr bool is_valid(Character const& character) {
    return Zeus::UTF8::is_valid_character(character);
}

}  // namespace cpp20_v1

}  // namespace UTF8

// ============================== TRANSFORMING ============================== //

constexpr Zeus::UTF8::Character::value_type at(
    Zeus::UTF8::Character const& character,
    Zeus::UTF8::Character::size_type index) {
    if (index < 0 || index > character.size()) {
        throw std::out_of_range{"Index out of bounds for UTF-8 character."};
    }

    return character[index];
}

constexpr Zeus::UTF8::Character::reference at(
    Zeus::UTF8::Character& character, Zeus::UTF8::Character::size_type index) {
    if (index < 0 || index > character.size()) {
        throw std::out_of_range{"Index out of bounds for UTF-8 character."};
    }

    return character[index];
}

/**
*
*
* @tparam IntegerType
*
* @param character
*
* @return
*/
template <typename IntegerType>
[[nodiscard]] constexpr IntegerType to_integer(
    Zeus::UTF8::Character const& character) {
    static_assert(std::is_integral_v<IntegerType>);

    /*
     * May throw if character is not a valid length.
     * @see Zeus::UTF8::ValidCharacter
     */
    return IntegerType(Memory::to_u32(character));
}

/**
 *
 * 
 * @param
 *
 * @return
 */
[[nodiscard]] std::string to_string(UTF8::Character const& character);

}  // namespace Zeus
