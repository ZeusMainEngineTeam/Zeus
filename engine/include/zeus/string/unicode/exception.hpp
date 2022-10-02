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

#include <stdexcept>
#include <string>

namespace Zeus::Unicode {

class Exception : public std::logic_error {
   public:
    explicit Exception(char const* message)
        : std::logic_error{message} {}

    ~Exception() override = default;

    Exception(Exception const&) noexcept = default;
    Exception(Exception&&) noexcept = default;

    Exception& operator=(Exception const&) noexcept = default;
    Exception& operator=(Exception&&) noexcept = default;
};

}  // namespace Zeus::Unicode
