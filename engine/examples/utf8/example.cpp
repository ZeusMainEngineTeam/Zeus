#include <iostream>

#include "zeus/string/utf8.hpp"
#include "zeus/string/unicode.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    Zeus::Unicode::CodePoint const dollar_sign{0x0024};
    Zeus::UTF8::Character const encoded_dollar_sign{dollar_sign};
    Zeus::Unicode::CodePoint const decoded_dollar_sign =
        Zeus::UTF8::decode(encoded_dollar_sign);

    std::cout << "Dollar sign: ";
    std::cout << std::boolalpha << (dollar_sign == decoded_dollar_sign)
              << std::endl;
    std::cout << "Encoded Values: " << std::hex
              << Zeus::to_integer<Zeus::u32>(encoded_dollar_sign) << std::endl;

    std::cout << Zeus::UTF8::peek_char_size(encoded_dollar_sign[0])
              << std::endl;

    // ====================================================================== //

    Zeus::Unicode::CodePoint const pound_sign{0x00A3};
    Zeus::UTF8::Character const encoded_pound_sign{pound_sign};
    Zeus::Unicode::CodePoint const decoded_pound_sign =
        Zeus::UTF8::decode(encoded_pound_sign);

    std::cout << "Pound sign: ";
    std::cout << std::boolalpha << (pound_sign == decoded_pound_sign)
              << std::endl;
    std::cout << "Encoded Values: " << std::hex
              << Zeus::to_integer<Zeus::u32>(encoded_pound_sign) << std::endl;

    std::cout << Zeus::UTF8::peek_char_size(encoded_pound_sign[0]) << std::endl;

    // ====================================================================== //

    Zeus::Unicode::CodePoint const devanagari{0x0939};
    Zeus::UTF8::Character const encoded_devanagari{devanagari};
    Zeus::Unicode::CodePoint const decoded_devanagari =
        Zeus::UTF8::decode(encoded_devanagari);

    std::cout << "Devanagari: ";
    std::cout << std::boolalpha << (devanagari == decoded_devanagari)
              << std::endl;
    std::cout << "Encoded Values: " << std::hex
              << Zeus::to_integer<Zeus::u32>(encoded_devanagari) << std::endl;

    std::cout << Zeus::UTF8::peek_char_size(encoded_devanagari[0]) << std::endl;

    // ====================================================================== //

    Zeus::Unicode::CodePoint const euro_sign{0x20AC};
    Zeus::UTF8::Character const encoded_euro_sign{euro_sign};
    Zeus::Unicode::CodePoint const decoded_euro_sign =
        Zeus::UTF8::decode(encoded_euro_sign);

    std::cout << "Euro sign: ";
    std::cout << std::boolalpha << (euro_sign == decoded_euro_sign)
              << std::endl;
    std::cout << "Encoded Values: " << std::hex
              << Zeus::to_integer<Zeus::u32>(encoded_euro_sign) << std::endl;

    std::cout << Zeus::UTF8::peek_char_size(encoded_euro_sign[0]) << std::endl;

    // ====================================================================== //

    // Hangul Syllables
    Zeus::Unicode::CodePoint const hangul_syllables{0xD55C};
    Zeus::UTF8::Character const encoded_hangul_syllables{hangul_syllables};
    Zeus::Unicode::CodePoint const decoded_hangul_syllables =
        Zeus::UTF8::decode(encoded_hangul_syllables);

    std::cout << "Hangul Syllables: ";
    std::cout << std::boolalpha
              << (hangul_syllables == decoded_hangul_syllables) << std::endl;
    std::cout << "Encoded Values: " << std::hex
              << Zeus::to_integer<Zeus::u32>(encoded_hangul_syllables)
              << std::endl;

    std::cout << Zeus::UTF8::peek_char_size(encoded_hangul_syllables[0])
              << std::endl;

    // ====================================================================== //

    Zeus::Unicode::CodePoint const hwair{0x10348};
    Zeus::UTF8::Character const encoded_hwair{hwair};
    Zeus::Unicode::CodePoint const decoded_hwair =
        Zeus::UTF8::decode(encoded_hwair);

    std::cout << "Hwair: ";
    std::cout << std::boolalpha << (hwair == decoded_hwair) << std::endl;
    std::cout << "Encoded Values: " << std::hex
              << Zeus::to_integer<Zeus::u32>(encoded_hwair) << std::endl;

    std::cout << Zeus::UTF8::peek_char_size(encoded_hwair[0]) << std::endl;
    std::cout << Zeus::UTF8::is_continuation_byte(encoded_hwair[1])
              << std::endl;

    // ====================================================================== //

    return 0;
}
