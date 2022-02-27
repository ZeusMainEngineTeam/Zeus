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

#include <array>

#include "zeus/core/assert.hpp"
#include "zeus/core/types.hpp"
#include "zeus/math/type_traits.hpp"


/**
 * @file matrix.hpp
 */

namespace Zeus {

namespace Math {


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
template <typename T, ssize M, ssize N>
class Matrix {

    public:
        static_assert(M > 0 && N > 0, "Dimensions of a matrix cannot be negative!");
        using value_type = T;
        using reference = value_type&;
        using const_reference = value_type const&;
        using this_type = Matrix<value_type, M, N>;
        using size_type = Zeus::ssize;
        
        Matrix() = default;
        ~Matrix() = default;
        [[nodiscard]] constexpr size_type size() const noexcept{return M * N;};

    private:
        std::array<std::array <int, M>, N> m_data = {};
        

};







} // namespace Math

}  // namespace Zeus
