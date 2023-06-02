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

Cell& Life::GetCell(const Coord& c) { return m_cellMatrix.Get(c); }

void Life::ForEachCell(const std::function<void(const Coord& c, const Cell& item)>& fn) const {
    m_cellMatrix.ForEach(fn);
}

const Coord& Life::GetDimension() const { return m_dimension; }

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

Cell Life::lifeGameDefaultStrategy(const CellMatrix& cellMatrix, const Coord& selfCoord) {
    Cell cell = cellMatrix.Get(selfCoord);
    const auto sum = cellMatrix.NeighborsSum(selfCoord);
    if (cellMatrix.Get(selfCoord).pow > 0) {// Если клетка живая
        if (sum == 2 || sum == 3)           // Если есть 2 или 3 живые соседки
            cell.pow = 1;                   // то клетка продолжает жить
        else
            cell.pow = 0;// иначе умирает
    } else {             // Если пусто
        if (sum == 3)    // Если есть ровно 3 живые соседки
            cell.pow = 1;// зарождается жизнь в клетке
        else
            cell.pow = 0;
    }
    return cell;
}
