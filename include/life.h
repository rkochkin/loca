#pragma once
#include "cell.h"
#include "field.h"

class Life {
public:
    CellMatrix cellMatrix;
    FieldMatrix fieldMatrix;
    Coord m_dimension;

    Life() = default;

    Life(const Coord& dimension, const ActionFunc& actFn, int pow = 0);

    void Quant();
};
