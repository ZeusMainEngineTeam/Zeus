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

#include "zeus/string/utf8/code_unit.hpp"

namespace Zeus::UTF8 {

class Exception : public std::exception {};

class InvalidCodeUnit : public Exception {
   public:
    InvalidCodeUnit(Zeus::UTF8::CodeUnit code_unit) {}
   private:
};

class InvalidUTF8 : public Exception {};

}  // namespace Zeus::UTF8
