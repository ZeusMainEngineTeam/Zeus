/**
 * This file is part of the Zeus Game Engine.
 * Copyright (C) 2021 Tristan F.
 *
 * The Zeus Game Engine is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The Zeus Game Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the Zeus Game Engine. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <cmath>
#include <stdexcept>

#include "zeus/core/assert.hpp"
#include "zeus/core/types.hpp"
#include "zeus/math/type_traits.hpp"

/**
 * @file vector_2d.hpp
 */

namespace Zeus {

namespace Math {

/**
 * A basic representation of a 2D vector.
 *
 * @tparam T The coordinate type for this vector.
 */
template <typename T>
struct BasicVector2D {
    using value_type = T;
    using reference = value_type&;
    using const_reference = value_type const&;
    using this_type = BasicVector2D<value_type>;
    using size_type = Zeus::ssize;

    /**
     * The x-coordinate in this vector.
     */
    value_type x;

    /**
     * The y-coordinate in this vector.
     */
    value_type y;

    /**
     * Returns a reference to an element inside this vector based on the given
     * index.
     *
     * @param position The position to retrieve an element inside this
     * vector
     *
     * @return a reference to the specified element
     */
    [[nodiscard]] constexpr reference operator[](size_type position) noexcept {
        ZEUS_ASSERT(position <= 1 && position >= 0);

        return (&(this->x))[position];
    }

    /**
     * Returns a constant reference to an element inside this vector based on
     * the given index.
     *
     * @param position The position to retrieve an element inside this
     * vector
     *
     * @return a constant reference to the specified element
     */
    [[nodiscard]] constexpr const_reference operator[](
        size_type position) const noexcept {
        ZEUS_ASSERT(position <= 1 && position >= 0);

        return (&(this->x))[position];
    }

    /**
     * Returns a vector with maximum values.
     *
     * @return
     */
    [[nodiscard]] static constexpr this_type max() noexcept {
        return this_type{std::numeric_limits<value_type>::max(),
                         std::numeric_limits<value_type>::max()};
    }

    /**
     * Returns a vector with minimum values.
     *
     * @return
     */
    [[nodiscard]] static constexpr this_type min() noexcept {
        return this_type{std::numeric_limits<value_type>::min(),
                         std::numeric_limits<value_type>::min()};
    }

    /**
     * Returns a vector with positive infinity for all values.
     *
     * @return A positive infinity vector
     */
    template <typename = Zeus::Math::enable_if_can_use_infinity_t<value_type>>
    [[nodiscard]] static constexpr this_type positive_infinity() noexcept {
        return this_type{std::numeric_limits<value_type>::infinity(),
                         std::numeric_limits<value_type>::infinity()};
    }

