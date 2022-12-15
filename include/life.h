#pragma once
#include "cell.h"
#include "field.h"

class Life {
public:
    CellMatrix cellMatrix;
    FieldMatrix fieldMatrix;
    Coord m_dimension;

    Life() = default;

    Life(const Coord& dimension, const ActionFunc& actFn, int pow = 0) {
        cellMatrix = CellMatrix{dimension};
        fieldMatrix = FieldMatrix{dimension};

        for (uint32_t j = 0; j < dimension.Y; j++) {
            for (uint32_t i = 0; i < dimension.X; i++) {
                fieldMatrix.Get(Coord{i, j}) = actFn;
                cellMatrix.Get(Coord{i, j}).pow = pow;
            }
        }
        m_dimension = dimension;
    }

    void Quant() {
        CellMatrix out{cellMatrix};
        auto s = cellMatrix.Size();
        for (uint32_t j = 0; j < s.Y; j++) {
            for (uint32_t i = 0; i < s.X; i++) {
                Coord c{i, j};
                out.Get(c) = fieldMatrix.Get(c)(cellMatrix, c);
            }
        }
        cellMatrix = out;
    }
};
