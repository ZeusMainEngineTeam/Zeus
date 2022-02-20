/**
 * This file is part of the Zeus Game Engine.
 * Copyright (C) 2021 Tristan F.
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

#include <cstddef>
#include <ostream>
#include <string>
#include <type_traits>

#include "zeus/core/compiler_macros.hpp"
#include "zeus/core/types.hpp"

/**
 *
 * @file endian.hpp
 *
 */

namespace Zeus {

namespace Memory {

namespace Endian {

/**
 * Contains the endian byte order of this platform.
 *
 * Inspired by C++20's possible implementation.
 */
enum class Type {
#if ZEUS_IS_MSVC
    Little = 0,
    Big = 1,
    Native = Little
#elif ZEUS_IS_GCC_OR_CLANG || defined(__BYTE_ORDER__)
    Little = __ORDER_LITTLE_ENDIAN__,
    Big = __ORDER_BIG_ENDIAN__,
    Native = __BYTE_ORDER__
#else
#error "Endianness not supported for this compiler."
#endif
};

/**
 * Checks if the given type is supported by the endian operations.
 *
 * @tparam The type to check
 */
template <typename T>
struct is_endian_swappable_type
    : std::bool_constant<std::is_same_v<T, u16> || std::is_same_v<T, u32> ||
                         std::is_same_v<T, u64>> {};

/**
 * Checks if the given type is supported by the endian operations.
 *
 * @note An alias for obtaining the static member variable "value" from
 * is_endian_swappable_type.
 *
 * @see is_endian_swappable_type
 *
 * @tparam The type to check
 */
template <typename T>
inline constexpr bool is_endian_swappable_type_v =
    is_endian_swappable_type<T>::value;

/**
 * Gets the opposite endian encoding of the given Endian value.
 *
 * @param endian The endianness to get the opposite of
 *
 * @return The opposite endian encoding
 */
[[nodiscard]] ZEUS_FORCE_INLINE inline constexpr Endian::Type getOtherEndian(
    Endian::Type endian) noexcept {
    return (endian == Endian::Type::Little) ? Endian::Type::Big
                                            : Endian::Type::Little;
}

/**
 * Reverses the byte-order of the given value.
 *
 * @param value The value to byte swap
 *
 * @return The swapped byte-order of the given value
 */
[[nodiscard]] ZEUS_FORCE_INLINE inline constexpr u16 byteSwap(
    u16 value) noexcept {
    return (value << 8) | (value >> 8);
}

/**
 * Reverses the byte-order of the given value.
 *
 * @param value The value to byte swap
 *
 * @return The swapped byte-order of the given value
 */
[[nodiscard]] ZEUS_FORCE_INLINE inline constexpr u32 byteSwap(
    u32 value) noexcept {
    return (byteSwap(static_cast<u16>((value << 16) >> 16)) << 16) |
           byteSwap(static_cast<u16>(value >> 16));
}

/**
 * Reverses the byte-order of the given value.
 *
 * @param value The value to byte swap
 *
 * @return The swapped byte-order of the given value
 */
[[nodiscard]] ZEUS_FORCE_INLINE inline constexpr u64 byteSwap(
    u64 value) noexcept {
    return (static_cast<u64>(byteSwap(static_cast<u32>((value << 24) >> 24)))
            << 32) |
           byteSwap(static_cast<u32>(value >> 32));
}

/**
 * Reverses the byte-order of the given value if the endianness of the given
 * source does not match the endianness of the given destination.
 *
 * @note If the given source is the same as the given destination then the given
 * value will be returned.
 *
 * @tparam T The type belonging to the value
 *
 * @param source        The endianness of the source
 * @param destination   The endianness of the destination
 * @param value         The value to byte swap
 *
 * @return The swapped byte-order of the given value if endianness is different
 */
template <typename T>
[[nodiscard]] ZEUS_FORCE_INLINE inline constexpr T byteSwapIf(
    Endian::Type source, Endian::Type destination, T value) noexcept {
    static_assert(is_endian_swappable_type_v<T>, "Type is not supported.");

    if (source != destination) {
        return byteSwap(value);
    }

    return value;
}

/**
 * Converts the given value from little endian to the native endianness of the
 * machine.
 *
 * @note Will not perform the operation if the native endianness of the machine
 * is little endian. Instead, the value will be returned as is.
 *
 * @see Zeus::Memory::byteSwapIf
 *
 * @tparam T The type of the given value
 *
 * @param value The value to convert to the native endianness of the machine
 *
 * @return The given value converted to the native endianness of the machine if
 * the original endianness was different
 */
template <typename T>
[[nodiscard]] ZEUS_FORCE_INLINE inline constexpr T littleToNative(
    T value) noexcept {
    return byteSwapIf(Endian::Type::Little, Endian::Type::Native, value);
}

/**
 * Converts the given value from big endian to the native endianness of the
 * machine.
 *
 * @note Will not perform the operation if the native endianness of the machine
 * is big endian. Instead, the value will be returned as is.
 *
 * @see Zeus::Memory::byteSwapIf
 *
 * @tparam T The type of the given value
 *
 * @param value The value to convert to the native endianness of the machine
 *
 * @return The given value converted to the native endianness of the machine if
 * the original endianness was different
 */
template <typename T>
[[nodiscard]] ZEUS_FORCE_INLINE inline constexpr T bigToNative(
    T value) noexcept {
    return byteSwapIf(Endian::Type::Big, Endian::Type::Native, value);
}

/**
 * Converts the given value from the native endianness of the machine to little
 * endian.
 *
 * @note Will not perform the operation if the native endianness of the machine
 * is little endian. Instead, the value will be returned as is.
 *
 * @see Zeus::Memory::byteSwapIf
 *
 * @tparam T The type of the given value
 *
 * @param value The value to convert to little endian
 *
 * @return The given value converted to little endian if the original
 * endianness was different
 */
template <typename T>
[[nodiscard]] ZEUS_FORCE_INLINE inline constexpr T nativeToLittle(
    T value) noexcept {
    return byteSwapIf(Endian::Type::Native, Endian::Type::Little, value);
}

/**
 * Converts the given value from the native endianness of the machine to big
 * endian.
 *
 * @note Will not perform the operation if the native endianness of the machine
 * is big endian. Instead, the value will be returned as is.
 *
 * @see Zeus::Memory::byteSwapIf
 *
 * @tparam T The type of the given value
 *
 * @param value The value to convert to big endian
 *
 * @return The given value converted to big endian if the original endianness
 * was different
 */
template <typename T>
[[nodiscard]] ZEUS_FORCE_INLINE inline constexpr T nativeToBig(
    T value) noexcept {
    return byteSwapIf(Endian::Type::Native, Endian::Type::Big, value);
}

}  // namespace Endian

}  // namespace Memory

/**
 * Returns a string representation of the given Endian value.
 *
 * @param endian The endian value to turn into a string representation
 *
 * @return The string representation of the given Endian value
 */
[[nodiscard]] ZEUS_FORCE_INLINE inline std::string to_string(
    Zeus::Memory::Endian::Type endian) noexcept {
    return (endian == Zeus::Memory::Endian::Type::Little) ? "Little" : "Big";
}

}  // namespace Zeus

/**
 * Sends a string representation of the given endian enumeration to the given
 * output stream.
 *
 * @param stream The output stream to send the endian string representation to
 * @param endian The endian enumeration to output to the given output stream
 *
 * @return A reference to the given output stream
 */
inline std::ostream& operator<<(std::ostream& stream,
                                Zeus::Memory::Endian::Type endian) noexcept {
    return stream << Zeus::to_string(endian);
}
