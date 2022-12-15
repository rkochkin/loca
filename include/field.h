#pragma once

#include "matrix.h"
#include <functional>

using ActionFunc = std::function<Cell(const CellMatrix&, const Coord& c)>;

class FieldMatrix : public Matrix<ActionFunc> {
public:
    FieldMatrix() = default;
    explicit FieldMatrix(const Coord& c) : Matrix<ActionFunc>(c) {
    }
};
