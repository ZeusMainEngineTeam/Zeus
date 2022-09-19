/**
 * This file is part of the Zeus Game Engine.
 * Copyright (C) 2022 Tristan F.
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

#include <string>

namespace Zeus {

namespace UTF8 {

/**
 * Error codes for UTF-8.
 */
enum class Error {
    /**
     * The UTF-8 character cannot fit in the space allotted.
     */
    eNotEnoughSpace,

    /**
     * An invalid UTF-8 code unit.
     */
    eInvalidCodeUnit
};

}  // namespace UTF8

[[nodiscard]] inline std::string to_string(Zeus::UTF8::Error error) {
    switch (error) {
        case Zeus::UTF8::Error::eNotEnoughSpace:
            return "eNotEnoughSpace";
            break;
        case Zeus::UTF8::Error::eInvalidCodeUnit:
            return "eInvalidCodeUnit";
            break;
    }
}

}  // namespace Zeus
