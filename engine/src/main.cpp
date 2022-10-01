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
#include "zeus/string/utf8/valid_character.hpp"
#include "zeus/string/utf8/string.hpp"
#include "zeus/string/utf8/updateable_character_iterator.hpp"

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
    // TODO(tristan):
    // Figure out why a valid UTF-8 string is failing the
    // Zeus::UTF8::is_valid_character_sequence test
    // $£€
    // Also Zeus::UTF8::String::create is throwing std::bad_optional_access a
    // lot - bug?
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

    /*
    for (auto& character : str) {
        if (character == u8"£") {
            std::cout << "Found symbol!\n";
            //character = u8"𐍈";
            std::cout << "So it's not this...\n";
        }
    }
    */

    /*
    Zeus::UTF8::UpdateableCharacterIterator<Zeus::UTF8::String> begin(
        str, str.code_unit_begin());
    
    for (; begin != str.code_unit_end(); ++begin) {
        if (*begin == u8"£") {
            std::cout << "Found symbol!\n";
            *begin = u8"€";
        }
    }

    println_utf8(str);
    */

    /*
    auto const* const u8_str_1{u8"£"};
    //auto const* const u8_str_2{u8"ह"};

    auto str_opt = Zeus::UTF8::String::create(u8_str_1);
    Zeus::UTF8::String str{std::move(str_opt.value())};

    print_debug(str);

    Zeus::UTF8::Character current_char{str.code_unit_cbegin(),
                                       str.code_unit_cend()};
    Zeus::UTF8::UpdateableCharacter<Zeus::UTF8::String> updateable_char(
        current_char, str, str.code_unit_begin());

    Zeus::UTF8::Character encoded_char{u8"ह"};
    updateable_char = encoded_char;

    print_debug(str);
    */

    /*
    std::cout << "Before insert: \n";
    print_debug(str);

    str.insert(str.code_unit_end(), u8_str_2);

    std::cout << "After insert: \n";
    print_debug(str);
    
    std::cout << "After replace: \n";
    auto end = std::ranges::next(str.code_unit_begin(), 2);
    std::u8string str_u8{u8"A"};
    str.replace(str.code_unit_begin(), end, str_u8.c_str(), str_u8.length());

    print_debug(str);
    
    std::u8string_view view{u8"cat"};
    str += u8"dog";

    std::cout << "After append: (dog)\n";
    print_debug(str);
    
    std::cout << "After str + cat\n";
    Zeus::UTF8::String new_str = str + u8"cat";
    print_debug(new_str);

    Zeus::UTF8::String utf8_7 = Zeus::to_utf8_string(7);
    print_debug(utf8_7);
    */

    /*
    Zeus::UTF8::Character replacement{
        Zeus::Unicode::CodePoint::g_replacementCharacter};
    Zeus::UTF8::ValidCharacter valid_char{replacement};
    */

    /*
    std::array<Zeus::UTF8::CodeUnit, 4> invalid_data = {0x80, 0xbf, 0x21, 0x21};
    Zeus::UTF8::ValidCharacter invalid_test{invalid_data};
    */

    /*
    std::cout << "Are equal? " << std::boolalpha << (valid_char == replacement)
              << '\n';

    std::cout << "Are equal? " << std::boolalpha << (replacement == valid_char)
              << '\n';

    auto const hwair = Zeus::UTF8::Character(Zeus::Unicode::CodePoint{0x10348});

    for (auto code_unit : hwair) {
        std::cout << "0x" << std::hex << static_cast<int>(code_unit) << '\n';
    }

    Zeus::UTF8::ValidCharacter test;

    std::cout << "0x" << std::hex << Zeus::to_integer<Zeus::i64>(replacement) << '\n';
    std::cout << "0x" << std::hex
              << Zeus::to_integer<Zeus::i64>(Zeus::UTF8::Character(Zeus::Unicode::CodePoint{0x10348}))
              << '\n';
    std::cout << "0x" << std::hex << Zeus::to_integer<Zeus::i64>(test) << '\n';

    auto const decoded = Zeus::UTF8::ValidCharacter::decode(test);
    std::cout << "0x" << std::hex << Zeus::to_integer<Zeus::i64>(decoded)
              << '\n';
    */

    /*
    std::array<Zeus::UTF8::CodeUnit, 4> data = {0xF0, 0x90, 0x8D, 0x88};
//    std::deque<Zeus::UTF8::CodeUnit> data = {0xF0, 0x90, 0x8D, 0x88};

    std::cout << "Is Valid? " << std::boolalpha
              << Zeus::UTF8::is_valid_character(data) << '\n';
    std::cout << "Length: " << Zeus::UTF8::calculate_length(data) << '\n';

    auto const code_point = Zeus::UTF8::decode(data);

    std::cout << std::hex << Zeus::to_integer<int>(code_point.value()) << '\n';
    */

    /*
    constexpr std::array<Zeus::Unicode::CodePoint, 6> code_points = {
        Zeus::Unicode::CodePoint{0x0024},
        Zeus::Unicode::CodePoint{0x00A3},
        Zeus::Unicode::CodePoint{0x0939},
        Zeus::Unicode::CodePoint{0x20AC},
        Zeus::Unicode::CodePoint{0xD55C},
        Zeus::Unicode::CodePoint{0x10348}
    };

    std::vector<Zeus::UTF8::CodeUnit> data(16);
    Zeus::UTF8::encode(code_points, std::ranges::begin(data));

    Zeus::UTF8::CharacterView view(std::ranges::begin(data) + 12,
                             std::ranges::end(data));

    std::cout << "Is valid? " << std::boolalpha << Zeus::UTF8::is_valid(view)
              << '\n';

    std::cout << "Full value: " << std::hex << Zeus::to_integer<Zeus::i32>(view)
              << '\n';

    std::cout << "Length: " << static_cast<int>(view.length()) << '\n';

    Zeus::UTF8::CharacterViewIterator iterator(std::ranges::begin(data));

    while (iterator != std::ranges::end(data)) {
        Zeus::UTF8::CharacterView  const character_view = *iterator;

        std::cout << "0x" << std::hex
                  << Zeus::to_integer<Zeus::i32>(character_view) << '\n';

        ++iterator;
    }

    --iterator;

    while (iterator != std::ranges::begin(data)) {
        Zeus::UTF8::CharacterView  const character_view = *iterator;

        std::cout << "0x" << std::hex
                  << Zeus::to_integer<Zeus::i32>(character_view) << '\n';

        --iterator;
    }

    for (auto byte : view) {
        std::cout << "0x" << std::hex << static_cast<int>(byte) << '\n';
    }

    Zeus::UTF8::CharacterViewIterator iterator(std::ranges::begin(data));

    while (iterator != std::ranges::end(data)) {
        auto const code_point_opt = Zeus::UTF8::decode(*iterator);
        auto const integer = Zeus::to_integer<int>(code_point_opt.value());

        std::cout << "0x" << std::hex << integer << '\n';
    }
    */


    return EXIT_SUCCESS;
}
