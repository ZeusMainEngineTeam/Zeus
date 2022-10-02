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

#include <array>
#include <iomanip>
#include <iostream>
#include <string>
#include <deque>
#include <vector>

//#include "zeus/string/utf8/algorithm.hpp"
//#include "zeus/string/utf8/iterator.hpp"
#include "zeus/string/utf8/string.hpp"

template <std::input_iterator InputIt, std::sentinel_for<InputIt> Sentinel>
void print_code_units(InputIt first, Sentinel last) {
    for (; first != last; ++first) {
        std::cout << "0x" << std::hex << static_cast<int>(*first) << '\n';
    }
}

template <std::input_iterator InputIt, std::sentinel_for<InputIt> Sentinel>
void print_utf8(InputIt first, Sentinel last) {
    for (; first != last; ++first) {
        std::wcout << static_cast<wchar_t>(*first);
    }
}

void print_utf8(Zeus::UTF8::code_unit_input_range auto&& range) {
    print_utf8(std::ranges::begin(range), std::ranges::end(range));
}

template <std::input_iterator InputIt, std::sentinel_for<InputIt> Sentinel>
void println_utf8(InputIt first, Sentinel last) {
    for (; first != last; ++first) {
        std::wcout << static_cast<wchar_t>(*first);
    }

    std::wcout << '\n';
}

void println_utf8(Zeus::UTF8::code_unit_input_range auto&& range) {
    println_utf8(std::ranges::begin(range), std::ranges::end(range));
}

void print_utf8(Zeus::UTF8::String const& str) {
    print_utf8(str.code_unit_cbegin(), str.code_unit_cend());
}

void println_utf8(Zeus::UTF8::String const& str) {
    println_utf8(str.code_unit_begin(), str.code_unit_cend());
}

void print_debug(Zeus::UTF8::String const& str) {
    std::cout << "===========================\n";
    std::cout << "Text: ";
    println_utf8(str);
    std::cout << std::dec << "UTF-8 Length: " << str.length() << '\n';
    std::cout << std::dec << "Code Unit Length: " << str.size_bytes() << '\n';
    std::cout << "Code Unit(s):\n";
    print_code_units(str.code_unit_begin(), str.code_unit_end());
    std::cout << "===========================\n";
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    auto const u8_str = std::u8string{u8"$£"};
    print_code_units(std::begin(u8_str), std::end(u8_str));
    //println_utf8(u8_str);

    std::cout << "Is valid? " << std::boolalpha
              << Zeus::UTF8::is_valid_character_sequence(u8_str) << '\n';

    auto str_opt = Zeus::UTF8::String::create(u8_str);
    auto str{std::move(str_opt.value())};
    // println_utf8(u8_str);
    
    for (auto character : str) {
        // Print out UTF-8 character....
        std::cout << "Size: " << static_cast<int>(character.size()) << '\n';
        println_utf8(character);
    }

    return EXIT_SUCCESS;
}
