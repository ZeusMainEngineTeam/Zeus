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
#include <iterator>
#include <string_view>
#include <variant>
#include <optional>
#include <string>
#include <vector>
#include <type_traits>

#include "zeus/core/types.hpp"

/**
 * EBNF For Command Line Interface
 *
 * <letter>         ::= 'a' | 'b' | 'c' | 'd' | 'e' | 'f' | 'g' | 'h' | 'i' |
 *                      'j' | 'k' | 'l' | 'm' | 'n' | 'o' | 'p' | 'q' | 'r' |
 *                      's' | 't' | 'u' | 'v' | 'w' | 'x' | 'y' | 'z' | 'A' |
 *                      'B' | 'C' | 'D' | 'E' | 'F' | 'G' | 'H' | 'I' | 'J' |
 *                      'K' | 'L' | 'M' | 'N' | 'O' | 'P' | 'Q' | 'R' | 'S' |
 *                      'T' | 'U' | 'V' | 'W' | 'X' | 'Y' | 'Z' ;
 *
 * <space>          ::= ' '
 *
 * <digit>          ::= '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' |
 *                      '9' ;
 *
 * <bool>           ::= true | false ;
 *
 * <string>         ::= '"' ,   , '"' ;
 *
 * <decimal>        ::= { digit }, [, '.', { digit } ]
 *
 * <number>         ::= 
 *
 * <value>          ::= <bool> | <string> | <number> ;
 *
 * <short_option>   ::= '-' , { letter }
 *
 * <long_option>    ::= '--', identifier [, [ <space>, ] <value> ] ;
 */

namespace Zeus {

namespace CLI {

using OptionValue = std::variant<std::monostate, long double, 
                                unsigned long long int, char*, bool>;

enum class TokenType {
    eString,
    eBoolean,
    eInteger,
    eDecimal,
    eSymbol,
    eIdentifier,
    eLongOption,
    eShortOption,
    eUnknown
};

namespace Internal {

struct Token {
    TokenType type;
    std::string text;
};

} // namespace Internal

struct BasicOption {
    std::string name;
    std::string value;
};

struct Option {
    std::string full_name;
    std::optional<char> short_name;
    std::optional<OptionValue> value;
    bool required = false;
};

struct Settings {
    Zeus::ssize long_option_max_name_length = 25;
    Zeus::ssize max_value_length = 100;
};

struct CommandLine {
    std::string program_name;
    std::vector<BasicOption> options;
    std::vector<std::string> operands;
};

[[nodiscard]] CommandLine parseCommandLine(int argc, char* argv[]) noexcept;

} // namespace CLI

std::string to_string(CLI::Option const& option) noexcept;

} // namespace Zeus


















