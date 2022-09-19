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
#include <array>
#include <initializer_list>
#include <ranges>
#include <stdexcept>

#include "zeus/core/types.hpp"

namespace Zeus {

template <typename T, Zeus::ssize N>
class FixedVector {
   public:
    static_assert(N > 0, "Cannot allocate negative space.");

    using this_type = FixedVector<T, N>;

    using storage_type = std::array<T, N>;

    using value_type = typename storage_type::value_type;
    using reference = typename storage_type::reference;
    using const_reference = typename storage_type::const_reference;
    using pointer = typename storage_type::pointer;
    using const_pointer = typename storage_type::const_pointer;

    using size_type = Zeus::ssize;

    using iterator = typename storage_type::iterator;
    using const_iterator = typename storage_type::const_iterator;

    FixedVector() noexcept = default;

    template <std::input_iterator InputIt, std::sentinel_for<InputIt> Sentinel>
    constexpr FixedVector(InputIt first, Sentinel last) {
        // TODO(tristan): Implement narrow_cast and narrow and use it here
        auto const new_size =
            static_cast<size_type>(std::ranges::distance(first, last));

        this->check_new_size(new_size);

        this->m_size = new_size;
        std::ranges::copy(first, last, this->m_data);
    }

    constexpr FixedVector(std::ranges::input_range auto&& range)
        : FixedVector{std::ranges::begin(range), std::ranges::end(range)} {}

    [[nodiscard]] constexpr bool operator<=>(this_type const&) const noexcept =
        default;

    constexpr void push_back(const_reference value) {
        this->impl_push_back(value);
    }

    constexpr void push_back(value_type&& value) {
        this->impl_push_back(std::move(value));
    }

    [[nodiscard]] constexpr reference operator[](size_type index) noexcept {
        return this->m_data[index];
    }

    [[nodiscard]] constexpr const_reference operator[](
        size_type index) const noexcept {
        return this->m_data[index];
    }

    [[nodiscard]] constexpr size_type capacity() const noexcept { return N; }

    [[nodiscard]] constexpr size_type size() const noexcept {
        return this->m_size;
    }

    constexpr void reserve(size_type size) {
        if (size > this->capacity() || size < 0) {
            throw std::length_error("Invalid length.");
        }

        this->m_size = size;
    }

    [[nodiscard]] constexpr iterator begin() noexcept {
        return std::begin(this->m_data);
    }

    [[nodiscard]] constexpr const_iterator begin() const noexcept {
        return std::begin(this->m_data);
    }

    [[nodiscard]] constexpr const_iterator cbegin() const noexcept {
        return std::begin(this->m_data);
    }

    [[nodiscard]] constexpr iterator end() noexcept {
        return this->begin() + this->m_size;
    }

    [[nodiscard]] constexpr const_iterator end() const noexcept {
        return this->begin() + this->m_size;
    }

    [[nodiscard]] constexpr const_iterator cend() const noexcept {
        return this->cbegin() + this->m_size;
    }

   private:
    storage_type m_data;
    size_type m_size;

    inline constexpr void check_new_size(size_type new_size) {
        if (new_size > this->capacity()) {
            throw std::length_error("Over allocated capacity.");
        }
    }

    constexpr void impl_push_back(value_type&& value) {
        auto const new_size = this->m_size + 1;
        this->check_new_size(new_size);

        this->m_data[this->m_size] = std::move(value);

        this->m_size = new_size;
    }
};

template <typename T, Zeus::ssize N>
[[nodiscard]] constexpr bool is_empty(
    FixedVector<T, N> const& vector) noexcept {
    return std::ssize(vector) == 0;
}

template <typename T, Zeus::ssize N>
[[nodiscard]] constexpr decltype(auto) at(
    FixedVector<T, N> const& vector,
    typename FixedVector<T, N>::size_type index) {
    if (index > vector.size()) {
        throw std::out_of_range("Index out of bounds.");
    }

    return vector[index];
}

template <typename T, Zeus::ssize N>
[[nodiscard]] constexpr decltype(auto) at(
    FixedVector<T, N>& vector, typename FixedVector<T, N>::size_type index) {
    if (index > vector.size()) {
        throw std::out_of_range("Index out of bounds.");
    }

    return vector[index];
}

}  // namespace Zeus
