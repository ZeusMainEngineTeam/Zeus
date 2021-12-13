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

/**
 * @file compiler_macros.hpp
 */

#if defined(__GNUC__)
#define ZEUS_IS_GCC 1
#else
#define ZEUS_IS_GCC 0
#endif

#if defined(__clang__)
#define ZEUS_IS_CLANG 1
#else
#define ZEUS_IS_CLANG 0
#endif

#if defined(_MSC_VER)
#define ZEUS_IS_MSVC 1
#else
#define ZEUS_IS_MSVC 0
#endif

// Since they share similar macros
#define ZEUS_IS_GCC_OR_CLANG ZEUS_IS_GCC || ZEUS_IS_CLANG

// Force inlining if enabled
#if defined(ZEUS_ENABLE_FORCE_INLINE)

#if ZEUS_IS_GCC_OR_CLANG
#define ZEUS_FORCE_INLINE __attribute__((always_inline))
#elif ZEUS_IS_MSVC
#define ZEUS_FORCE_INLINE __forceinline
#else
#define ZEUS_FORCE_INLINE
#endif

#else
#define ZEUS_FORCE_INLINE
#endif

#define ZEUS_ERROR(x) static_assert(false, x);
