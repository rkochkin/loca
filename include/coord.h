#pragma once

#include <cstdint>

struct Coord {
    Coord(uint32_t x, uint32_t y) : X(x), Y(y) {
    }
    uint32_t X;
    uint32_t Y;
};