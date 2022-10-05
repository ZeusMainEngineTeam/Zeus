#include "zeus/string/unicode/code_point.hpp"

namespace Zeus {

std::string to_string(Zeus::Unicode::CodePoint point) noexcept {
    return std::to_string(
        Zeus::to_integer<Unicode::CodePoint::value_type>(point));
}

}  // namespace Zeus
