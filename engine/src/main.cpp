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

#include <iostream>
#include <algorithm>

#include "zeus/config.hpp"
#include "zeus/cli/cli.hpp"

int main(int argc, char* argv[]) {
    std::cout << "Zeus Version: " << ZEUS_VERSION_MAJOR << '.'
              << ZEUS_VERSION_MINOR << '.' << ZEUS_VERSION_PATCH << '\n';

    auto const arguments = Zeus::CLI::parseCommandLine(argc, argv);

    //std::for_each(std::begin(arguments), std::end(arguments), Zeus::to_string);

    return EXIT_SUCCESS;
}




