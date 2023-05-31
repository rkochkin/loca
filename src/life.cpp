#include "life.h"

Life::Life(const Coord& dimension, const ActionFunc& actFn, int pow) {
    m_cellMatrix = CellMatrix{dimension};
    m_fieldMatrix = FieldMatrix{dimension};

    for (int32_t j = 0; j < dimension.Y; j++) {
        for (int32_t i = 0; i < dimension.X; i++) {
            m_fieldMatrix.Get(Coord{i, j}) = actFn;
            m_cellMatrix.Get(Coord{i, j}).pow = pow;
        }
    }
    m_dimension = dimension;
}

Cell& Life::GetCell(const Coord& c) {
    return m_cellMatrix.Get(c);
}

void Life::ForEachCell(const std::function<void(const Coord& c, const Cell& item)>& fn) const  {
    m_cellMatrix.ForEach(fn);
}

const Coord& Life::GetDimension() const {
    return m_dimension;
}

void Life::Quant() {
    CellMatrix out{m_cellMatrix};
    auto s = m_cellMatrix.Size();
    for (int32_t j = 0; j < s.Y; j++) {
        for (int32_t i = 0; i < s.X; i++) {
            Coord c{i, j};
            out.Get(c) = m_fieldMatrix.Get(c)(m_cellMatrix, c);
        }
    }
    m_cellMatrix = out;
}
