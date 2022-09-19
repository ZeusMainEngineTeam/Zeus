#include "zeus/string/utf8/string.hpp"

namespace Zeus {

namespace UTF8 {

inline namespace cpp20_v1 {

// ================================ OPERATORS =============================== //

String String::operator+(Zeus::UTF8::String::const_pointer rhs) {
    Zeus::UTF8::String new_str{*this};

    std::u8string_view const view{rhs};
    new_str.insert(new_str.cend(), view);

    return new_str;
}

String String::operator+(Zeus::UTF8::Character const& character) {
    Zeus::UTF8::String new_str{*this};
    new_str.insert(new_str.cend(), character);

    return new_str;
}

String String::operator+(Zeus::UTF8::ValidCharacter character) {
    Zeus::UTF8::String new_str{*this};
    new_str.insert(new_str.cend(), character);

    return new_str;
}

String String::operator+(String const& other) {
    Zeus::UTF8::String new_str{*this};
    new_str.insert(new_str.cend(), other);

    return new_str;
}

String& String::operator+=(Zeus::UTF8::String::const_pointer cstr) {
    std::u8string_view const view{cstr};
    return this->insert(this->cend(), view);
}

String& String::operator+=(Zeus::UTF8::Character const& character) {
    return this->insert(this->cend(), character);
}

String& String::operator+=(Zeus::UTF8::ValidCharacter character) {
    return this->insert(this->cend(), character);
}

String& String::operator+=(Zeus::UTF8::String const& other) {
    return this->insert(this->cend(), other);
}

// ================================ CREATION ================================ //

/*
std::optional<String> String::create(String const& other,
                                     String::size_type index,
                                     String::size_type count) {
    // TODO(tristan): May have to update to use custom Zeus::size
    auto const size = std::ssize(other);

    // Preconditions
    ZEUS_ASSERT(index >= 0 && index < size, "Index is out of bounds.");
    ZEUS_ASSERT(count >= 0, "Count cannot be negative.");

    auto const max_count = size - index;
    auto const actual_count = std::min(max_count, count);

    auto const begin = std::ranges::next(other.cbegin(), index);
    auto const end = std::ranges::next(begin, actual_count);

    if (Zeus::UTF8::is_valid_character_sequence(begin, end)) {
        return Zeus::UTF8::String{other, index, count};
    }

    return std::nullopt;
}
*/

std::optional<String> String::create(std::u8string str) {
    if (Zeus::UTF8::is_valid_character_sequence(str)) {
        return Zeus::UTF8::String{std::move(str)};
    }

    return std::nullopt;
}

// ================================ ITERATORS =============================== //

String::const_iterator String::begin() const noexcept {
    return std::begin(this->m_data);
}

String::const_iterator String::cbegin() const noexcept {
    return std::cbegin(this->m_data);
}

String::const_iterator String::end() const noexcept {
    return std::end(this->m_data);
}

String::const_iterator String::cend() const noexcept {
    return std::cend(this->m_data);
}

String::const_reverse_iterator String::rbegin() const noexcept {
    return std::rbegin(this->m_data);
}

String::const_reverse_iterator String::crbegin() const noexcept {
    return std::crbegin(this->m_data);
}

String::const_reverse_iterator String::rend() const noexcept {
    return std::rend(this->m_data);
}

String::const_reverse_iterator String::crend() const noexcept {
    return std::crend(this->m_data);
}

// ================================ CAPACITY ================================ //

String::size_type String::size() const noexcept {
    return std::ssize(this->m_data);
}

String::size_type String::size_bytes() const noexcept {
    return std::ssize(this->m_data);
}

String::size_type String::length() const noexcept { return this->m_len; }

void String::reserve(String::size_type new_cap) {
    this->m_data.reserve(new_cap);
}

String::size_type String::capacity() const noexcept {
    return static_cast<size_type>(this->m_data.capacity());
}

void String::shrink_to_fit() { this->m_data.shrink_to_fit(); }

// =============================== OPERATIONS =============================== //

void String::swap(Zeus::UTF8::String& other) noexcept {
    std::swap(this->m_len, other.m_len);
    this->m_data.swap(other.m_data);
}

String::const_pointer String::c_str() const noexcept {
    return this->m_data.c_str();
}

String::const_pointer String::data() const noexcept {
    return std::data(this->m_data);
}

// ================================= INSERT ================================= //

String& String::insert(String::const_iterator position,
                       String::const_pointer cstr, String::size_type count) {
    return this->insert_if_impl(
        position, cstr, count,
        Zeus::UTF8::calculate_length(cstr, cstr + count));
}

Zeus::UTF8::String& String::insert(String::const_iterator position,
                                   String const& other) {
    return this->insert_impl(position, other.m_data, other.m_len);
}

String& String::insert(String::const_iterator position,
                       Zeus::UTF8::ValidCharacter character) {
    return this->insert_impl(position, character, std::ssize(character));
}

// ================================= ERASE ================================== //

std::optional<String::iterator> String::erase(
    String::const_iterator first,
    String::const_iterator last) {
    if (Zeus::UTF8::is_valid_character_sequence(first, last)) {
        return this->erase_impl(first, last);
    }

    return std::nullopt;
}

std::optional<String::iterator> String::erase(
    String::const_iterator position) {
    if (this->is_valid_insert_pos(position)) {
        return this->erase_impl(position, Zeus::UTF8::next(position));
    }

    return std::nullopt;
}

void String::clear() noexcept {
    this->m_data.clear();
    this->m_len = 0;
}

// ================================ REPLACE ================================= //

String& String::replace(String::const_iterator first,
                        String::const_iterator last,
                        String::const_pointer cstr, String::size_type count) {
    return this->replace_impl(first, last, cstr,
                              std::ranges::next(cstr, count));
}

String& String::replace(String::const_iterator first,
                        String::const_iterator last,
                        String const& other) {
    return this->replace_impl(first, last, other.m_data);
}

String& String::replace(String::const_iterator first,
                        String::const_iterator last,
                        ValidCharacter character) {
    return this->replace_impl(first, last, character);
}

/*
 * =============================================================================
 *                                   PRIVATE
 * =============================================================================
 */

// ================================= INSERT ================================= //

bool String::is_valid_insert_pos(
    String::const_iterator position) const {
    return Zeus::UTF8::is_leading_byte(*position) ||
           position == std::cend(this->m_data);
}

String& String::insert_impl(String::const_iterator position,
                            String::const_pointer cstr,
                            String::size_type count,
                            String::size_type utf8_count) {
    this->m_len += utf8_count;
    this->m_data.insert(position, cstr,
                        std::ranges::next(cstr, count));

    return *this;
}

String& String::insert_if_impl(String::const_iterator position,
                               String::const_pointer cstr,
                               String::size_type count,
                               String::size_type utf8_count) {
    if (this->is_valid_insert_pos(position) &&
        Zeus::UTF8::is_valid_character_sequence(cstr, cstr + count)) {
        this->insert_impl(position, cstr, count, utf8_count);
    }

    return *this;
}

// ================================= ERASE ================================== //

String::iterator String::erase_impl(
    String::const_iterator first,
    String::const_iterator last) {
    this->m_len -= Zeus::UTF8::calculate_length(first, last);
    return this->m_data.erase(first, last);
}

}  // namespace cpp20_v1

}  // namespace UTF8

// ================================= INSERT ================================= //

Zeus::UTF8::String& insert(
    Zeus::UTF8::String& string,
    Zeus::UTF8::String::const_iterator position,
    Zeus::UTF8::String::const_pointer str) {
    // Quick way to get std::begin & std::end
    std::u8string_view const view{str};

    using size_type = Zeus::UTF8::String::size_type;

    return string.insert(position, str, static_cast<size_type>(view.size()));
}

Zeus::UTF8::String& insert(
    Zeus::UTF8::String& string,
    Zeus::UTF8::String::const_iterator position,
    Zeus::Unicode::CodePoint code_point) {
    auto const str = Zeus::UTF8::encode(code_point);
    return string.insert(position, std::ranges::cbegin(str),
                         std::ranges::cend(str));
}

Zeus::UTF8::String& insert(
    Zeus::UTF8::String& string,
    Zeus::UTF8::String::const_iterator position,
    Zeus::UTF8::ValidCharacter character) {
    return string.insert(position, character);
}

Zeus::UTF8::String& insert(Zeus::UTF8::String& string,
                                  Zeus::UTF8::String::size_type index,
                                  Zeus::UTF8::String::const_pointer cstr,
                                  Zeus::UTF8::String::size_type count) {
    return string.insert(std::ranges::next(string.cbegin(), index),
                         cstr, count);
}

Zeus::UTF8::String& insert(Zeus::UTF8::String& string,
                           Zeus::UTF8::String::size_type index,
                           Zeus::UTF8::String const& other) {
    return string.insert(std::ranges::next(string.cbegin(), index),
                         other);
}

Zeus::UTF8::String& insert(Zeus::UTF8::String& string,
                           Zeus::UTF8::String::size_type index,
                           Zeus::UTF8::ValidCharacter character) {
    return string.insert(std::ranges::next(string.cbegin(), index),
                         character);
}

// ================================= APPEND ================================= //

Zeus::UTF8::String& append(Zeus::UTF8::String& str,
                           Zeus::UTF8::String::const_pointer cstr) {
    return Zeus::insert(str, str.cend(), cstr);
}

Zeus::UTF8::String& append(Zeus::UTF8::String& str,
                           Zeus::UTF8::String::const_pointer cstr,
                           Zeus::UTF8::String::size_type size) {
    return str.insert(str.cend(), cstr, size);
}

Zeus::UTF8::String& append(Zeus::UTF8::String& str,
                           Zeus::Unicode::CodePoint code_point) {
    auto const encoded_str = Zeus::UTF8::encode(code_point);
    return Zeus::append(str, encoded_str.c_str(), std::ssize(encoded_str));
}

Zeus::UTF8::String& append(Zeus::UTF8::String& str,
                           Zeus::UTF8::Character const& character) {
    return Zeus::append(str, std::ranges::begin(character),
                        std::ranges::end(character));
}

Zeus::UTF8::String& append(Zeus::UTF8::String& str,
                           Zeus::UTF8::ValidCharacter character) {
    return str.insert(str.cend(), character);
}

Zeus::UTF8::String& append(Zeus::UTF8::String& str,
                           Zeus::UTF8::String const& other) {
    return str.insert(str.cend(), other);
}

// ================================= ERASE ================================== //

std::optional<Zeus::UTF8::String::iterator> erase(
    Zeus::UTF8::String& str, Zeus::UTF8::String::size_type index) {
    if (index > str.size() || index < 0) {
        throw std::out_of_range{"Index is out of range."};
    }

    auto const position = std::ranges::next(str.cbegin(), index);

    return str.erase(position);
}

std::optional<Zeus::UTF8::String::iterator> erase(
    Zeus::UTF8::String& str, Zeus::UTF8::String::size_type index,
    Zeus::UTF8::String::size_type count) {
    ZEUS_ASSERT(count >= 0, "Cannot have negative count.");

    if (index > str.size() || index < 0) {
        throw std::out_of_range{"Index is out of range."};
    }

    auto const position = std::ranges::next(str.cbegin(), index);

    // Check if section is within string
    auto const is_valid_section = count != Zeus::UTF8::String::g_nposCodeUnit &&
                                  index + count <= str.length();

    auto const end = is_valid_section
                         ? std::ranges::next(str.cbegin(), count)
                         : str.cend();

    return str.erase(position, end);
}

void clear(Zeus::UTF8::String& string) noexcept { string.clear(); }

// ================================= SUBSTR ================================= //

std::optional<Zeus::UTF8::String> substr(Zeus::UTF8::String const& other,
                                         Zeus::UTF8::String::size_type index,
                                         Zeus::UTF8::String::size_type count) {
    // TODO(tristan): May have to update to use custom Zeus::size
    auto const size = std::ssize(other);

    // Preconditions
    ZEUS_ASSERT(index >= 0 && index < size, "Index is out of bounds.");
    ZEUS_ASSERT(count >= 0, "Count cannot be negative.");

    auto const max_count = size - index;
    auto const actual_count = std::min(max_count, count);

    auto const begin = std::ranges::next(other.cbegin(), index);
    auto const end = std::ranges::next(begin, actual_count);

    return Zeus::UTF8::String::create(begin, end);
}

bool is_empty(Zeus::UTF8::String const& string) noexcept {
    return std::ssize(string) == 0;
}

Zeus::UTF8::String::size_type length(
    Zeus::UTF8::String const& string) noexcept {
    return string.length();
}

}  // namespace Zeus
