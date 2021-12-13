/**
 * This file is part of the Zeus Game Engine.
 * Copyright (C) 2021 Tristan F.
 *
 * The Zeus Game Engine is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The Zeus Game Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the Zeus Game Engine. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <cstdint>
#include <optional>
#include <sstream>
#include <string_view>

#include "zeus/core/compiler_macros.hpp"
#include "zeus/core/log.hpp"
#include "zeus/core/macro_helpers.hpp"

/**
 * @file assert.hpp
 */

namespace Zeus {

/**
 * Assertion against the given condition.
 *
 * @note Terminates the program if the given condition is false, otherwise the
 * program continues.
 *
 * @param condition         The condition to assert against
 * @param assertion_text    A string representation of the assertion
 * @param message           The message to output if the assertion fails
 * @param file_name         The file name where the assertion occurred
 * @param line_number       The line number where the assertion occurred
 */
ZEUS_FORCE_INLINE inline void assert_condition(
    bool condition, std::string_view assertion_text,
    std::optional<std::string_view> message, std::string_view file_name,
    std::int64_t line_number) {
    if (!condition) {
        std::stringstream stream;

        stream << "\n=========== ZEUS ASSERTION FAILED ===========\n";
        stream << "Assertion (" << assertion_text << ") failed!\n";
        stream << "File: " << file_name << '\n';
        stream << "Line: " << line_number << '\n';

        if (message.has_value()) {
            stream << "Message: " << message.value() << "\n";
        }

        stream << "==============================================\n";

        ZEUS_ERROR_LOG(stream.str());

        std::terminate();
    }
}

}  // namespace Zeus

#define ZEUS_ASSERT_2(ASSERTION, MESSAGE)                                     \
    Zeus::assert_condition(ASSERTION, ZEUS_STR(ASSERTION), MESSAGE, __FILE__, \
                           __LINE__)

#define ZEUS_ASSERT_1(ASSERTION) ZEUS_ASSERT_2(ASSERTION, std::nullopt)

#ifdef ZEUS_DEBUG
#define ZEUS_ASSERT(...) ZEUS_VA_SELECT(ZEUS_ASSERT, __VA_ARGS__)
#else
#define ZEUS_ASSERT(...)
#endif
