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

#include "zeus/string/utf8/algorithm.hpp"
#include "zeus/string/utf8/code_unit.hpp"
#include "zeus/string/unicode/code_point.hpp"
#include "zeus/memory/utility.hpp"

namespace Zeus {

namespace UTF8 {

/**
 * A view of a collection of UTF-8 code units that make up a UTF-8 character.
 *
 * @note The contents of the view are not guaranteed to be a valid UTF-8
 * character. Member functions and free functions are provided to inspect the
 * validity of the UTF-8 character.
 *
 * @see Zeus::UTF8::is_valid
 * @see Zeus::UTF8::CharacterView::create
 */
class CharacterView {
   public:
    using value_type = CodeUnit;
    using const_pointer = CodeUnit const*;
    using const_reference = CodeUnit const&;

    using size_type = Zeus::i8;

    constexpr CharacterView() noexcept : m_len{0}, m_begin{nullptr} {}

    constexpr CharacterView(const_pointer begin, size_type count) noexcept
        : m_len{count}, m_begin{begin} {}

    template <std::contiguous_iterator It, std::sized_sentinel_for<It> End>
    requires std::same_as<std::iter_value_t<It>, value_type> &&
        (!std::convertible_to<End, size_type>)
        constexpr CharacterView(
            It first, End last) noexcept(noexcept(last - first))
        : m_len(static_cast<size_type>(last - first)),
          m_begin(std::to_address(first)) {}

    template <typename Range, typename DRange = std::remove_cvref_t<Range>>
    requires (!std::is_same_v<DRange, CharacterView>)
    && std::ranges::contiguous_range<Range>
    && std::ranges::sized_range<Range>
    && std::is_same_v<std::ranges::range_value_t<Range>, value_type>
    && (!std::is_convertible_v<Range, const_pointer>)
    constexpr CharacterView(Range&& range) noexcept(noexcept(
        std::ranges::size(range)) && noexcept(std::ranges::data(range)))
        : m_len(std::ranges::size(range)), m_begin(std::ranges::data(range)) {}

    [[nodiscard]] constexpr bool operator==(CharacterView view) const noexcept {
        return std::ranges::equal(*this, view);
    }

    [[nodiscard]] constexpr const_reference operator[](
            size_type index) const noexcept {
        return *(this->m_begin + index);
    }

    [[nodiscard]] constexpr const_pointer begin() const noexcept {
        return this->m_begin;
    }

    [[nodiscard]] constexpr const_pointer cbegin() const noexcept {
        return this->m_begin;
    }

    [[nodiscard]] constexpr const_pointer end() const noexcept {
        return this->m_begin + this->m_len;
    }

    [[nodiscard]] constexpr const_pointer cend() const noexcept {
        return this->m_begin + this->m_len;
    }

    [[nodiscard]] constexpr size_type length() const noexcept {
        return this->m_len;
    }

    [[nodiscard]] static constexpr std::optional<CharacterView> create(
        const_pointer begin, size_type count) noexcept {
        if (count > 4 ||
            !Zeus::UTF8::is_valid_character(begin, begin + count)) {
            return std::nullopt;
        }

        return CharacterView(begin, count);
    }

    template <std::contiguous_iterator It, std::sized_sentinel_for<It> End>
    requires std::same_as<std::iter_value_t<It>, value_type> 
    && (!std::convertible_to<End, size_type>)
    [[nodiscard]] static constexpr std::optional<CharacterView> create(
        It first, End last) noexcept(noexcept(last - first)) {
        if (last - first > 4 || !Zeus::UTF8::is_valid_character(first, last)) {
            return std::nullopt;
        }

        return CharacterView(first, last);
    }

    template <typename Range, typename DRange = std::remove_cvref_t<Range>>
    requires (!std::is_same_v<DRange, CharacterView>)
    && std::ranges::contiguous_range<Range>
    && std::ranges::sized_range<Range>
    && std::is_same_v<std::ranges::range_value_t<Range>, value_type>
    && (!std::is_convertible_v<Range, const_pointer>)
    [[nodiscard]] static constexpr std::optional<CharacterView> 
        create(Range&& range) noexcept {
        return CharacterView::create(std::ranges::begin(range),
                                     std::ranges::end(range));
    }

   private:
    size_type m_len;
    const_pointer m_begin;
};

[[nodiscard]] constexpr std::optional<Unicode::CodePoint> decode(
    UTF8::CharacterView character) noexcept {
    return Zeus::UTF8::decode(std::ranges::begin(character),
                              std::ranges::end(character));
}

[[nodiscard]] constexpr bool is_valid(UTF8::CharacterView character) noexcept {
    return Zeus::UTF8::is_valid_character(character);
}

}  // namespace UTF8

[[nodiscard]] constexpr UTF8::CharacterView::size_type size(
    UTF8::CharacterView character) noexcept {
    return static_cast<UTF8::CharacterView::size_type>(
        std::ranges::size(character));
}

[[nodiscard]] constexpr UTF8::CharacterView::const_reference at(
    UTF8::CharacterView character,
    typename UTF8::CharacterView::size_type index) {
    if (index > Zeus::size(character) || index < 0) {
        throw std::out_of_range("Index out of bounds.");
    }

    return character[index];
}

template <typename IntegerType>
[[nodiscard]] constexpr IntegerType to_integer(
    Zeus::UTF8::CharacterView character) noexcept {
    static_assert(std::is_integral_v<IntegerType>);

    return IntegerType(
        Memory::to_u32(std::begin(character), std::end(character)));
}

}  // namespace Zeus
