#include "cell.h"

CellMatrix::CellMatrix(const Coord& c) : Matrix<Cell>(c) {
}

CellMatrix::CellMatrix(const Coord& c, const Cell& value) : Matrix<Cell>(c, value) {
}

uint32_t CellMatrix::NeighborsSum(const Coord& c) const {
    uint32_t sum = 0;
    auto size = Size();
    for (int j = -1; j <= 1; j++) {
        for (int i = -1; i <= 1; i++) {
            if (i == 0 && j == 0) {
            } else {
                const int rx = c.X + i;
                const int ry = c.Y + j;
                if (rx >= 0 && rx < size.X && ry >= 0 && ry < size.Y) {
                    sum += Get({rx, ry}).pow;
                }
            }
        }
    }
    return sum;
}
