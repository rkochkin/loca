#pragma once

#include <cstdint>

struct Coord {
    Coord() = default;
    Coord(int32_t x, int32_t y) : X(x), Y(y) {
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

    int32_t X = 0;
    int32_t Y = 0;
};