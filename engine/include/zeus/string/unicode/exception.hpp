/**
 * This file is part of the Zeus Game Engine.
 * Copyright (C) 2022 Tristan F.
 *
 * The Zeus Game Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * The Zeus Game Engine is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * the Zeus Game Engine. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <stdexcept>
#include <string>

/**
 * @file string/unicode/exception.hpp
 *
 * Contains Unicode exceptions.
 */

namespace Zeus::Unicode::Exception {

/**
 * An exception for invalid Unicode code points.
 */
class InvalidCodePoint : public std::logic_error {
   public:
    explicit InvalidCodePoint(char const* msg) : std::logic_error{msg} {}

    explicit InvalidCodePoint(std::string const& msg) : std::logic_error{msg} {}

    ~InvalidCodePoint() override = default;

    InvalidCodePoint(InvalidCodePoint const&) noexcept = default;
    InvalidCodePoint(InvalidCodePoint&&) noexcept = default;

    InvalidCodePoint& operator=(InvalidCodePoint const&) noexcept = default;
    InvalidCodePoint& operator=(InvalidCodePoint&&) noexcept = default;
};

}  // namespace Zeus::Unicode::Exception
