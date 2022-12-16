#include "life.h"

Life::Life(const Coord& dimension, const ActionFunc& actFn, int pow) {
    cellMatrix = CellMatrix{dimension};
    fieldMatrix = FieldMatrix{dimension};

    for (int32_t j = 0; j < dimension.Y; j++) {
        for (int32_t i = 0; i < dimension.X; i++) {
            fieldMatrix.Get(Coord{i, j}) = actFn;
            cellMatrix.Get(Coord{i, j}).pow = pow;
        }
    }
    m_dimension = dimension;
}

void Life::Quant() {
    CellMatrix out{cellMatrix};
    auto s = cellMatrix.Size();
    for (int32_t j = 0; j < s.Y; j++) {
        for (int32_t i = 0; i < s.X; i++) {
            Coord c{i, j};
            out.Get(c) = fieldMatrix.Get(c)(cellMatrix, c);
        }
    }
    cellMatrix = out;
}
