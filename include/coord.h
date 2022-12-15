#pragma once

#include <cstdint>

struct Coord {
    Coord() = default;
    Coord(uint32_t x, uint32_t y) : X(x), Y(y) {
    }

    bool operator<(const Coord &rhs) const {
        if (Y < rhs.Y) {
            return true;
        } else if (Y == rhs.Y){
            if (X < rhs.X) {
                return true;
            }
        }
        return false;
    }

    uint32_t X = 0;
    uint32_t Y = 0;
};