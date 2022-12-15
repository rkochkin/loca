#pragma once

#include "matrix.h"

class Cell {
public:
    int pow = 0;
};

class CellMatrix : public Matrix<Cell> {
public:
    CellMatrix() = default;
    explicit CellMatrix(const Coord& c) : Matrix<Cell>(c) {
    }
    [[nodiscard]] uint32_t NeighborsSum(const Coord& c) const {
        uint32_t sum = 0;
        auto size = Size();
        for (int j = -1; j <= 1; j++) {
            for (int i = -1; i <= 1; i++) {
                if (i == 0 && j == 0) {
                } else {
                    int rx = c.X + i;
                    int ry = c.Y + j;
                    if (rx >= 0 && rx < static_cast<int32_t>(size.X) && ry >= 0 && ry < static_cast<int32_t>(size.Y)) {
                        sum += Get({static_cast<uint32_t>(rx), static_cast<uint32_t>(ry)}).pow;
                    }
                }
            }
        }
        return sum;
    }
};