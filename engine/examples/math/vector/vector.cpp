#include <iostream>

#include "zeus/math/vector_2d.hpp"

int main() {
    // A super simple example
    constexpr Zeus::Math::Vector2D const vector = {1, 2};

    std::cout << "x: " << vector.x << " y: " << vector.y << std::endl;

    return EXIT_SUCCESS;
}
