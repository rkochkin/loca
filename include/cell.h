#pragma once

#include "matrix.h"

class Cell {
public:
    int pow = 0;
};

class CellMatrix : public Matrix<Cell> {
public:
    CellMatrix() = default;
    explicit CellMatrix(const Coord& c);
    explicit CellMatrix(const Coord& c, const Cell& value);
    [[nodiscard]] uint32_t NeighborsSum(const Coord& c) const;
};