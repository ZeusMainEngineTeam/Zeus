#include "gtest/gtest.h"

#include <sstream>

#include "zeus/memory/endian.hpp"

/**
 * Tests for endian.hpp
 */
namespace {

constexpr Zeus::u16 const byte2 = 0x1122;
constexpr Zeus::u32 const byte4 = 0x11223344;
constexpr Zeus::u64 const byte8 = 0x1122334455667788;

constexpr Zeus::u16 const byte2_swap = 0x2211;
constexpr Zeus::u32 const byte4_swap = 0x44332211;
constexpr Zeus::u64 const byte8_swap = 0x8877665544332211;

constexpr auto const otherEndian =
    Zeus::Memory::Endian::getOtherEndian(Zeus::Memory::Endian::Type::Native);

TEST(endian_test, endian_enum) {
    using namespace Zeus::Memory;

    ASSERT_FALSE(Endian::Type::Little == Endian::Type::Big);
    ASSERT_TRUE(Endian::Type::Native == Endian::Type::Little ||
                Endian::Type::Native == Endian::Type::Big);
}

TEST(endian_test, is_endian_swappable_type_template) {
    using namespace Zeus::Memory;

    ASSERT_FALSE(Endian::is_endian_swappable_type_v<Zeus::u8>);
    ASSERT_TRUE(Endian::is_endian_swappable_type_v<Zeus::u16>);
    ASSERT_TRUE(Endian::is_endian_swappable_type_v<Zeus::u32>);
    ASSERT_TRUE(Endian::is_endian_swappable_type_v<Zeus::u64>);
}

TEST(endian_test, getOtherEndian) {
    using namespace Zeus::Memory;

    ASSERT_TRUE(Endian::getOtherEndian(Endian::Type::Little) ==
                Endian::Type::Big);
    ASSERT_TRUE(Endian::getOtherEndian(Endian::Type::Big) ==
                Endian::Type::Little);
}

TEST(endian_test, byteSwap) {
    using namespace Zeus::Memory;

    ASSERT_EQ(Endian::byteSwap(byte2), byte2_swap);
    ASSERT_EQ(Endian::byteSwap(byte4), byte4_swap);
    ASSERT_EQ(Endian::byteSwap(byte8), byte8_swap);
}

TEST(endian_test, byteSwapIf_no_swap) {
    using namespace Zeus::Memory;

    ASSERT_EQ(
        Endian::byteSwapIf(Endian::Type::Native, Endian::Type::Native, byte2),
        byte2);
    ASSERT_EQ(
        Endian::byteSwapIf(Endian::Type::Native, Endian::Type::Native, byte4),
        byte4);
    ASSERT_EQ(
        Endian::byteSwapIf(Endian::Type::Native, Endian::Type::Native, byte8),
        byte8);
}

TEST(endian_test, byteSwapIf_swap) {
    using namespace Zeus::Memory;

    ASSERT_EQ(Endian::byteSwapIf(Endian::Type::Native, otherEndian, byte2),
              byte2_swap);
    ASSERT_EQ(Endian::byteSwapIf(Endian::Type::Native, otherEndian, byte4),
              byte4_swap);
    ASSERT_EQ(Endian::byteSwapIf(Endian::Type::Native, otherEndian, byte8),
              byte8_swap);
}

TEST(endian_test, littleToNative) {
    using namespace Zeus::Memory;

    if (Endian::Type::Little == Endian::Type::Native) {
        ASSERT_EQ(Endian::littleToNative(byte2), byte2);
        ASSERT_EQ(Endian::littleToNative(byte4), byte4);
        ASSERT_EQ(Endian::littleToNative(byte8), byte8);
    } else {
        ASSERT_EQ(Endian::littleToNative(byte2), byte2_swap);
        ASSERT_EQ(Endian::littleToNative(byte4), byte4_swap);
        ASSERT_EQ(Endian::littleToNative(byte8), byte8_swap);
    }
}

TEST(endian_test, bigToNative) {
    using namespace Zeus::Memory;

    if (Endian::Type::Big == Endian::Type::Native) {
        ASSERT_EQ(Endian::bigToNative(byte2), byte2);
        ASSERT_EQ(Endian::bigToNative(byte4), byte4);
        ASSERT_EQ(Endian::bigToNative(byte8), byte8);
    } else {
        ASSERT_EQ(Endian::bigToNative(byte2), byte2_swap);
        ASSERT_EQ(Endian::bigToNative(byte4), byte4_swap);
        ASSERT_EQ(Endian::bigToNative(byte8), byte8_swap);
    }
}

TEST(endian_test, nativeToLittle) {
    using namespace Zeus::Memory;

    if (Endian::Type::Little == Endian::Type::Native) {
        ASSERT_EQ(Endian::nativeToLittle(byte2), byte2);
        ASSERT_EQ(Endian::nativeToLittle(byte4), byte4);
        ASSERT_EQ(Endian::nativeToLittle(byte8), byte8);
    } else {
        ASSERT_EQ(Endian::nativeToLittle(byte2), byte2_swap);
        ASSERT_EQ(Endian::nativeToLittle(byte4), byte4_swap);
        ASSERT_EQ(Endian::nativeToLittle(byte8), byte8_swap);
    }
}

TEST(endian_test, nativeToBig) {
    using namespace Zeus::Memory;

    if (Endian::Type::Big == Endian::Type::Native) {
        ASSERT_EQ(Endian::nativeToBig(byte2), byte2);
        ASSERT_EQ(Endian::nativeToBig(byte4), byte4);
        ASSERT_EQ(Endian::nativeToBig(byte8), byte8);
    } else {
        ASSERT_EQ(Endian::nativeToBig(byte2), byte2_swap);
        ASSERT_EQ(Endian::nativeToBig(byte4), byte4_swap);
        ASSERT_EQ(Endian::nativeToBig(byte8), byte8_swap);
    }
}

TEST(endian_test, to_string) {
    using namespace Zeus::Memory;

    if (Endian::Type::Big == Endian::Type::Native) {
        ASSERT_EQ(Zeus::to_string(Endian::Type::Native), "Big");
    } else {
        ASSERT_EQ(Zeus::to_string(Endian::Type::Little), "Little");
    }

    ASSERT_EQ(Zeus::to_string(Endian::Type::Little), "Little");
    ASSERT_EQ(Zeus::to_string(Endian::Type::Big), "Big");
}

TEST(endian_test, ostream) {
    using namespace Zeus::Memory;

    std::ostringstream native;
    native << Endian::Type::Native;

    std::ostringstream little;
    little << Endian::Type::Little;

    std::ostringstream big;
    big << Endian::Type::Big;

    if (Endian::Type::Big == Endian::Type::Native) {
        ASSERT_EQ(native.str(), "Big");
    } else {
        ASSERT_EQ(native.str(), "Little");
    }

    ASSERT_EQ(little.str(), "Little");
    ASSERT_EQ(big.str(), "Big");
}

}  // namespace
