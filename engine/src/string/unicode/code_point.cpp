#include "zeus/string/unicode/code_point.hpp"

namespace Zeus {

namespace Unicode {

constexpr CodePoint CodePoint::g_max{CodePoint::g_rawMax};
constexpr CodePoint CodePoint::g_min{CodePoint::g_rawMin};
constexpr CodePoint CodePoint::g_replacementCharacter{0xFFFDU};

}  // namespace Unicode

std::string to_string(Zeus::Unicode::CodePoint point) noexcept {
    return std::to_string(
        Zeus::to_integer<Unicode::CodePoint::value_type>(point));
}

}  // namespace Zeus
