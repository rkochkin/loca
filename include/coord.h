#pragma once

#include <cstdint>

struct Coord {
    Coord() = default;
    Coord(uint32_t x, uint32_t y) : X(x), Y(y) {
    }
    uint32_t X = 0;
    uint32_t Y = 0;
};