    /**
     * Returns a vector with negative infinity for all values.
     *
     * @return A negative infinity vector
     */
    template <typename = Zeus::Math::enable_if_can_use_infinity_t<value_type>>
    [[nodiscard]] static constexpr this_type negative_infinity() noexcept {
        return this_type{-1 * std::numeric_limits<value_type>::infinity(),
                         -1 * std::numeric_limits<value_type>::infinity()};
    }
};

/**
 * Compares the two given 2D vectors.
 *
 * @tparam T
 *
 * @param lhs
 * @param rhs
 *
 * @return true if they are the same, otherwise false
 */
template <typename T>
constexpr bool operator==(BasicVector2D<T> const& lhs,
                          BasicVector2D<T> const& rhs) noexcept {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

/**
 * Computes the dot product of the two given 2D vectors.
 *
 * @tparam T
 *
 * @param lhs
 * @param rhs
 *
 * @return
 */
template <typename T>
constexpr bool operator!=(BasicVector2D<T> const& lhs,
                          BasicVector2D<T> const& rhs) noexcept {
    return !(lhs == rhs);
}

/**
 * Adds the given vector to this vector.
 *
 * @param other The vector to add to this vector
 *
 * @return a reference to this vector
 */
template <typename T>
constexpr BasicVector2D<T>& operator+=(BasicVector2D<T>& lhs,
                                       BasicVector2D<T> const& rhs) noexcept {
    lhs.x += rhs.x;
    lhs.y += rhs.y;

    return lhs;
}

/**
 * Subtracts the given vector to this vector.
 *
 * @param other The vector to subtract from this vector
 *
 * @return a reference to this vector
 */
template <typename T>
constexpr BasicVector2D<T>& operator-=(BasicVector2D<T>& lhs,
                                       BasicVector2D<T> const& rhs) noexcept {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;

    return lhs;
}

/**
 * Multiplies this vector by the given vector.
 *
 * @param other The vector to multiply this vector
 *
 * @return a reference to this vector
 */
template <typename T>
constexpr BasicVector2D<T>& operator*=(BasicVector2D<T>& lhs,
                                       T scalar) noexcept {
    lhs.x *= scalar;
    lhs.y *= scalar;

    return lhs;
}

/**
 * Divides this vector by the given vector.
 *
 * @param other The vector to divide this vector
 *
 * @return a reference to this vector
 */
template <typename T>
constexpr BasicVector2D<T>& operator/=(BasicVector2D<T>& lhs,
                                       T scalar) noexcept {
    auto const scale = 1.0f / scalar;

    lhs.x *= scale;
    lhs.y *= scale;

    return lhs;
}

/**
 * Returns a reference to an element inside this vector.
 *
 * @param position The position to retrieve an element inside this
 * vector
 *
 * @throws std::out_of_range if the specified position is out of bounds
 *
 * @return a reference to the specified element
 */
template <typename T>
[[nodiscard]] constexpr typename BasicVector2D<T>::reference at(
    BasicVector2D<T>& vector, typename BasicVector2D<T>::size_type position) {
    if (position > 1 || position < 0) {
        throw std::out_of_range("Index out of bounds.");
    }

    return vector[position];
}

/**
 * Returns a constant reference to an element inside this vector.
 *
 * @param position The position to retrieve an element inside this
 * vector
 *
 * @throws std::out_of_range if the specified position is out of bounds
 *
 * @return a constant reference to the specified element
 */
template <typename T>
[[nodiscard]] constexpr typename BasicVector2D<T>::const_reference at(
    BasicVector2D<T> const& vector,
    typename BasicVector2D<T>::size_type position) {
    if (position > 1 || position < 0) {
        throw std::out_of_range("Index out of bounds.");
    }

    return vector[position];
}

/**
 * Adds two 2D vectors together.
 *
 * @tparam T The floating-point type for the vectors
 *
 * @param lhs The left-hand side of the expression
 * @param rhs The right-hand side of the expression
 *
 * @return a new 2D vector containing the sum
 */
template <typename T>
[[nodiscard]] constexpr BasicVector2D<T> operator+(
    BasicVector2D<T> const& lhs, BasicVector2D<T> const& rhs) noexcept {
    return BasicVector2D<T>{lhs.x + rhs.x, lhs.y + rhs.y};
}

/**
 * Subtracts two 2D vectors together.
 *
 * @tparam T The floating-point type for the vectors
 *
 * @param lhs The left-hand side of the expression
 * @param rhs The right-hand side of the expression
 *
 * @return A new 2D vector containing the difference
 */
template <typename T>
[[nodiscard]] constexpr BasicVector2D<T> operator-(
    BasicVector2D<T> const& lhs, BasicVector2D<T> const& rhs) noexcept {
    return BasicVector2D<T>{lhs.x - rhs.x, lhs.y - rhs.y};
}

/**
 * Multiplies a 2D vector by a scalar value.
 *
 * @tparam T The floating-point type for the vector
 *
 * @param vec    The vector to multiply
 * @param scalar The scalar to multiply the vector by
 *
 * @return A new 2D vector containing the product
 */
template <typename T>
[[nodiscard]] constexpr BasicVector2D<T> operator*(BasicVector2D<T> const& vec,
                                                   T scalar) noexcept {
    return BasicVector2D<T>{vec.x * scalar, vec.y * scalar};
}

/**
 * Multiplies a 2D vector by a scalar value.
 *
 * @tparam T The floating-point type for the vector
 *
 * @param scalar The scalar to multiply the vector by
 * @param vec    The vector to multiply
 *
 * @return A new 2D vector containing the product
 */
template <typename T>
[[nodiscard]] constexpr BasicVector2D<T> operator*(
    T scalar, BasicVector2D<T> const& vec) noexcept {
    return vec * scalar;
}

/**
 * Divides a 2D vector by a scalar value.
 *
 * @tparam T The floating-point type for the vector
 *
 * @param scalar The scalar to divide the vector by
 * @param vec    The vector to divide
 *
 * @return A new 2D vector containing the quotient
 */
template <typename T>
[[nodiscard]] constexpr BasicVector2D<T> operator/(BasicVector2D<T> const& vec,
                                                   T scalar) noexcept {
    T const scale = 1.0F / scalar;

    return BasicVector2D<T>{vec.x * scalar, vec.y * scalar};
}

/**
 * Flips the vector's values.
 *
 * @tparam T The floating-point type for the vector
 *
 * @param vec The vector to operate on
 *
 * @return A new 2D vector containing the product
 */
template <typename T>
[[nodiscard]] constexpr BasicVector2D<T> operator-(
    BasicVector2D<T> const& vec) noexcept {
    return BasicVector2D<T>{-vec.x, -vec.y};
}

/**
 * Returns the magnitude of the given 2D vector.
 *
 * @tparam T The floating-point type for the vector
 *
 * @param vec The vector to find its magnitude
 *
 * @return The magnitude
 */
template <typename T>
[[nodiscard]] constexpr T magnitude(BasicVector2D<T> const& vec) noexcept {
    // TODO: Replace with better version
    return std::hypot(vec.x, vec.y);
}

/**
 * Returns the normalization of the given 2D vector.
 *
 * @tparam T The floating-point type for the vector
 *
 * @param vec The vector to normalize
 *
 * @return The normalization
 */
template <typename T>
[[nodiscard]] constexpr T normalize(BasicVector2D<T> const& vec) noexcept {
    // TODO: Replace with better version
    return vec / magnitude(vec);
}

/**
 * Computes the dot product of the two given 2D vectors.
 *
 * @tparam T
 *
 * @param lhs
 * @param rhs
 *
 * @return
 */
template <typename T>
[[nodiscard]] constexpr T dot(BasicVector2D<T> const& lhs,
                              BasicVector2D<T> const& rhs) noexcept {
    return (lhs[0] * rhs[0]) + (lhs[1] * rhs[1]);
}

/**
 * An alias of a 32-bit 2D Vector.
 */
using Vector2D = BasicVector2D<f32>;

}  // namespace Math

}  // namespace Zeus
