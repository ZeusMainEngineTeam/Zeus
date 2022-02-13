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

/**
 * Contains the endian byte order of this platform.
 *
 * Inspired by C++20's possible implementation.
 */
enum class Endian {
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
struct is_endian_value_type
    : std::bool_constant<std::is_same_v<T, u8> || std::is_same_v<T, u16> ||
                         std::is_same_v<T, u32> || std::is_same_v<T, u64> ||
                         std::is_same_v<T, std::byte>> {};

/**
 * Checks if the given type is supported by the endian operations.
 *
 * @note An alias for obtaining the static member function "value" from
 * is_endian_value_type.
 *
 * @see is_endian_value_type
 *
 * @tparam The type to check
 */
template <typename T>
inline constexpr bool is_endian_value_type_v =
    is_endian_value_type<T>::value;

/**
 * Gets the opposite endian encoding of the given Endian value.
 *
 * @param endian The endianness to get the opposite of
 *
 * @return The opposite endian encoding
 */
[[nodiscard]] ZEUS_FORCE_INLINE inline constexpr Endian getOtherEndian(
    Endian endian) noexcept {
    return (endian == Endian::Little) ? Endian::Big : Endian::Little;
}

/**
 * Reverses the byte-order of the given value.
 *
 * @param value The value to reverse
 *
 * @return The reverse byte-order of the given value
 */
[[nodiscard]] ZEUS_FORCE_INLINE inline constexpr u8 reverse(u8 value) noexcept {
    return ((value & 0x0F) << 4) | ((value & 0xF0) >> 4);
}

/**
 * Reverses the byte-order of the given value.
 *
 * @param value The value to reverse
 *
 * @return The reverse byte-order of the given value
 */
[[nodiscard]] ZEUS_FORCE_INLINE inline constexpr std::byte reverse(
    std::byte value) noexcept {
    return std::byte{reverse(std::to_integer<u8>(value))};
}

/**
 * Reverses the byte-order of the given value.
 *
 * @param value The value to reverse
 *
 * @return The reverse byte-order of the given value
 */
[[nodiscard]] ZEUS_FORCE_INLINE inline constexpr u16 reverse(
    u16 value) noexcept {
    return (value << 8) | (value >> 8);
}

/**
 * Reverses the byte-order of the given value.
 *
 * @param value The value to reverse
 *
 * @return The reverse byte-order of the given value
 */
[[nodiscard]] ZEUS_FORCE_INLINE inline constexpr u32 reverse(
    u32 value) noexcept {
    return (reverse(static_cast<u16>((value << 16) >> 16)) << 16) |
           reverse(static_cast<u16>(value >> 16));
}

/**
 * Reverses the byte-order of the given value.
 *
 * @param value The value to reverse
 *
 * @return The reverse byte-order of the given value
 */
[[nodiscard]] ZEUS_FORCE_INLINE inline constexpr u64 reverse(
    u64 value) noexcept {
    return (static_cast<u64>(reverse(static_cast<u32>((value << 24) >> 24)))
            << 32) |
           reverse(static_cast<u32>(value >> 32));
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
 * @param value         The value to reverse
 *
 * @return The reverse byte-order of the given value if endianness is different
 */
template <typename T>
[[nodiscard]] ZEUS_FORCE_INLINE inline constexpr T reverseIf(Endian source,
                                                             Endian destination,
                                                             T value) noexcept {
    static_assert(is_endian_value_type_v<T>, "Type is not supported.");

    if (source != destination) {
        return reverse(value);
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
 * @see Zeus::Memory::reverseIf
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
    return reverseIf(Endian::Little, Endian::Native, value);
}

/**
 * Converts the given value from big endian to the native endianness of the
 * machine.
 *
 * @note Will not perform the operation if the native endianness of the machine
 * is big endian. Instead, the value will be returned as is.
 *
 * @see Zeus::Memory::reverseIf
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
    return reverseIf(Endian::Big, Endian::Native, value);
}

/**
 * Converts the given value from the native endianness of the machine to little
 * endian.
 *
 * @note Will not perform the operation if the native endianness of the machine
 * is little endian. Instead, the value will be returned as is.
 *
 * @see Zeus::Memory::reverseIf
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
    return reverseIf(Endian::Native, Endian::Little, value);
}

/**
 * Converts the given value from the native endianness of the machine to big
 * endian.
 *
 * @note Will not perform the operation if the native endianness of the machine
 * is big endian. Instead, the value will be returned as is.
 *
 * @see Zeus::Memory::reverseIf
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
    return reverseIf(Endian::Native, Endian::Big, value);
}

}  // namespace Memory

/**
 * Returns a string representation of the given Endian value.
 *
 * @param endian The endian value to turn into a string representation
 *
 * @return The string representation of the given Endian value
 */
[[nodiscard]] ZEUS_FORCE_INLINE inline std::string to_string(
    Zeus::Memory::Endian endian) noexcept {
    return (endian == Zeus::Memory::Endian::Little) ? "Little" : "Big";
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
                                Zeus::Memory::Endian endian) noexcept {
    return stream << Zeus::to_string(endian);
}
