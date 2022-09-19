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
#include <stdexcept>
#include <algorithm>

#include "zeus/string/utf8/character.hpp"
#include "zeus/string/unicode/code_point.hpp"

namespace Zeus {

namespace UTF8 {

// NOLINTNEXTLINE(readability-identifier-naming)
inline namespace cpp20_v1 {

#define ZEUS_USE_EXCEPTION_VALIDATION

/**
 * A wrapper around Zeus::UTF8::Character that will guarantee a valid UTF-8
 * character.
 */
class ValidCharacter final {
   public:
    using data_type = Zeus::UTF8::Character;

    using size_type = typename data_type::size_type;

    using value_type = typename data_type::value_type;
    using const_pointer = typename data_type::value_type;

    using const_iterator = typename data_type::const_iterator;
    using const_reverse_iterator = typename data_type::const_reverse_iterator;

    // Default value is replacement character (array is encoded UTF-8)
    ValidCharacter() noexcept
        : m_char{std::array<Zeus::UTF8::CodeUnit, 3>{0xef, 0xbf, 0xbd}} {}

    /*
    [[nodiscard]] static std::optional<ValidCharacter> create(
            const_pointer cstr, size_type len) {

    }

    [[nodiscard]] static std::optional<ValidCharacter> create(
            std::u8string_view view) {

    }
    */

    template <Zeus::UTF8::code_unit_input_iterator InputIt,
              std::sentinel_for<InputIt> Sentinel>
    [[nodiscard]] static std::optional<ValidCharacter> create(InputIt first,
            Sentinel last) {
        if (Zeus::UTF8::is_valid_character_sequence(first, last)) {
            return ValidCharacter{first, last};
        }

        return std::nullopt;
    }

    template <typename Range, typename DRange = std::remove_cvref_t<Range>>
    requires (!std::is_same_v<DRange, std::u8string_view> 
            || !std::is_same_v<DRange, std::u8string>)
    && Zeus::UTF8::code_unit_input_range<Range>
    && (!std::is_convertible_v<Range, const_pointer>)
    [[nodiscard]] static std::optional<ValidCharacter> create(Range&& range) {
        return Zeus::UTF8::ValidCharacter::create(std::ranges::begin(range),
                                                  std::ranges::end(range));
    }

    [[nodiscard]] constexpr bool operator==(
        ValidCharacter const& character) const noexcept = default;

    [[nodiscard]] constexpr bool operator==(
        Zeus::UTF8::code_unit_input_range auto&& range) const {
        return std::ranges::equal(*this, range);
    }

    [[nodiscard]] constexpr value_type operator[](
        size_type index) const noexcept {
        return this->m_char[index];
    }

    [[nodiscard]] constexpr size_type size() const noexcept {
        return this->m_char.size();
    }

    [[nodiscard]] constexpr const_iterator begin() const noexcept {
        return std::begin(this->m_char);
    }

    [[nodiscard]] constexpr const_iterator cbegin() const noexcept {
        return std::begin(this->m_char);
    }

    [[nodiscard]] constexpr const_iterator end() const noexcept {
        return std::end(this->m_char);
    }

    [[nodiscard]] constexpr const_iterator cend() const noexcept {
        return std::end(this->m_char);
    }

    [[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept {
        return std::rbegin(this->m_char);
    }

    [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept {
        return std::crbegin(this->m_char);
    }

    [[nodiscard]] constexpr const_reverse_iterator rend() const noexcept {
        return std::rend(this->m_char);
    }

    [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept {
        return std::crend(this->m_char);
    }

    [[nodiscard]] static constexpr Zeus::Unicode::CodePoint decode(
            ValidCharacter const& character) {
        return Zeus::UTF8::decode(character).value();
    }

   private:
    Zeus::UTF8::Character m_char;
};

}  // namespace cpp20_v1

}  // namespace UTF8

// ============================== TRANSFORMING ============================== //

constexpr Zeus::UTF8::ValidCharacter::value_type at(
    Zeus::UTF8::ValidCharacter const& character,
    Zeus::UTF8::ValidCharacter::size_type index) {
    if (index < 0 || index > character.size()) {
        throw std::out_of_range{"Index out of bounds for UTF-8 character."};
    }

    return character[index];
}

template <typename IntegerType>
[[nodiscard]] constexpr IntegerType to_integer(
    Zeus::UTF8::ValidCharacter const& character) {
    static_assert(std::is_integral_v<IntegerType>);

    // Should not throw since Zeus::UTF8::ValidCharacter is a valid length
    return IntegerType(Memory::to_u32(character));
}

}  // namespace Zeus
