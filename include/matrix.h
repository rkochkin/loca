#pragma once

#include "coord.h"
#include <cstdint>
#include <tuple>
#include <vector>

template<typename T>
class Matrix {
public:
    Matrix() = default;
    explicit Matrix(const Coord& c) {
        for (uint32_t i = 0; i < c.Y; i++) {
            m_matrix.emplace_back(std::vector<T>(c.X));
        }
    }

    [[nodiscard]] Coord Size() const {
        return {static_cast<uint32_t>(m_matrix.front().size()), static_cast<uint32_t>(m_matrix.size())};
    }

    [[nodiscard]] T& Get(const Coord& c) {
        return m_matrix[c.X][c.Y];
    }

    [[nodiscard]] const T& Get(const Coord& c) const {
        return m_matrix[c.X][c.Y];
    }

private:
    std::vector<std::vector<T>> m_matrix;
};
