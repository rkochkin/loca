#pragma once
#include "cell.h"
#include "field.h"

class Life {
public:
    Life() = default;

    Life(const Coord& dimension, const ActionFunc& actFn = lifeGameDefaultStrategy, int pow = 0);

    Cell& GetCell(const Coord& c);

    void ForEachCell(const std::function<void(const Coord& c, const Cell& item)>& fn) const;

    const Coord& GetDimension() const;

    void Quant();

    static Cell lifeGameDefaultStrategy(const CellMatrix& cellMatrix, const Coord& selfCoord);

private:
    CellMatrix m_cellMatrix;
    FieldMatrix m_fieldMatrix;
    Coord m_dimension;
};
