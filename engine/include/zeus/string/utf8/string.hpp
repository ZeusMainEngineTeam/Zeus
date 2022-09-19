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

#include <algorithm>
#include <iterator>
#include <memory>
#include <optional>
#include <stdexcept>
#include <cstring>
#include <limits>

#include "zeus/core/types.hpp"
#include "zeus/string/unicode/code_point.hpp"
#include "zeus/string/utf8/algorithm.hpp"
#include "zeus/string/utf8/character.hpp"
#include "zeus/string/utf8/valid_character.hpp"
#include "zeus/string/utf8/character_iterator.hpp"

#include "zeus/string/utf8/updateable_character_iterator.hpp"

namespace Zeus {

/**
 * @file string/utf8.hpp
 */

namespace UTF8 {

// NOLINTNEXTLINE(readability-identifier-naming)
inline namespace cpp20_v1 {

/**
 * TODO(tristan): Come up with better name and/or casing.
 */
using ASCIIString = std::u8string;

/**
 * A representation of a valid UTF-8 string.
 *
 * @note This only offers sequential access when accessing UTF-8 characters.
 */
class String {
   public:
    using storage_type = std::u8string;

    using value_type = typename storage_type::value_type;
    using const_pointer = typename storage_type::const_pointer;
    using const_reference = typename storage_type::const_reference;

    using const_iterator = typename storage_type::const_iterator;
    using iterator = typename storage_type::iterator;
    using const_reverse_iterator =
        typename storage_type::const_reverse_iterator;
    using reverse_iterator = typename storage_type::reverse_iterator;

    using size_type = Zeus::ssize;

    /**
     * The max UTF-8 code unit limit that is possible to fit into this string.
     */
    static constexpr size_type g_nposCodeUnit =
        std::numeric_limits<size_type>::max();

    // ============================ CONSTRUCTORS ============================ //

    String() noexcept = default;
 
    // ============================= OPERATORS ============================== //

    [[nodiscard]] String operator+(const_pointer rhs);

    [[nodiscard]] String operator+(Zeus::UTF8::Character const& character);

    [[nodiscard]] String operator+(Zeus::UTF8::ValidCharacter character);

    [[nodiscard]] String operator+(String const& other);

    String& operator+=(const_pointer cstr);

    String& operator+=(Zeus::UTF8::Character const& character);

    String& operator+=(Zeus::UTF8::ValidCharacter character);

    String& operator+=(String const& other);

    // ============================== CREATION ============================== //

    [[nodiscard]] static std::optional<String> create(std::u8string str);

    template <Zeus::UTF8::code_unit_input_iterator Iterator,
              std::sentinel_for<Iterator> Sentinel>
    [[nodiscard]] static std::optional<String> create(Iterator first,
                                                      Sentinel last) {
        if (Zeus::UTF8::is_valid_character_sequence(first, last)) {
            return Zeus::UTF8::String{first, last};
        }

        return std::nullopt;
    }

    template <typename Range, typename DRange = std::remove_cvref_t<Range>>
    requires (!std::is_same_v<DRange, std::u8string_view> 
            && !std::is_same_v<DRange, std::u8string>)
    && Zeus::UTF8::code_unit_input_range<Range>
    && (!std::is_convertible_v<Range, const_pointer>)
    [[nodiscard]] static std::optional<String> create(
        Zeus::UTF8::code_unit_input_range auto&& range) {
        return Zeus::UTF8::String::create(std::ranges::begin(range),
                                          std::ranges::end(range));
    }

    // ============================== ENCODING ============================== //

    template <std::input_iterator InputIt, std::sentinel_for<InputIt> Sentinel>
    [[nodiscard]] static std::optional<String> encode(InputIt first,
                                                      Sentinel last) {
        // Make sure data is valid Unicode
        if (std::ranges::all_of(first, last, Zeus::Unicode::is_valid)) {
            return Zeus::UTF8::String{first, last};
        }

        return std::nullopt;
    }

    template <std::ranges::input_range Range>
    [[nodiscard]] static std::optional<String> encode(Range&& range) {
        return Zeus::UTF8::String::encode(std::ranges::begin(range),
                                          std::ranges::end(range));
    }

    // ============================== ITERATORS ============================= //

    [[nodiscard]] const_iterator begin() const noexcept;

    [[nodiscard]] const_iterator cbegin() const noexcept;

    [[nodiscard]] const_iterator end() const noexcept;

    [[nodiscard]] const_iterator cend() const noexcept;

    [[nodiscard]] const_reverse_iterator rbegin() const noexcept;

    [[nodiscard]] const_reverse_iterator crbegin() const noexcept;

    [[nodiscard]] const_reverse_iterator rend() const noexcept;

    [[nodiscard]] const_reverse_iterator crend() const noexcept;

    // ============================== CAPACITY ============================== //

    [[nodiscard]] size_type size() const noexcept;

    /**
     * Returns the size in bytes.
     */
    [[nodiscard]] size_type size_bytes() const noexcept;

    [[nodiscard]] size_type length() const noexcept;

    void reserve(size_type new_cap);

    [[nodiscard]] size_type capacity() const noexcept;

    void shrink_to_fit();

    // ============================= OPERATIONS ============================= //

    void swap(Zeus::UTF8::String& other) noexcept;

    [[nodiscard]] const_pointer c_str() const noexcept;

    [[nodiscard]] const_pointer data() const noexcept;

    // =============================== INSERT =============================== //

    String& insert(const_iterator position, const_pointer cstr,
                   size_type count);

    Zeus::UTF8::String& insert(const_iterator position,
                               String const& other);

    String& insert(const_iterator position,
                   Zeus::UTF8::ValidCharacter character);

    template <Zeus::UTF8::code_unit_input_iterator InputIt,
              std::sentinel_for<InputIt> Sentinel>
    String& insert(const_iterator position, InputIt first,
                   Sentinel last) {
        return this->insert_if_impl(position, first, last,
                                    Zeus::UTF8::calculate_length(first, last));
    }

    String& insert(const_iterator position,
                   Zeus::UTF8::code_unit_input_range auto&& range) {
        return this->insert(position, std::ranges::begin(range),
                            std::ranges::end(range));
    }

    // ================================ ERASE =============================== //

    std::optional<iterator> erase(const_iterator first, const_iterator last);

    std::optional<iterator> erase(const_iterator position);

    void clear() noexcept;

    // =============================== REPLACE ============================== //

    String& replace(const_iterator first, const_iterator last,
                    const_pointer cstr, size_type count);

    String& replace(const_iterator first, const_iterator last,
                    String const& other);

    String& replace(const_iterator first, const_iterator last,
                    ValidCharacter character);

    /**
     * Replaces the current valid UTF-8 character(s) with the given UTF-8
     * replacement character(s).
     *
     * @note This may cause allocations or deallocations. A single UTF-8
     * character can contain between 1 to 4 code units (8-bits each).
     *
     * @tparam InputIt
     * @tparam Sentinel
     *
     * @param first
     * @param last
     * @param first2
     * @param last2
     *
     * @return
     */
    template <Zeus::UTF8::code_unit_input_iterator InputIt,
              std::sentinel_for<InputIt> Sentinel>
    String& replace(const_iterator first,
                    const_iterator last, InputIt first2,
                    Sentinel last2) {
        // Check if ranges are not valid UTF-8 characters
        if (!Zeus::UTF8::is_valid_character_sequence(first, last) ||
            !Zeus::UTF8::is_valid_character_sequence(first2, last2)) {
            return *this;
        }

        return this->replace_impl(first, last, first2, last2);
    }

    String& replace(const_iterator first,
                    const_iterator last,
                    Zeus::UTF8::code_unit_input_range auto&& range) {
        return this->replace(first, last, std::ranges::begin(range),
                             std::ranges::end(range));
    }

   private:
    /**
     * Contains the code units.
     */
    std::u8string m_data;

    /**
     * The UTF-8 character length.
     */
    size_type m_len{};

    // ============================= CONSTRUCTOR ============================ //

    String(String const& other, size_type index, size_type count)
        : m_data{other.m_data, static_cast<storage_type::size_type>(index),
                 static_cast<storage_type::size_type>(count)},
          m_len{Zeus::UTF8::calculate_length(other.m_data)} {}

    explicit String(std::u8string str)
        : m_data{std::move(str)},
          m_len{Zeus::UTF8::calculate_length(this->m_data)} {}

    template <Zeus::UTF8::code_unit_input_iterator InputIt,
              std::sentinel_for<InputIt> Sentinel>
    constexpr String(InputIt first, Sentinel last)
        : m_data{first, last},
          m_len{Zeus::UTF8::calculate_length(first, last)} {}

    constexpr explicit String(Zeus::UTF8::code_unit_input_range auto&& range)
        : String{std::ranges::begin(range), std::ranges::end(range)} {}

    template <std::input_iterator InputIt, std::sentinel_for<InputIt> Sentinel>
    constexpr String(InputIt first, Sentinel last)
        : m_data{}, m_len{Zeus::UTF8::encode_length(first, last)} {
        this->m_data.reserve(this->m_len);

        Zeus::UTF8::encode(first, last, this->m_data);
    }

    constexpr String(std::ranges::input_range auto&& range)
        : String{std::ranges::begin(range), std::ranges::end(range)} {}

    // =============================== INSERT =============================== //

    [[nodiscard]] bool is_valid_insert_pos(const_iterator position) const;

    String& insert_impl(const_iterator position, const_pointer cstr,
                        size_type count, size_type utf8_count);

    String& insert_if_impl(const_iterator position, const_pointer cstr,
                           size_type count, size_type utf8_count);

    template <Zeus::UTF8::code_unit_input_iterator InputIt,
              std::sentinel_for<InputIt> Sentinel>
    String& insert_impl(const_iterator position, InputIt first, Sentinel last,
                        size_type utf8_count) {
        this->m_len += utf8_count;
        this->m_data.insert(position, first, last);

        return *this;
    }

    String& insert_impl(const_iterator position,
                        Zeus::UTF8::code_unit_input_range auto&& range,
                        size_type utf8_count) {
        this->insert_impl(position, std::ranges::begin(range),
                          std::ranges::end(range), utf8_count);

        return *this;
    }

    template <Zeus::UTF8::code_unit_input_iterator InputIt,
              std::sentinel_for<InputIt> Sentinel>
    String& insert_if_impl(const_iterator position, InputIt first,
                           Sentinel last, size_type utf8_count) {
        if (this->is_valid_insert_pos(position) &&
            Zeus::UTF8::is_valid_character_sequence(first, last)) {
            this->insert_impl(position, first, last, utf8_count);
        }

        return *this;
    }

    String& insert_if_impl(const_iterator position,
                           Zeus::UTF8::code_unit_input_range auto&& range,
                           size_type utf8_count) {
        this->insert_if_impl(position, std::ranges::begin(range),
                             std::ranges::end(range), utf8_count);

        return *this;
    }

    // ================================ ERASE =============================== //

    iterator erase_impl(const_iterator first, const_iterator last);

    // =============================== REPLACE ============================== //

    template <Zeus::UTF8::code_unit_input_iterator InputIt,
              std::sentinel_for<InputIt> Sentinel>
    String& replace_impl(const_iterator first, const_iterator last,
                         InputIt first2, Sentinel last2) {
        using Zeus::UTF8::calculate_length;

        auto const old_utf8_length = calculate_length(first, last);
        auto const new_utf8_length = calculate_length(first2, last2);

        this->m_len += new_utf8_length - old_utf8_length;
        this->m_data.replace(first, last, first2, last2);

        return *this;
    }

    String& replace_impl(const_iterator first, const_iterator last,
                         Zeus::UTF8::code_unit_input_range auto&& range) {
        return this->replace_impl(first, last, std::ranges::begin(range),
                                  std::ranges::end(range));
    }
};

}  // namespace cpp20_v1

}  // namespace UTF8

// ================================= INSERT ================================= //

Zeus::UTF8::String& insert(Zeus::UTF8::String& string,
                           Zeus::UTF8::String::const_iterator position,
                           Zeus::UTF8::String::const_pointer str);

Zeus::UTF8::String& insert(Zeus::UTF8::String& string,
                           Zeus::UTF8::String::const_iterator position,
                           Zeus::Unicode::CodePoint code_point);

Zeus::UTF8::String& insert(Zeus::UTF8::String& string,
                           Zeus::UTF8::String::const_iterator position,
                           Zeus::UTF8::ValidCharacter character);

Zeus::UTF8::String& insert(Zeus::UTF8::String& string,
                           Zeus::UTF8::String::size_type index,
                           Zeus::UTF8::String::const_pointer cstr,
                           Zeus::UTF8::String::size_type count);

Zeus::UTF8::String& insert(Zeus::UTF8::String& string,
                           Zeus::UTF8::String::size_type index,
                           Zeus::UTF8::String const& other);

Zeus::UTF8::String& insert(Zeus::UTF8::String& string,
                           Zeus::UTF8::String::size_type index,
                           Zeus::UTF8::ValidCharacter character);

template <Zeus::UTF8::code_unit_input_iterator InputIt,
          std::sentinel_for<InputIt> Sentinel>
Zeus::UTF8::String& insert(Zeus::UTF8::String& string,
                           Zeus::UTF8::String::const_iterator position,
                           InputIt first, Sentinel last) {
    return string.insert(position, first, last);
}

Zeus::UTF8::String& insert(Zeus::UTF8::String& string,
                           Zeus::UTF8::String::const_iterator position,
                           Zeus::UTF8::code_unit_input_range auto&& range) {
    return Zeus::insert(string, position, std::ranges::begin(range),
                        std::ranges::end(range));
}

template <Zeus::UTF8::code_unit_input_iterator InputIt,
          std::sentinel_for<InputIt> Sentinel>
Zeus::UTF8::String& insert(Zeus::UTF8::String& string,
                           Zeus::UTF8::String::size_type index, InputIt first,
                           Sentinel last) {
    return string.insert(std::ranges::next(string.cbegin(), index), first,
                         last);
}

Zeus::UTF8::String& insert(Zeus::UTF8::String& string,
                           Zeus::UTF8::String::size_type index,
                           Zeus::UTF8::code_unit_input_range auto&& range) {
    return Zeus::insert(string, index, std::ranges::begin(range),
                        std::ranges::end(range));
}

// ================================= APPEND ================================= //

Zeus::UTF8::String& append(Zeus::UTF8::String& str,
                           Zeus::UTF8::String::const_pointer cstr);

Zeus::UTF8::String& append(Zeus::UTF8::String& str,
                           Zeus::UTF8::String::const_pointer cstr,
                           Zeus::UTF8::String::size_type size);

Zeus::UTF8::String& append(Zeus::UTF8::String& str,
                           Zeus::Unicode::CodePoint code_point);

Zeus::UTF8::String& append(Zeus::UTF8::String& str,
                           Zeus::UTF8::Character const& character);

Zeus::UTF8::String& append(Zeus::UTF8::String& str,
                           Zeus::UTF8::ValidCharacter character);

Zeus::UTF8::String& append(Zeus::UTF8::String& str,
                           Zeus::UTF8::String const& other);

template <Zeus::UTF8::code_unit_input_iterator InputIt,
          std::sentinel_for<InputIt> Sentinel>
Zeus::UTF8::String& append(Zeus::UTF8::String& str, InputIt first,
                           Sentinel last) {
    return str.insert(str.cend(), first, last);
}

template <typename Range, typename DRange = std::remove_cvref_t<Range>>
requires((!std::is_same_v<DRange, std::u8string_view>) 
        && Zeus::UTF8::code_unit_input_range<Range>)
Zeus::UTF8::String& append(Zeus::UTF8::String& str, Range&& range) {
    return Zeus::append(str, std::ranges::begin(range),
                        std::ranges::end(range));
}

// ================================= ERASE ================================== //

std::optional<Zeus::UTF8::String::iterator> erase(
    Zeus::UTF8::String& str, Zeus::UTF8::String::size_type index);

std::optional<Zeus::UTF8::String::iterator> erase(
    Zeus::UTF8::String& str, Zeus::UTF8::String::size_type index,
    Zeus::UTF8::String::size_type count = Zeus::UTF8::String::g_nposCodeUnit);

void clear(Zeus::UTF8::String& string) noexcept;

// ================================= SUBSTR ================================= //

[[nodiscard]] std::optional<Zeus::UTF8::String> substr(
    Zeus::UTF8::String const& other, Zeus::UTF8::String::size_type index,
    Zeus::UTF8::String::size_type count = Zeus::UTF8::String::g_nposCodeUnit);

[[nodiscard]] bool is_empty(Zeus::UTF8::String const& string) noexcept;

[[nodiscard]] Zeus::UTF8::String::size_type length(
    Zeus::UTF8::String const& string) noexcept;

template <typename T>
[[nodiscard]] Zeus::UTF8::String to_utf8_string(T value) {
    auto const std_str = std::to_string(value);
    // TODO(tristan): Figure out a workaround to avoid having to run everything
    // through the "encode" member function. If the contents of the std string
    // are just ASCII then the string is already encoded.
    return Zeus::UTF8::String::encode(std_str).value();
}

}  // namespace Zeus

/*
namespace std {

template <>
struct hash<Zeus::UTF8::String> {
    std::size_t operator()(Zeus::UTF8::String const& str) const {
        return std::hash<std::u8string>();
    }
};

}  // namespace std
*/
