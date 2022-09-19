#include "zeus/string/utf8/character.hpp"

namespace Zeus {

std::string to_string(UTF8::Character const& character) {
    return std::to_string(
        Zeus::to_integer<UTF8::Character::value_type>(character));
}

}  // namespace Zeus
