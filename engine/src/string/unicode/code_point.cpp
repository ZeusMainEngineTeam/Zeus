#include "zeus/string/unicode/code_point.hpp"

#include <iomanip>
#include <sstream>

namespace Zeus {

inline namespace cpp20_v1 {

std::string to_string(Unicode::CodePoint point) {
    std::ostringstream stream;
    stream << "U+" << std::hex << Zeus::to_integer(point);

    return stream.str();
}

std::ostream& operator<<(std::ostream& stream, Unicode::CodePoint point) {
    return stream << Zeus::to_string(point);
}

}  // namespace cpp20_v1

}  // namespace Zeus
