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

/**
 * @file macro_helpers.hpp
 */

// Make into string literal
#define ZEUS_STR(x) #x

// Concatenate two arguments together
#define ZEUS_CAT(A, B) A##B

// Creates NAME_NUM output
#define ZEUS_SELECT(NAME, NUM) ZEUS_CAT(NAME##_, NUM)

// Binds NAME to the given ARGS
#define ZEUS_BIND(NAME, ARGS) NAME ARGS

// ZEUS_VA_SIZE helper to get __VA_ARGS__ count
#define ZEUS_GET_COUNT(_1, _2, _3, _4, _5, _6, COUNT, ...) COUNT

// Gets the number of arguments from __VA_ARGS__
#define ZEUS_VA_SIZE(...) ZEUS_GET_COUNT(__VA_ARGS__, 6, 5, 4, 3, 2, 1, -1)

#define ZEUS_VA_SELECT(NAME, ...)                \
    ZEUS_SELECT(NAME, ZEUS_VA_SIZE(__VA_ARGS__)) \
    (__VA_ARGS__)
